#pragma once
#include <memory>
#include "Event.hpp"
#include "MouseButtonPressedEvent.hpp"
#include <SDL2/SDL.h>

class EventConversionFactory{ //convert SDL to my own events
public:
    static std::unique_ptr<Event> convertEvent( SDL_Event& event ){
        if( event.type == SDL_MOUSEBUTTONDOWN ){
            return std::make_unique<MouseButtonPressedEvent>( event );
        }
        return nullptr;
    }
};