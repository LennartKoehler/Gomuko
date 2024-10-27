#pragma once

#include "Event.hpp"
#include <vector>
#include <memory>

class EventQueue{
public:
    void put(SDL_Event event);

    template <typename T>
    T& get(){

    }

private:
    std::vector<Event> events;
    

};
