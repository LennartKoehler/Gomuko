#include "Trainer.h"

std::string getTimeStamp(){
    auto now = std::chrono::system_clock::now();
    std::time_t now_time = std::chrono::system_clock::to_time_t(now);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&now_time), "%Y_%m_%d_%H_%M_%S_");
    std::string timestamp = ss.str();
    return timestamp;
}

int main(){


    int size = 7;
    int win_con = 4;
    GomokuTraining* gomoku = new GomokuTraining(size, win_con, 1, 0, -1, 2);
    std::shared_ptr<AZeroNNParallel> model_parallel = std::make_shared<AZeroNNParallel>(4, size * size);
    // Model* model = new Model(size*size);

    bool use_cuda = true;
    if (use_cuda){
        // model->to(at::kCUDA);
        model_parallel->to(at::kCUDA);
    }


    int depth = 200;
    int num_iterations = 100;
    int num_episodes = 10;
    int num_epochs = 5;
    int batch_size = 8;
    int threads = 4; // at many threads it seems to block



    std::string checkpoint_path = "../models/gomoku/" + std::string("size_") + std::to_string(size) + "_" + getTimeStamp();


    TrainerParallel trainer = {threads, gomoku, model_parallel, depth, num_iterations, num_episodes, num_epochs, batch_size, checkpoint_path};
    // trainer.loadModel("/home/lennart/cpp_projects/game_engine/alpha_zero/models/gomoku/2025_05_16_15_00_27_state_dict.pt");
    trainer.learn();

    return 0;
}