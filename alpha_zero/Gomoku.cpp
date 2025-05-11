#include "Gomoku.h"
#include "MCTS.h"

Gomoku::Gomoku(int size, int win_condition_number): size(size), win_condition_number(win_condition_number){}

Matrix<int> Gomoku::get_init_board(){
    Matrix<int> m{size, size, 0};
    m.setValue(size/2, size/2, -1);
    return m;
}

bool Gomoku::player_won(const Matrix<int>& state_matrix, int playerID) {
    int n_rows = state_matrix.get_num_rows();
    int n_cols = state_matrix.get_num_cols();

    // not sure if i got cols and rows mixed up somewhere here
    // Check rows and columns
    for (int i = 0; i < n_rows; ++i) {
        int rowCount = 0, colCount = 0;
        for (int j = 0; j < n_cols; ++j) {
            // Check row
            if (state_matrix.getValue(i,j) == playerID) {
                colCount++;
                if (colCount >= win_condition_number) return true;
            } else {
                colCount = 0;
            }

            // Check column
            if (state_matrix.getValue(j,i) == playerID) {
                rowCount++;
                if (rowCount >= win_condition_number) return true;
            } else {
                rowCount = 0;
            }
        }
    }

    // Check diagonals
    for (int d = -n_rows + 1; d < n_cols; ++d) {
        int mainDiagonalCount = 0, antiDiagonalCount = 0;
        for (int i = 0; i < n_rows; ++i) {
            int j = i + d;
            // Check main diagonal
            if (j >= 0 && j < n_rows) {
                if (state_matrix.getValue(i,j) == playerID) {
                    mainDiagonalCount++;
                    if (mainDiagonalCount >= win_condition_number) return true;
                } else {
                    mainDiagonalCount = 0;
                }
            }

            // Check anti-diagonal
            int antiJ = n_rows - 1 - i + d;
            if (antiJ >= 0 && antiJ < n_rows) {
                if (state_matrix.getValue(i,antiJ) == playerID) {
                    antiDiagonalCount++;
                    if (antiDiagonalCount >= win_condition_number) return true;
                } else {
                    antiDiagonalCount = 0;
                }
            }
        }
    }

    return false;
}

bool Gomoku::is_tie(const Matrix<int>& state_matrix){
    for (auto it = state_matrix.begin(); it != state_matrix.end(); ++it){
        MatrixElement ME = *it;
        if (ME.getValue() == 0){
            return false;
        }
    }
    return true;
}

Matrix<int> Gomoku::get_next_state(Matrix<int>& state_matrix, int player_id, const Action& action){
    state_matrix.setValue(action.row, action.col, player_id);
    return state_matrix;
}

Matrix<int> Gomoku::get_valid_moves(const Matrix<int>& state_matrix){
    Matrix<int> valid_moves = Matrix<int>(state_matrix, 0);
    for (auto it = state_matrix.begin(); it != state_matrix.end(); ++it){
        MatrixElement ME = *it;
        if (ME.getValue() == 0){
            valid_moves.setValue(ME.getRow(), ME.getCol(), 1);
        }

    }
    return std::move(valid_moves);
}

Matrix<int> Gomoku::get_canonical(const Matrix<int>& state_matrix, int player_id){
    Matrix<int> canonical_state = Matrix<int>(state_matrix);
    for (auto it = state_matrix.begin(); it != state_matrix.end(); ++it){
        MatrixElement ME = *it;
        canonical_state.setValue(ME.getRow(), ME.getCol(), ME.getValue() * player_id);
    }
    return std::move(canonical_state);
}



GomokuTraining::GomokuTraining(int size, int win_condition_number, float win_reward, float tie_reward, float lose_reward, float no_reward)
    : Gomoku(size, win_condition_number), win_reward(win_reward), tie_reward(tie_reward), lose_reward(lose_reward), no_reward(no_reward){
        if (!(win_reward != no_reward && lose_reward != no_reward && tie_reward != no_reward)) {
            throw std::invalid_argument("Rewards must not be equal to no_reward. "
                                        "win_reward: " + std::to_string(win_reward) +
                                        ", tie_reward: " + std::to_string(tie_reward) +
                                        ", lose_reward: " + std::to_string(lose_reward) +
                                        ", no_reward: " + std::to_string(no_reward));
        }
    }

float GomokuTraining::get_reward(const Matrix<int>& state_matrix, int player_id){
    if (player_won(state_matrix, player_id)){
        return win_reward;
    }
    if (player_won(state_matrix, -player_id)){
        return lose_reward;
    }
    if (is_tie(state_matrix)){
        return tie_reward;
    }
    else{
        return no_reward;
    }
}

float GomokuTraining::get_no_reward(){
    return no_reward;
}