#pragma once
#include "Event.hpp"
#include "../VectorMatrix.hpp"



class GameStateEvent : public Event{
public:
    GameStateEvent(SDL_Event& event)
        : gameState(*static_cast<Matrix<int>*>(event.user.data1)){}
    EVENT_CLASS_TYPE(GameState);
    
public:
    Matrix<int> gameState;
};


class GameStatePlayerEvent : public GameStateEvent{
public:
    GameStatePlayerEvent(SDL_Event& event)
        : GameStateEvent(event), playerID(*static_cast<int*>(event.user.data2)){}
    EVENT_CLASS_TYPE(GameStatePlayer);
    
public:
    int playerID;
};