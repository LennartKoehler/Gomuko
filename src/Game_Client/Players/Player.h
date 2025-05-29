#pragma once

#include "../GameState.hpp"


class Player{
public:
    Player(int playerID) : playerID(playerID){}
    virtual Matrix<int> takeTurn(const Matrix<int>& stateMatrix){ return Matrix<int>(stateMatrix); }


protected:
    int playerID;
    // GameController* gameController;
};