#pragma once
#ifndef Game_hpp
#define Game_hpp

#include <iostream>
#include <SDL2/SDL.h>
#include <iostream>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <set>
#include "Textures.hpp"
#include "Events/EventConversion.hpp"
#include "Scenes/SceneManager.hpp"

//#include "Layers/LayerStack.hpp"

extern const Uint32 GAMESTATE_RECIEVED_EVENT;
extern const Uint32 TEXT_RECIEVED_EVENT;
extern const Uint32 NETWORK_CONNECTION_EVENT;
extern const Uint32 SEND_GAMESTATE_EVENT;

class ChatClient;
class Package;

class Game {
public:
    Game();
    void init(const char* title, int xpos, int ypos, int width, int height, bool fullscreenn);

    void handleEvents(SDL_Event& event);
    void update();
    bool running() {return isRunning;};
    void render();
    void clean();
    void handleNetwork();
    void sendPackageToServer(Package package);
    void sendGameStateToServer(GameStatePlayerEvent& event);
    void setClient(const char* address);
    static SDL_Renderer* renderer;
    static SDL_Event event;
    // void setGameState(GameState* gameState);
    // GameState* getGameState();
    bool isHandledEvent(SDL_Event& event);

    private:
    std::set<Uint32> handledEvents{SDL_QUIT,
         SDL_MOUSEBUTTONDOWN,
         SDL_KEYDOWN,
         SDL_TEXTINPUT,
         AI_MOVE_EVENT,
         GAMESTATE_RECIEVED_EVENT,
         TEXT_RECIEVED_EVENT,
         NETWORK_CONNECTION_EVENT,
         SEND_GAMESTATE_EVENT};
    // GameState* gameState;
    bool isRunning = false;
    SDL_Window* window;
    SceneManager* sceneManager;
    ChatClient* client = nullptr;
};

#endif /* Game_hpp */