#pragma once

#include <vector>
#include <tuple>
#include <torch/torch.h>
#include "PlayOut.h"

struct CustomExample{
    CustomExample(){}
    CustomExample(torch::Tensor data, torch::Tensor action_probs, torch::Tensor value)
    : data(std::move(data)),
    action_probs(std::move(action_probs)),
    value(std::move(value)){}
    
    torch::Tensor data;
    torch::Tensor action_probs;
    torch::Tensor value;
};

class CustomDataset : public torch::data::Dataset<CustomDataset, CustomExample>
{
    private:
        const std::vector<PlayOutReward> data;

    public:
        explicit CustomDataset(std::vector<PlayOutReward>& data)
            // Load csv file with file locations and labels.
            : data(data) {

        };

        // Override the get method to load custom data.
        CustomExample get(size_t index) override {

            PlayOutReward sample = data[(int)index];

            Matrix<int> game_state = sample.old_state;
            torch::Tensor game_state_tensor = torch::from_blob(
            game_state.getVector().data(),
            {game_state.get_num_rows(), game_state.get_num_cols()},
            torch::kInt32 // Explicitly specify the data type as signed 32-bit integers
            ).clone(); // Clone to ensure the tensor owns the data
            game_state_tensor = game_state_tensor.to(torch::kFloat);


            Matrix<float> action_probs = sample.action_probs;
            torch::Tensor action_probs_tensor = torch::from_blob(
            action_probs.getVector().data(),
            {action_probs.get_num_rows(), action_probs.get_num_cols()},
            torch::kFloat // Explicitly specify the data type as signed 32-bit integers
            ).clone(); // Clone to ensure the tensor owns the data

            float reward = sample.reward;
            torch::Tensor reward_tensor = torch::tensor(reward, torch::kFloat);

            return CustomExample(game_state_tensor, action_probs_tensor, reward_tensor);
        };

        // Override the size method to infer the size of the data set.
        torch::optional<size_t> size() const override {

            return data.size();
        };
};

struct Stack : public torch::data::transforms::Collation<CustomExample> {
    CustomExample apply_batch(std::vector<CustomExample> examples) override {
      std::vector<torch::Tensor> data, action_probs, value;
      data.reserve(examples.size());
      action_probs.reserve(examples.size());
      value.reserve(examples.size());
      for (auto &example : examples) {
        data.push_back(std::move(example.data));
        action_probs.push_back(std::move(example.action_probs));
        value.push_back(std::move(example.value));
      }
      return {torch::stack(data), torch::stack(action_probs), torch::stack(value)};
    }
  };