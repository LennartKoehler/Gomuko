#pragma once
#include "VectorMatrix.hpp"
#include <vector>
#include <string>
#include <sstream>
#include <iostream>
// Simple representation of the game state e.g. for saving, this is what is sent/recieved from server

struct GameState{
    int ownerID; // only used on server
    int winner = 0;
    Matrix<int> stateMatrix;
    int turn_number;
    bool unchangable = false;
    int player_at_turn;
    int win_condition_number; // number of pieces in a row/col/diag to win

    GameState() = default;
    GameState(int size_x, int size_y, int start_player, int win_condition_number);
    //GameState(GameState& other){ std::cerr << "copying gamestate" << std::endl; }

    int getValue(int i, int j){ return stateMatrix.getValue(i, j); }
    void placePieceRequest(int i, int j);
    void placePiece(int i, int j);
    bool checkIfWon(int playerID);
    void nextPlayer();
    std::string to_string();

};




