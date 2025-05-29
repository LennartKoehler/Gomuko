#pragma once
#include "../src/Game_Client/VectorMatrix.hpp"
#include <torch/torch.h>


template <typename T, typename U>
struct Prediction {
    Prediction(){}
    Prediction(T action_probs, U value)
        : action_probs(action_probs), value(value) {}

    T action_probs; // Can be Matrix<double> or torch::Tensor
    U value;        // Can be double or torch::Tensor
};


class ResidualLayer : public torch::nn::Module{
public:
    ResidualLayer(int channels_in, int channels_out){
        conv1 = register_module("conv1", torch::nn::Conv2d(torch::nn::Conv2dOptions(channels_in, channels_out, 3).padding(1).stride(1)));
        conv2 = register_module("conv2", torch::nn::Conv2d(torch::nn::Conv2dOptions(channels_out, channels_out, 3).padding(1).stride(1)));
        batch_norm1 = register_module("batch_norm1", torch::nn::BatchNorm2d(torch::nn::BatchNorm2dOptions(channels_out).eps(0.1).affine(false)));
        batch_norm2 = register_module("batch_norm2", torch::nn::BatchNorm2d(torch::nn::BatchNorm2dOptions(channels_out).eps(0.1).affine(false)));
    }

    torch::Tensor forward(torch::Tensor x) {
        torch::Tensor skip = x;
        x = conv1(x);
        x = torch::relu(batch_norm1(x));
        x = conv2(x);
        x = batch_norm2(x);
        x = torch::relu(skip + x);

        return x;
    }

private:
    torch::nn::Conv2d conv1{nullptr};
    torch::nn::Conv2d conv2{nullptr};
    torch::nn::BatchNorm2d batch_norm1{nullptr};
    torch::nn::BatchNorm2d batch_norm2{nullptr};

};

class AZeroNN : public torch::nn::Module{
public:
    AZeroNN(int size): size(size) {

        // Construct and register two Linear submodules.
        int conv_dim = 256;
        residualLayers = torch::nn::Sequential(
            register_module("conv", torch::nn::Conv2d(torch::nn::Conv2dOptions(1, conv_dim, 3).padding(1).stride(1))),
            register_module("residual1", std::make_shared<ResidualLayer>(conv_dim, conv_dim)),
            register_module("residual2", std::make_shared<ResidualLayer>(conv_dim, conv_dim)),
            register_module("residual3", std::make_shared<ResidualLayer>(conv_dim, conv_dim)),
            register_module("residual4", std::make_shared<ResidualLayer>(conv_dim, conv_dim)),
            register_module("residual5", std::make_shared<ResidualLayer>(conv_dim, conv_dim)),
            register_module("residual6", std::make_shared<ResidualLayer>(conv_dim, conv_dim)),
            register_module("residual7", std::make_shared<ResidualLayer>(conv_dim, conv_dim)),
            register_module("residual8", std::make_shared<ResidualLayer>(conv_dim, conv_dim)),
            register_module("residual9", std::make_shared<ResidualLayer>(conv_dim, conv_dim))
        );

        action_head = register_module("action_head", torch::nn::Linear(conv_dim*size, size));
        value_head = register_module("value_head", torch::nn::Linear(conv_dim*size, 1));
    }




    Prediction<torch::Tensor, torch::Tensor> forward(torch::Tensor x) {
        x = x.to(this->parameters().front().device()); // move data to model device

        x = residualLayers->forward(x);

        x = x.flatten(1,3);
        torch::Tensor actions = torch::softmax(action_head->forward(x), /*dim=*/-1);
        torch::Tensor value = torch::tanh(value_head->forward(x));

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
    torch::nn::Sequential residualLayers{nullptr};

    int size;
};

// TORCH_MODULE(Model); // defines: using MyModel = std::shared_ptr<MyModelImpl>;
