#pragma once

#include "../src/Game_Common/VectorMatrix.hpp"

class Action;

class Gomoku: public Matrix<int>{
public:
    Gomoku(int size, int win_condition_number);
    bool player_won(const Matrix<int>& state_matrix, int playerID);
    bool is_tie(const Matrix<int>& state_matrix);
    Matrix<int> get_next_state(Matrix<int>& state_matrix, int player_id, const Action& action);
    Matrix<int> get_valid_moves(const Matrix<int>& state_matrix);
    Matrix<int> get_canonical(const Matrix<int>& state_matrix, int player_id);
    Matrix<int> get_init_board();

private:
    int size;
    int win_condition_number;
};



class GomokuTraining: public Gomoku{
public:
    GomokuTraining(int size, int win_condition_number, float win_reward, float tie_reward, float lose_reward, float no_reward);
    float get_reward(const Matrix<int>& state_matrix, int player_id);
    float get_no_reward();

private:
    float win_reward;
    float tie_reward;
    float lose_reward;
    float no_reward;
};