#pragma once
#include <SDL2/SDL.h>
#include <string>

extern const Uint32 TEXT_RECIEVED_EVENT;
extern const Uint32 SEND_TEXT_EVENT;

inline void sendChatMessageEvent(const std::string& msg){
    SDL_Event event;
    SDL_zero(event);
    event.type = TEXT_RECIEVED_EVENT;
    event.user.data1 = strdup(msg.c_str());
    SDL_PushEvent(&event);
}

inline void sendChatMessageNetworkEvent(const std::string& msg){
    SDL_Event event;
    SDL_zero(event);
    event.type = SEND_TEXT_EVENT;
    event.user.data1 = strdup(msg.c_str());
    SDL_PushEvent(&event);
}