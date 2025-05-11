#pragma once
#ifndef Game_hpp
#define Game_hpp

#include <iostream>
#include <SDL2/SDL.h>
#include <iostream>
#include <SDL2/SDL_image.h>
#include <set>
#include "Textures.hpp"
#include "Events/EventConversionFactory.hpp"
#include "Scenes/SceneManager.hpp"


//#include "Layers/LayerStack.hpp"


class Game {
public:
    Game();
    void init(const char* title, int xpos, int ypos, int width, int height, bool fullscreenn);

    void handleEvents(SDL_Event& event);
    void update();
    bool running() {return isRunning;};
    void render();
    void clean();
    static SDL_Renderer* renderer;
    static SDL_Event event;
    // void setGameState(GameState* gameState);
    // GameState* getGameState();
    bool isHandledEvent(SDL_Event& event);

    private:
    EventConversionFactory eventConversionFactory;
    std::set<Uint32> handledEvents{SDL_QUIT, SDL_MOUSEBUTTONDOWN, AI_MOVE_EVENT};
    // GameState* gameState;
    bool isRunning = false;
    SDL_Window* window;
    SceneManager* sceneManager;
};

#endif /* Game_hpp */