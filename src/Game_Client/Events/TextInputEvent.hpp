#pragma once
#include "Event.hpp"

#include <string>

class TextInputEvent : public Event{
public:
    TextInputEvent(SDL_Event& event)
        : text(event.text.text) {}
    EVENT_CLASS_TYPE(TextInput);
    
public:
    std::string text;
};

