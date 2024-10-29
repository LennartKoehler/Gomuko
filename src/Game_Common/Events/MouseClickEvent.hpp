#include "Event.hpp"
class MouseClickEvent : public Event{
public:
    MouseClickEvent(const SDL_Event& event)
        : x(event.button.x), y(event.button.y){}
        
public:
    int x;
    int y;
};
