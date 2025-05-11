#pragma once
#include "Model.h"
#include <condition_variable>
#include <mutex>
#include <thread>
#include <vector>

class ModelParallel : public Model {
public:
    
    ModelParallel(int threads, int size) : Model(size), threads(threads), num_requests(0) {}

    void reduce_threads(){
        std::unique_lock<std::mutex> lock(mutex);
        threads--;
        global_cv.notify_all();
    }

    void set_threads(int threads){
        std::unique_lock<std::mutex> lock(mutex);
        this->threads = threads;
    }

    // Thread-safe predict method
    Prediction<Matrix<float>, float> predict(Matrix<int> game_state) override {
        std::unique_lock<std::mutex> lock(mutex);
        // Add the game state to the batch
        game_states.push_back(game_state);

        // Increment the number of requests
        int request_id = num_requests++;
        if (request_id+1 != threads){
            global_cv.wait(lock, [&]() {
                // Recheck both conditions: result is ready or batch is full
                return predictions.find(request_id) != predictions.end() || request_id+1 == threads;
            });
        }
        // If the batch is full, process predictions
        if (request_id+1 == threads) {
            process_predictions();
        }

        // Retrieve the result for this thread
        Prediction<Matrix<float>, float> result = predictions[request_id];
        predictions.erase(request_id); // Clean up the result
        // std::cerr << "Thread " << request_id << " result is done!" << std::endl;
        return result;
    }

private:
    void process_predictions() {
        // Process the batch of game states
        std::vector<torch::Tensor> input_tensors;
        for (auto& game_state : game_states) {
            torch::Tensor game_state_tensor = torch::from_blob(
                game_state.getVector().data(),
                {game_state.get_num_rows(), game_state.get_num_cols()},
                torch::kInt32
            ).clone();
            game_state_tensor = game_state_tensor.to(torch::kFloat).unsqueeze(0).unsqueeze(0);
            input_tensors.push_back(game_state_tensor);
        }

        // Concatenate tensors into a batch
        torch::Tensor batch_tensor = torch::cat(input_tensors, 0);

        // Perform the forward pass
        eval();
        torch::NoGradGuard no_grad;
        Prediction<torch::Tensor, torch::Tensor> batch_result = forward(batch_tensor);

        // Split the batch results into individual predictions
        auto action_probs_list = batch_result.action_probs.split(1, 0);
        auto value_list = batch_result.value.split(1, 0);

        // Notify each thread of its result
        for (int i = 0; i < threads; i++) {
            std::vector<float> actions(
                action_probs_list[i].data_ptr<float>(),
                action_probs_list[i].data_ptr<float>() + action_probs_list[i].numel()
            );
            float value = *value_list[i].data_ptr<float>();
            Matrix<float> action_probs = {game_states[i].get_num_rows(), game_states[i].get_num_cols(), actions};
            predictions[i] = {action_probs, value};
        }
        global_cv.notify_all();

        // Clear the batch
        game_states.clear();
        num_requests = 0;
    }

    std::vector<Matrix<int>> game_states; // Batch of game states
    std::condition_variable global_cv;
    std::vector<std::condition_variable*> results_ready; // Condition variables for each thread
    std::map<int, Prediction<Matrix<float>, float>> predictions; // Results for each thread
    int num_requests; // Number of requests in the current batch
    int threads; // Batch size
    std::mutex mutex; // Mutex for thread-safe access
};