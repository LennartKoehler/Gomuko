#include "../Game_Common/Game.hpp"
#include "SDL2/SDL.h"
#include <iostream>
#include "NetworkingClient.hpp"

Game* game = nullptr;

int main(int argc, const char* argv[]) {

    SteamNetworkingIPAddr addrServer = input_handling(argc, argv);
    ChatClient* client = run_client(addrServer);

    game = new Game();
    game->init("Gomuko", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 64*10, 64*10, false);
    SDL_Event event;





    while (game->running() && !g_bQuit){
        while (SDL_WaitEventTimeout(&event, 100)){
            game->handleEvents(event);
            game->update();
            game->render();

        }

        client->iteration();

    }


    game->clean(); 
    client->shutdown();
    
    return 0;
}
