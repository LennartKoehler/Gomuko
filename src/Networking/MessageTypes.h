#pragma once
#include <cstdint>

enum class MessageType : uint8_t {
    TEXT_MESSAGE = 1,
    GAME_STATE_UPDATE = 2,
    PLAYER_INPUT = 3,
    // add more as needed
};