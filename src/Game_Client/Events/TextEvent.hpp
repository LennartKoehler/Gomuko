#pragma once
#include "Event.hpp"


class TextEvent : public Event{
public:
    TextEvent(SDL_Event& event)
        : text(static_cast<const char*>(event.user.data1)){}
    EVENT_CLASS_TYPE(Text);
    
public:
    const char* text;
};

