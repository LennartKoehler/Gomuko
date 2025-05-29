#pragma once

#include "Player.h"
#include "../../../alpha_zero/Trainer.h"

class AZero : public Player{
public:
    AZero(int playerID, int depths) : Player(playerID){
        int size = 15; //TODO train on 20x20? game sizes dont match i think IMPORTANT
        int win_con = 5;

        GomokuTraining* gomoku = new GomokuTraining(size, win_con, 1, 0, -1, 2);
        std::shared_ptr<AZeroNN> model = std::make_shared<AZeroNN>(size*size);
        int depth = 100;

        gomokuPlayer = new AZeroPlayer(gomoku, model, depth);
        gomokuPlayer->loadModel("../../../alpha_zero/models/gomoku/2025_05_09_21_15_56_state_dict.pt");
    }
    
    Matrix<int> takeTurn(const Matrix<int>& stateMatrix) override{
        Matrix<int> copy = stateMatrix.replace(2, -1);
        PlayOut out = gomokuPlayer->play(copy, -1);
        copy = out.new_state.replace(-1, 2); //TODO check if the out.new_state needs to be inverted(for other player)
        return copy;
    }

private:
    AZeroPlayer* gomokuPlayer;
};