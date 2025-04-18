#include "Game.hpp"
#include "SDL2/SDL.h"
#include <iostream>
//#include "Networking.hpp"

Game* game = nullptr;

int main(int argc, const char* argv[]) {



    // const int FPS = 100;
    // const int frameDelay = 1000 / FPS;

    // Uint32 frameStart;
    // int frameTime;

    game = new Game();
    game->init("Gomuko", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 64*20, 64*20, false);
    SDL_Event event;
    
    game->update();
    game->render();

    while (game->running()){
        while (SDL_WaitEventTimeout(&event, 100)){
            if (game->isHandledEvent(event)){ //we dont want every event to be processed

                //frameStart = SDL_GetTicks();
                game->handleEvents(event);
                game->update();
                game->render();
            }
                
            // frameTime = SDL_GetTicks() - frameStart;

            // if (frameDelay > frameTime){
            //     SDL_Delay(frameDelay - frameTime);
            // }
        }
        

    }


    game->clean(); 
    
    return 0;
}

