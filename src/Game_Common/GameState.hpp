#pragma once
#include "VectorMatrix.hpp"
#include <vector>
#include <string>
#include <sstream>
// Simple representation of the game state e.g. for saving

struct GameState{
public:
    int winner = 0;
    Matrix<int> stateMatrix;
    int turn_number;
    int player_at_turn;
    int win_condition_number; // number of pieces in a row to win

    GameState() = default;
    GameState(int size_x, int size_y, int start_player, int win_condition_number);
    int getValue(int i, int j){ return stateMatrix.getValue(i, j); }
    void placePiece(int i, int j, int playeRID);
    bool checkIfWon(int playerID);
    std::string to_string();

};




