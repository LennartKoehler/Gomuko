#pragma once
#include "Event.hpp"


class MouseButtonPressedEvent : public Event{
public:
    MouseButtonPressedEvent(SDL_Event& event)
        : x(event.button.x), y(event.button.y){}
    EVENT_CLASS_TYPE(MouseButtonPressed);
    
public:
    int x;
    int y;
};
