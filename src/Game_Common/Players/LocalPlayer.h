#pragma once

#include "Player.h"
#include <functional>


class LocalPlayer : public Player {
public:
    LocalPlayer(int id) : Player(id) {}

    // void takeTurn(GameState& gameState, int i, int j) override {
    //     this->moveCallback = onMove;
    //     // Show UI highlight, enable input, etc.
    // }
    void recieveInput(int i, int j){
        
    }

private:
    std::function<void(int, int)> moveCallback;
};