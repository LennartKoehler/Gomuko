#include "Game.hpp"
#include "../Networking/NetworkingClient.h"

SDL_Renderer* Game::renderer = nullptr;
SDL_Event Game::event;
    



Game::Game(){
}


void Game::init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen){
    TTF_Init();
    SDL_StartTextInput();

    int flags = 0;
    if (fullscreen){
        flags = SDL_WINDOW_FULLSCREEN;
    }
    
    if(SDL_Init(SDL_INIT_EVERYTHING) == 0){
        std::cout << "Subsystem initialized" << std::endl;

        window = SDL_CreateWindow(title, xpos, ypos, width, height, flags);
        if (window){
            std::cout << "Window created" << std::endl;
        }
        

        renderer = SDL_CreateRenderer(window, -1, 0);
        if (renderer){
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            std::cout << "Renderer created" << std::endl;
        }

        isRunning = true;
    }
    sceneManager = new SceneManager();
    sceneManager->setActiveScene(sceneManager->mainMenu);

}

//TODO handleEvents can be adjusted to fit the event dispatcher system
void Game::handleEvents(SDL_Event& event){
    std::unique_ptr<Event> convertedEvent = EventConversion::convertEvent( event ); // idk why they are unique ptr

    if (event.type == AI_MOVE_EVENT) {
        update();
        render();
    }
    if (event.type == GAMESTATE_RECIEVED_EVENT) {
        sceneManager->getActiveScene()->handleEvent(*convertedEvent);
    }
    if (event.type == SEND_GAMESTATE_EVENT) {
        GameStatePlayerEvent* gsEvent = dynamic_cast<GameStatePlayerEvent*>(convertedEvent.get());
        sendGameStateToServer(*gsEvent);
    }
    if (event.type == NETWORK_CONNECTION_EVENT) {
        NetworkConnectionEvent* netEvent = dynamic_cast<NetworkConnectionEvent*>(convertedEvent.get());
        setClient(netEvent->IPAddress);
    }
    if (event.type == TEXT_RECIEVED_EVENT) {
        TextEvent* netEvent = dynamic_cast<TextEvent*>(convertedEvent.get());
        sceneManager->getActiveScene()->handleEvent(*convertedEvent);
    }
    if (event.type == SEND_TEXT_EVENT) {
        TextEvent* textEvent = dynamic_cast<TextEvent*>(convertedEvent.get());
        sendTextToServer(*textEvent);
    }

    switch (event.type)
    {
    case SDL_QUIT:
        isRunning = false;
        break;

    case SDL_MOUSEBUTTONDOWN:
        sceneManager->getActiveScene()->handleEvent(*convertedEvent);
        break;

    case SDL_KEYDOWN:
        sceneManager->getActiveScene()->handleEvent(*convertedEvent);
        break;

    case SDL_TEXTINPUT:
        sceneManager->getActiveScene()->handleEvent(*convertedEvent);
        break;

    default:
        break;
    }

}

void Game::update(){
    sceneManager->getActiveScene()->refresh();
    sceneManager->getActiveScene()->update();
}

void Game::render(){
    SDL_RenderClear(renderer);
    sceneManager->getActiveScene()->draw();
    SDL_RenderPresent(renderer);

}

void Game::clean(){
    // delete gameState;
    delete sceneManager;
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
    std::cout << "Game cleaned" << std::endl;
}



// void Game::setGameState(GameState* gameState){
//     this->gameState = gameState;
// }

// GameState* Game::getGameState() { return gameState;}

bool Game::isHandledEvent(SDL_Event& event){
    bool found = (std::find(handledEvents.begin(), handledEvents.end(), event.type) != handledEvents.end());
    return found;
}
void Game::handleNetwork(){
    if (client != nullptr){
        client->iteration(); // check for new data from the server
    }
}

void Game::sendGameStateToServer(GameStatePlayerEvent& event){
    if (client != nullptr){
        std::vector<uint8_t> data = Serializer::serializeGameState(event.gameState, event.playerID);
        Package package{MessageType::GAME_STATE_UPDATE, data};
        sendPackageToServer(package);
    }
}

void Game::sendTextToServer(TextEvent& event){
    if (client != nullptr){
        std::vector<uint8_t> data = Serializer::serializeText(std::string(event.text));
        Package package{MessageType::TEXT_MESSAGE, data};
        sendPackageToServer(package);
    }
}

void Game::sendPackageToServer(Package package){
    client->sendToServer(package);
}

void Game::setClient(const char* serverAddress){
    SteamNetworkingIPAddr IPaddr;
    if (!get_address(serverAddress, IPaddr)){
        std::cout << "Problemski connecting to Server IP Address: " << serverAddress << std::endl;
    }
    else {client = run_client(IPaddr);}

}