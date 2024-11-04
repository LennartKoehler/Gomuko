#include "GameState.hpp"


GameState::GameState(int size_x, int size_y, int start_player, int win_condition_number)
    : stateMatrix(Matrix<int>(size_x, size_y, 0)), player_at_turn(start_player), turn_number(0), win_condition_number(win_condition_number){
}

void GameState::placePieceRequest(int i, int j){
    if (getValue(i, j) == 0 && !unchangable){ // check if viable move
        placePiece(i, j);
    }
}

void GameState::placePiece(int i, int j){
    stateMatrix.setValue(i, j, player_at_turn);
    if (checkIfWon(player_at_turn)){
        winner = player_at_turn;
    }
    else{ nextPlayer(); }
    turn_number++;
}


bool GameState::checkIfWon(int playerID) {
    int n_rows = stateMatrix.get_num_rows();
    int n_cols = stateMatrix.get_num_cols();

    // not sure if i got cols and rows mixed up somewhere here
    // Check rows and columns
    for (int i = 0; i < n_rows; ++i) {
        int rowCount = 0, colCount = 0;
        for (int j = 0; j < n_cols; ++j) {
            // Check row
            if (getValue(i,j) == playerID) {
                colCount++;
                if (colCount >= win_condition_number) return true;
            } else {
                colCount = 0;
            }

            // Check column
            if (getValue(j,i) == playerID) {
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
                if (getValue(i,j) == playerID) {
                    mainDiagonalCount++;
                    if (mainDiagonalCount >= win_condition_number) return true;
                } else {
                    mainDiagonalCount = 0;
                }
            }

            // Check anti-diagonal
            int antiJ = n_rows - 1 - i + d;
            if (antiJ >= 0 && antiJ < n_rows) {
                if (getValue(i,antiJ) == playerID) {
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

std::string GameState::to_string(){
        std::stringstream ss;
        ss << "player_at_turn: " << player_at_turn << ", turn_number: " << turn_number;
    return ss.str();
}

void GameState::nextPlayer(){
    player_at_turn = (player_at_turn % 2) + 1;
}