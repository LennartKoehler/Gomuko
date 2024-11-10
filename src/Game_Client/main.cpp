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

    int recieved_turn_number = 0;
    bool sent = false;
    game->getGameState()->unchangable = true; // unchangable is what tells us whos at turn


    game->handleEvents(event); // wait for events
    game->update();
    game->render();
    while (game->running() && !g_bQuit){
        while (SDL_WaitEventTimeout(&event, 100)){
            if (game->isHandledEvent(event)){ //we dont want every event to be processed
                game->handleEvents(event); // wait for events
                game->update();
                game->render();
            }
        }
        if (game->getGameState()->turn_number != recieved_turn_number && !sent){
            std::cerr << "sending gamestate" << std::endl;
            game->getGameState()->unchangable = true; // unchangable is what tells us whos at turn
            client->sendGameState(game->getGameState());
            sent = true;
        }
        client->iteration(); // wait for update from server
        if (client->newGameState){
            std::cerr << "reading gamestate" << std::endl;
            delete game->getGameState();
            game->setGameState(client->getGameState());
            recieved_turn_number = game->getGameState()->turn_number;

            std::cerr << "read gamestate" << game->getGameState() << std::endl;
            game->update();
            game->render();
            sent = false;
        }
    }

    client->shutdown();
    game->clean(); 
    
    return 0;
}


