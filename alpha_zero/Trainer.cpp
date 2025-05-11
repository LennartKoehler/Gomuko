#include "Trainer.h"
#include <chrono>
#include <omp.h>
#include <ctime>
#include <sstream>
#include <iomanip>

#include <thread>
#include <mutex>
#include <queue>
#include <condition_variable>

AZeroPlayer::AZeroPlayer(GomokuTraining* game, std::shared_ptr<Model> model, int depth) : game(game), model(model), depth(depth){}

PlayOut AZeroPlayer::play(Matrix<int>& game_state, int player_id){
    Matrix<int> canonical_board = game->get_canonical(game_state, player_id);

    MCTS mcts = {game, model, depth};

    std::shared_ptr<Node> root = mcts.run(canonical_board, player_id);

    Matrix<float> action_probs = Matrix<float>(game_state, 0);

    for (const auto& [action, child] : root->children) {
        action_probs.setValue(action.row, action.col, child->visit_count);
    }

    action_probs = action_probs / action_probs.sum();

    Action action = root->select_action(0); // TODO temperature

    Matrix<int> next_state = game->get_next_state(game_state, player_id, action);

    return PlayOut(game_state, player_id * -1, action_probs, next_state);
}

void AZeroPlayer::loadModel(std::string model_path){
    std::shared_ptr<Model> model_loaded = std::make_shared<Model>(15*15);
    torch::load(model_loaded, model_path);
    model_loaded->to(model->parameters().front().device()); // load to device of model used for init
    model = model_loaded;

}




Trainer::Trainer(GomokuTraining* game, std::shared_ptr<Model> model, int depth, int num_iterations, int num_episodes, int num_epochs, int batch_size, std::string checkpoint_path)
: AZeroPlayer(game, model, depth), num_iterations(num_iterations), num_episodes(num_episodes), num_epochs(num_epochs), batch_size(batch_size), checkpoint_path(checkpoint_path){}


Trainer::Trainer(const Trainer& other)
    : AZeroPlayer(new GomokuTraining(*other.game), other.model, other.depth),
      num_iterations(other.num_iterations),
      num_episodes(other.num_episodes),
      num_epochs(other.num_epochs),
      batch_size(other.batch_size),
      checkpoint_path(other.checkpoint_path) {}




std::vector<PlayOutReward> Trainer::execute_episode(){
    std::vector<PlayOut> train_examples;
    int current_player = 1;
    Matrix<int> state = game->get_init_board();

    while(true){
        PlayOut out = play(state, current_player);
        current_player = out.player;
        state = out.new_state;
        train_examples.push_back(out);
        float reward = game->get_reward(state, current_player);
        if (reward != game->get_no_reward()){
            std::vector<PlayOutReward> ret;
            for (PlayOut& example : train_examples){
                float player_reward = reward * ((example.player != current_player) ? -1.0f : 1.0f);
                ret.emplace_back(PlayOutReward(example, player_reward));
            }
            return ret;
        }
    }

}






void Trainer::learn(){
    for (int i=0; i<num_iterations; i++){
        std::vector<PlayOutReward> train_examples;
        auto start = std::chrono::high_resolution_clock::now();

        for (int e=0; e<num_episodes; e++){
            std::cout << e << std::endl;
            std::vector<PlayOutReward> examples = execute_episode();
            train_examples.insert(train_examples.end(), examples.begin(), examples.end());
        }

        shuffle(train_examples);
        train_iteration(train_examples);
        save_checkpoint("state_dict.pt");

        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = end - start;
        std::cout << "Iteration " << i << " took " << elapsed.count() << " s" << std::endl << std::endl;
    }
}

void Trainer::train_iteration(std::vector<PlayOutReward>& train_examples){
    torch::optim::SGD optimizer(model->parameters(), /*lr=*/0.01); //TODO reduce lr over time
    std::vector<float> pi_losses;
    std::vector<float> v_losses;

    auto data = CustomDataset(train_examples).map(Stack());

    auto data_loader = torch::data::make_data_loader<torch::data::samplers::SequentialSampler>(
        std::move(data), 
        batch_size);

    for (int epoch=0; epoch<num_epochs; epoch++){
        model->train();
        
        CustomExample last_batch; // Variable to store the last batch
        Prediction<torch::Tensor, torch::Tensor> last_prediction;

        for (auto& batch : *data_loader){
            Prediction<torch::Tensor, torch::Tensor> prediction = model->forward(batch.data.unsqueeze(1));
            torch::Tensor l_pi = get_loss_pi(batch.action_probs.flatten(1,2), prediction.action_probs);
            torch::Tensor l_v = get_loss_v(batch.value, prediction.value);

            torch::Tensor total_loss = l_v + l_pi;

            optimizer.zero_grad();
            total_loss.backward();
            optimizer.step();

            pi_losses.push_back(*l_pi.data_ptr<float>());
            v_losses.push_back(*l_v.data_ptr<float>());

            last_batch = batch;
            last_prediction = prediction;

        }
        // std::cout << "Action Probs Prediction: " << last_prediction.action_probs.index({0}) << std::endl;
        // std::cout << "Action Probs Targets: " << last_batch.action_probs.index({0}) << std::endl;

        std::cout << "Action Probs Loss: " << average(pi_losses) << std::endl;
        std::cout << "Value Loss: " << average(v_losses) << std::endl;
    }
}

