#pragma once

#include "AZero.h"
#include "LocalPlayer.h"
#include <thread>
#include <mutex>
#include <condition_variable>

extern const Uint32 AI_MOVE_EVENT;

class GameController{
public:
    GameController(){
        gameState = new GameState(15, 15, 1, 5); // IMPORTANT currently only works with fixed size: 20 x 20  this is because it needs to be the same file when recieved by server
        LocalPlayer* player1 = new LocalPlayer(1);
        LocalPlayer* player2 = new LocalPlayer(2);
        // AZero* player2 = new AZero(2, 100);
        players = std::vector<Player*>{player1, player2};
        currentPlayer = player1;


    }
    ~GameController(){ delete gameState; }

    const Matrix<int> getState() const {
        return gameState->getState();
    }

    void pieceSelectedInput(int i, int j){
        if (dynamic_cast<LocalPlayer*>(currentPlayer)){
            gameState->placePieceRequest(i, j, currentPlayerIndex);
            turnTaken();
            AITurn();
        }


    }

    // void serverInput(GameState* newGameState){
    //     if (dynamic_cast<RemotePlayer*>(currentPlayer)){
    //         gameState = newGameState;
    //     }
    //     turnTaken();
    //     AITurn();

    // }

    void AITurn() {
        if (dynamic_cast<AZero*>(currentPlayer)) {
            std::thread([this]() {
                Matrix<int> result = currentPlayer->takeTurn(gameState->getState());

                {
                    std::lock_guard<std::mutex> lock(aiMutex);
                }
                gameState->setState(result);
                turnTaken();


                SDL_Event event;
                SDL_zero(event);
                event.type = AI_MOVE_EVENT;
                SDL_PushEvent(&event); // Notify main thread to update visual
            }).detach();

        }
    }
    // AI winner doesnt work

    void turnTaken(){
        currentPlayerIndex = (currentPlayerIndex % 2) +1;
        currentPlayer = players[currentPlayerIndex-1];
        
        winner = currentPlayerIndex * gameState->checkIfWon(currentPlayerIndex);
    }

    int winner = 0;

private:
    std::mutex aiMutex;
    std::vector<Player*> players;
    Player* currentPlayer = nullptr;
    GameState* gameState;
    int currentPlayerIndex = 1;
};