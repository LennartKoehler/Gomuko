#include "Game.hpp"
#include "SDL2/SDL.h"
#include <iostream>
//#include "Networking.cpp"

Game* game = nullptr;

int test(int argc, const char* argv[]) {



    // const int FPS = 100;
    // const int frameDelay = 1000 / FPS;

    // Uint32 frameStart;
    // int frameTime;

    game = new Game();
    game->init("Gomuko", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 64*10, 64*10, false);
    SDL_Event event;


    while (game->running()){
        while (SDL_WaitEventTimeout(&event, 100)){
            //frameStart = SDL_GetTicks();
            game->handleEvents(event);
            game->update();
            game->render();

            
            //frameTime = SDL_GetTicks() - frameStart;

            // if (frameDelay > frameTime){
            //     SDL_Delay(frameDelay - frameTime);
            // }

        }
    }


    game->clean(); 
    
    return 0;
}

