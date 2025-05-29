#pragma once
#include "Event.hpp"

class NetworkConnectionEvent : public Event{
public:
    NetworkConnectionEvent(SDL_Event& event)
        : IPAddress(static_cast<const char*>(event.user.data1)){}
    EVENT_CLASS_TYPE(NetworkConnection);
    
public:
    const char* IPAddress;
};

