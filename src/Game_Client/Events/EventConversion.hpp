#pragma once
#include <memory>
#include "Event.hpp"
#include "MouseButtonPressedEvent.hpp"
#include "GameStateEvent.hpp"
#include "NetworkConnectionEvent.hpp"
#include "TextInputEvent.hpp"
#include "KeyDownEvent.hpp"
#include <SDL2/SDL.h>

extern const Uint32 GAMESTATE_RECIEVED_EVENT;
extern const Uint32 NETWORK_CONNECTION_EVENT;
extern const Uint32 SEND_GAMESTATE_EVENT;

namespace EventConversion{ //convert SDL to my own events, idk why they are now unique ptr
    static std::unique_ptr<Event> convertEvent( SDL_Event& event ){
        if( event.type == SDL_MOUSEBUTTONDOWN ){
            return std::make_unique<MouseButtonPressedEvent>( event );
        }
        if( event.type == SDL_KEYDOWN ){
            return std::make_unique<KeyDownEvent>( event );
        }
        if( event.type == SDL_TEXTINPUT ){
            return std::make_unique<TextInputEvent>( event );
        }
        if( event.type == GAMESTATE_RECIEVED_EVENT){
            return std::make_unique<GameStatePlayerEvent>( event );
        }
        if( event.type == NETWORK_CONNECTION_EVENT){
            return std::make_unique<NetworkConnectionEvent>( event );
        }
        if( event.type == SEND_GAMESTATE_EVENT){
            return std::make_unique<GameStatePlayerEvent>( event );
        }
        return nullptr;
    }


};