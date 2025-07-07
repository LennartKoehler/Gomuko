#include "Game_Client/Game.hpp"
#include <SDL2/SDL.h>
#include <iostream>
#include <SDL2/SDL_ttf.h>

Game* game = nullptr;
const Uint32 AI_MOVE_EVENT = SDL_RegisterEvents(1);
const Uint32 GAMESTATE_RECIEVED_EVENT = SDL_RegisterEvents(1);
const Uint32 TEXT_RECIEVED_EVENT = SDL_RegisterEvents(1);
const Uint32 NETWORK_CONNECTION_EVENT = SDL_RegisterEvents(1);
const Uint32 SEND_GAMESTATE_EVENT = SDL_RegisterEvents(1);
const Uint32 SEND_TEXT_EVENT = SDL_RegisterEvents(1);

int main(int argc, const char* argv[]) {

    game = new Game();
    game->init("Gomoku", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 64*20, 64*20, false);
    SDL_Event event;
    
    game->update();
    game->render();

    while (game->running()){
        while (SDL_WaitEventTimeout(&event, 10)){
            if (game->isHandledEvent(event)){ //we dont want every event to be processed

                game->handleEvents(event);
                game->update();
                game->render();

            }
            game->handleNetwork();
        }
    }


    game->clean(); 
    
    return 0;
}

