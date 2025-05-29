#pragma once

#include "AZero.h"
#include "LocalPlayer.h"
#include "RemotePlayer.h"
#include <thread>
#include <mutex>
#include <condition_variable>
#include "../Events/GameStateEvent.hpp"


extern const Uint32 AI_MOVE_EVENT;
extern const Uint32 SEND_GAMESTATE_EVENT;
extern const Uint32 GAMESTATE_RECIEVED_EVENT;



class GameController{
public:
    GameController(){
        gameState = new GameState(15, 15, 1, 5); // IMPORTANT currently only works with fixed size: 20 x 20  this is because it needs to be the same file when recieved by server
        LocalPlayer* player1 = new LocalPlayer(1);
        RemotePlayer* player2 = new RemotePlayer(2);
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
            Matrix<int>* newGameState = new Matrix<int>{gameState->placePieceRequest(i, j, currentPlayerIndex)};
            pushGameStateEvent(newGameState, currentPlayerIndex);
        }


    }

    void AITurn() {
        if (dynamic_cast<AZero*>(currentPlayer)) {
            std::thread([this]() {
                Matrix<int>* result = new Matrix<int>(currentPlayer->takeTurn(gameState->getState()));

                {
                    std::lock_guard<std::mutex> lock(aiMutex);
                }
                pushGameStateEvent(result, currentPlayerIndex);
            }).detach();

        }
    }


    void pushGameStateEvent(Matrix<int>* stateMatrix, int playerID){
        SDL_Event event; // TODO delete matrix and playerid later
        SDL_zero(event);
        event.user.data1 = stateMatrix;
        int* playerID_Heap = new int(playerID);
        event.user.data2 = playerID_Heap;
        event.type = GAMESTATE_RECIEVED_EVENT;
        SDL_PushEvent(&event);
    }

    void sendGameStateEventToServer(Matrix<int> stateMatrix, int playerID){
        Matrix<int>* stateMatrix_Heap = new Matrix<int>(stateMatrix); // TODO delete later
        SDL_Event event;
        SDL_zero(event);
        event.user.data1 = stateMatrix_Heap;
        int* playerID_Heap = new int(playerID);
        event.user.data2 = playerID_Heap;
        event.type = SEND_GAMESTATE_EVENT;
        SDL_PushEvent(&event);
    }

    void turnTaken(){
        currentPlayerIndex = (currentPlayerIndex % 2) +1;
        currentPlayer = players[currentPlayerIndex-1];
        winner = currentPlayerIndex * gameState->checkIfWon(currentPlayerIndex);
        
        if (dynamic_cast<RemotePlayer*>(currentPlayer)){
            sendGameStateEventToServer(gameState->getState(), currentPlayerIndex);
        }
    }

    void handleEvent(Event& event){ // TODO maybe process all the moves through events
        EventDispatcher dispatcher(event);
        dispatcher.dispatch<GameStatePlayerEvent>(HZ_BIND_EVENT_FN(GameController::onGameStateEvent));
    }

    bool onGameStateEvent(GameStatePlayerEvent& event){
        if (event.playerID == currentPlayerIndex){
            gameState->setState(event.gameState);
            turnTaken();
        }
        return true;
    }


    int winner = 0;
private:

    std::mutex aiMutex;
    std::vector<Player*> players;
    Player* currentPlayer = nullptr;
    GameState* gameState;
    int currentPlayerIndex = 1;
};