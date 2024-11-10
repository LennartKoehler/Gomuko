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

    gameState = new GameState(20, 20, 1, 5); // IMPORTANT currently only works with fixed size: 20 x 20  this is because it needs to be the same file when recieved by server
    BackgroundLayer* backgroundLayer = new BackgroundLayer(gameState, 64, textures::tile);
    PieceLayer* pieceLayer = new PieceLayer(gameState, 64, textures::white_piece, textures::black_piece);
    layerStack.addLayer(backgroundLayer);
    layerStack.addLayer(pieceLayer);

}

void Game::handleEvents(SDL_Event& event){
    std::unique_ptr<Event> convertedEvent = eventConversionFactory.convertEvent( event );
    switch (event.type)
    {
    case SDL_QUIT:
        isRunning = false;
        break;
    case SDL_MOUSEBUTTONDOWN:

        layerStack.handleEvent(*convertedEvent);
    default:
        break;
    }
}

void Game::update(){
    layerStack.refreshLayers();
    layerStack.updateLayers();

}

void Game::render(){
    SDL_RenderClear(renderer);
    layerStack.drawLayers();
    SDL_RenderPresent(renderer);

}

void Game::clean(){
    delete gameState;
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
