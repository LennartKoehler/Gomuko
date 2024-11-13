#include "Game.hpp"

SDL_Renderer* Game::renderer = nullptr;
SDL_Event Game::event;
    



Game::Game(){
}


void Game::init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen){

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
    std::unique_ptr<Event> convertedEvent = eventConversionFactory.convertEvent( event );
    switch (event.type)
    {
    case SDL_QUIT:
        isRunning = false;
        break;
    case SDL_MOUSEBUTTONDOWN:

        sceneManager->getActiveScene()->handleEvent(*convertedEvent);
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
    delete gameState;
    delete sceneManager;
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
    std::cout << "Game cleaned" << std::endl;
}



void Game::setGameState(GameState* gameState){
    this->gameState = gameState;
}

GameState* Game::getGameState() { return gameState;}

bool Game::isHandledEvent(SDL_Event& event){
    bool found = (std::find(handledEvents.begin(), handledEvents.end(), event.type) != handledEvents.end());
    return found;
}
