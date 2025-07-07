#pragma once

#include <SDL2/SDL.h>

#include "Package.h"

extern const Uint32 GAMESTATE_RECIEVED_EVENT;
extern const Uint32 TEXT_RECIEVED_EVENT;



inline void packageToSDLEvent(Package* package){
    SDL_Event event;
    SDL_zero(event);
    if (package->type == MessageType::TEXT_MESSAGE){
        event.type = TEXT_RECIEVED_EVENT;
        // Allocate on heap so the pointer remains valid after this scope
        auto textData = Serializer::deserializeText(package->payload);
        event.user.data1 = strdup(textData.c_str()); // TODO delete the data later
        SDL_PushEvent(&event);

    }
    if (package->type == MessageType::GAME_STATE_UPDATE){
        event.type = GAMESTATE_RECIEVED_EVENT;
        // Allocate on heap so the pointer remains valid after this scope
        auto tuple = Serializer::deserializeGameState(package->payload);
        auto* gameState = new Matrix<int>(std::get<0>(tuple));
        auto* playerID = new int(std::get<1>(tuple));
        event.user.data1 = gameState;
        event.user.data2 = playerID;
        SDL_PushEvent(&event);
    }
    delete package;
}



inline Package eventToPackage(GameStatePlayerEvent& event){
    std::vector<uint8_t> buffer = Serializer::serializeGameState(event.gameState, event.playerID);
    Package package{MessageType::GAME_STATE_UPDATE, buffer};
    return package;
}