#pragma once

#include "GameState.hpp"


// TODO coould use this format with the matrix as a vector as the normal gamestate, then i dont have any pointers and can easily send/recieve from server
// would need to change the get and set value to be compatible with the vector layout
class EncodedGameState{
public:
    EncodedGameState(GameState& gamestate) : player_at_turn(gamestate.player_at_turn),
        nRows(gamestate.stateMatrix.get_num_rows()),
        nCols(gamestate.stateMatrix.get_num_cols()),
        winner(gamestate.winner),
        turn_number(gamestate.turn_number),
        win_condition_number(gamestate.win_condition_number){
        for (int row = 0; row < gamestate.stateMatrix.get_num_rows(); row++){
                for (int col = 0; col < gamestate.stateMatrix.get_num_cols(); col++){
                    encodedMatrix.push_back(gamestate.getValue(row,col));
                }
        }
    }

private:
    int winner = 0;
    int turn_number;
    int win_condition_number;
    int nRows;
    int nCols;
    std::vector<int> encodedMatrix;
    int player_at_turn;
};


GameState* decodeGameState(EncodedGameState encgamestate){

}