torch::Tensor Trainer::get_loss_pi(torch::Tensor target, torch::Tensor prediction){
    torch::Tensor loss = -(target * torch::log(prediction)).sum(1);
    return loss.mean();
}

torch::Tensor Trainer::get_loss_v(torch::Tensor target, torch::Tensor prediction){
    torch::Tensor loss = torch::mean(torch::exp2(target - prediction.view(-1)));
    return loss;
}

void Trainer::save_checkpoint(const std::string& filename) {
    namespace fs = std::filesystem;

    // Create the folder if it doesn't exist
    if (!fs::exists(checkpoint_path)) {
        fs::create_directory(checkpoint_path);
    }

    std::string filepath = checkpoint_path + filename;    

    model->to(at::kCPU);
    torch::save(model, filepath);
    model->to(at::kCUDA);
}


TrainerParallel::TrainerParallel(
     int num_threads,
     GomokuTraining* game,
     std::shared_ptr<ModelParallel> model,
     int depth,
     int num_iterations,
     int num_episodes,
     int num_epochs,
     int batch_size,
     std::string checkpoint_path)
        : Trainer(game, model, depth, num_iterations, num_episodes, num_epochs, batch_size, checkpoint_path), num_threads(num_threads), model(model){}

TrainerParallel::TrainerParallel(const TrainerParallel& other)
    : Trainer(other), // Call the copy constructor of the Trainer base class
      num_threads(other.num_threads), // Copy the number of threads
      model(other.model){}


void TrainerParallel::learn() {
    
    std::vector<std::thread> threads;
    std::mutex queue_mutex; // Mutex to protect the work queue
    std::mutex results_mutex; // Mutex to protect shared results
    std::condition_variable cv; // Condition variable for thread synchronization
    std::queue<int> work_queue; // Queue of episode indices
    bool done = false; // Flag to indicate when all work is done

    // Populate the work queue with episode indices
    for (int e = 0; e < num_episodes; e++) {
        work_queue.push(e);
    }
    std::shared_ptr<ModelParallel> model;
    for (int i = 0; i < num_iterations; i++) {
        std::vector<PlayOutReward> train_examples; // Shared vector for all examples
        auto start = std::chrono::high_resolution_clock::now();

        // Launch threads
        for (int t = 0; t < num_threads; t++) {
            threads.emplace_back([&, t]() {
                TrainerParallel thread_trainer(*this); // Thread-local copy of Trainer
                model = std::dynamic_pointer_cast<ModelParallel>(thread_trainer.model);

                while (true) {
                    int episode_index;

                    // Get the next episode index from the queue
                    {
                        std::unique_lock<std::mutex> lock(queue_mutex);
                        if (work_queue.empty()) {
                            model->reduce_threads();
                            break; // Exit if no more work is available
                        }

                        cv.wait(lock, [&]() { return !work_queue.empty() || done; });

                        episode_index = work_queue.front();

                        work_queue.pop();
                    }

                    // Process the episode
                    std::vector<PlayOutReward> examples = thread_trainer.execute_episode();

                    // Store the results
                    {
                        std::lock_guard<std::mutex> lock(results_mutex);
                        train_examples.insert(train_examples.end(), examples.begin(), examples.end());
                    }
                }
            });
        }

        // Wait for all threads to finish
        for (auto& thread : threads) {
            thread.join();
        }
        threads.clear(); // Clear the thread vector for the next iteration
        model->set_threads(num_threads);

        // Shuffle and train
        shuffle(train_examples);
        train_iteration(train_examples);
        save_checkpoint("state_dict.pt");

        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = end - start;
        std::cout << "Iteration " << i << " took " << elapsed.count() << " s" << std::endl << std::endl;

        // Reset the work queue for the next iteration
        {
            std::lock_guard<std::mutex> lock(queue_mutex);
            for (int e = 0; e < num_episodes; e++) {
                work_queue.push(e);
            }
            done = false;
        }
    }

    // Notify all threads to exit
    {
        std::lock_guard<std::mutex> lock(queue_mutex);
        done = true;
        cv.notify_all();
    }
}