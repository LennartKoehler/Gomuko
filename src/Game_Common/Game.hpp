#pragma once
#ifndef Game_hpp
#define Game_hpp

#include <iostream>
#include <SDL2/SDL.h>
#include <iostream>
#include <SDL2/SDL_image.h>
#include "Textures.hpp"
#include "Events/MouseClickEvent.hpp"
#include "Layers/Layerinclude.hpp"


class Game {
public:
    Game();
    void init(const char* title, int xpos, int ypos, int width, int height, bool fullscreenn);

    void handleEvents(SDL_Event& event);
    void update();
    bool running() {return isRunning;};
    void render();
    void clean();
    void onClick(SDL_Event& event, PieceLayer* layer);
    
    static SDL_Renderer* renderer;
    static SDL_Event event;

private:
    bool isRunning = false;
    SDL_Window* window;
    GameState* gameState;
    BackgroundLayer* backgroundLayer;
    PieceLayer* pieceLayer;
};

#endif /* Game_hpp */