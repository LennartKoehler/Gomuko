#pragma once
#include "../src/Game_Common/VectorMatrix.hpp"
#include <torch/torch.h>


template <typename T, typename U>
struct Prediction {
    Prediction(){}
    Prediction(T action_probs, U value)
        : action_probs(action_probs), value(value) {}

    T action_probs; // Can be Matrix<double> or torch::Tensor
    U value;        // Can be double or torch::Tensor
};

class Model : public torch::nn::Module{
public:
    Model(int size): size(size) {

        // Construct and register two Linear submodules.
        int conv_dim = 32;
        int fc_dim = 1;
        conv1 = register_module("conv1", torch::nn::Conv2d(torch::nn::Conv2dOptions(1, conv_dim, 3).padding(1).stride(1)));
        fc1 = register_module("fc1", torch::nn::Linear(conv_dim*size, conv_dim*size*fc_dim));
        fc2 = register_module("fc2", torch::nn::Linear(conv_dim*size*fc_dim, conv_dim*size*fc_dim));
        action_head = register_module("action_head", torch::nn::Linear(conv_dim*size*fc_dim, size));
        value_head = register_module("value_head", torch::nn::Linear(conv_dim*size*fc_dim, 1));
    }




    Prediction<torch::Tensor, torch::Tensor> forward(torch::Tensor x) {

        x = x.to(this->parameters().front().device()); // depending on if this module is on cuda or not

        x = torch::relu(conv1->forward(x));
        x = x.flatten(1,3);

        x = torch::relu(fc1->forward(x));
        // x = torch::dropout(x, /*p=*/0.5, /*train=*/is_training());
        x = torch::relu(fc2->forward(x));
        torch::Tensor actions = torch::softmax(action_head->forward(x), /*dim=*/-1);
        torch::Tensor value = torch::tanh(value_head->forward(x));

        // return Prediction<torch::Tensor, torch::Tensor>(actions, value);
        return Prediction<torch::Tensor, torch::Tensor>(actions.to(at::kCPU), value.to(at::kCPU));
    }

    virtual Prediction<Matrix<float>, float> predict(Matrix<int> game_state){
        torch::Tensor game_state_tensor = torch::from_blob(
            game_state.getVector().data(),
            {game_state.get_num_rows(), game_state.get_num_cols()},
            torch::kInt32 // Explicitly specify the data type as signed 32-bit integers
        ).clone(); // Clone to ensure the tensor owns the data
        game_state_tensor = game_state_tensor.to(torch::kFloat).unsqueeze(0).unsqueeze(0); // add channel and batch dimension

        eval();
        torch::NoGradGuard no_grad;
        Prediction result = forward(game_state_tensor);
 
        std::vector<float> actions(result.action_probs.data_ptr<float>(), result.action_probs.data_ptr<float>() + result.action_probs.numel());
        float value(*result.value.data_ptr<float>());
        Matrix<float> action_probs = {game_state.get_num_rows(), game_state.get_num_cols(), actions};
        Prediction<Matrix<float>, float> prediction = {action_probs, value};
        return prediction;
    }

    torch::nn::Linear fc1{nullptr}, fc2{nullptr}, action_head{nullptr}, value_head{nullptr};
    torch::nn::Conv2d conv1{nullptr};
    int size;
};

// TORCH_MODULE(Model); // defines: using MyModel = std::shared_ptr<MyModelImpl>;
