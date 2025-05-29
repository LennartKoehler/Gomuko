#pragma once
#include "Event.hpp"
#include <SDL2/SDL.h>

class KeyDownEvent : public Event{
public:
    KeyDownEvent(SDL_Event& event)
        : key(event.key.keysym.sym){}
    EVENT_CLASS_TYPE(KeyDown);
    
public:
    SDL_Keycode key;
};

