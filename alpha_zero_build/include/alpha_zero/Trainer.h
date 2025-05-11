#pragma once

#include <algorithm>
#include <random>
#include <filesystem>
#include <iostream>

#include "MCTS.h"
#include "CustomDataset.h"



class Trainer{
public:
    Trainer(GomokuTraining* game, Model* model, int depth, int num_iterations, int num_episodes, int num_epochs, int batch_size, std::string checkpoint_path);
    Trainer(const Trainer& other);
    PlayOut play(Matrix<int>& state_matrix, int current_player);
    std::vector<PlayOutReward> execute_episode();
    void learn();
    void learn_parallel(const int num_threads);
    void train_iteration(std::vector<PlayOutReward>& train_examples);


private:
    Model* model;
    GomokuTraining* game;
    int depth;
    int num_iterations;
    int num_episodes;
    int num_epochs;
    int batch_size;
    std::string checkpoint_path;

    torch::Tensor get_loss_pi(torch::Tensor target, torch::Tensor prediction);
    torch::Tensor get_loss_v(torch::Tensor target, torch::Tensor prediction);
    void save_checkpoint(const std::string& filename);

    template <typename T>
    void shuffle(std::vector<T>& vector) {
        std::random_device rd;
        std::mt19937 g(rd());
        std::shuffle(vector.begin(), vector.end(), g);
    }
    float average(std::vector<float> const& v){
        if(v.empty()){
            return 0;
        }
    
        auto const count = static_cast<float>(v.size());
        return std::reduce(v.begin(), v.end()) / count;
    }
};


