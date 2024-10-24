#include "Event.hpp"
namespace GE{
    class MouseClickEvent : public GE::Event{
    public:
        MouseClickEvent(const SDL_Event& event)
            : x(event.button.x), y(event.button.y){}
            
    public:
        int x;
        int y;
    };
}