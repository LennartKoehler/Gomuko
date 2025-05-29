#pragma once
#include "Player.h"
class RemotePlayer : public Player{
public:
    RemotePlayer(int id) : Player(id){}
};