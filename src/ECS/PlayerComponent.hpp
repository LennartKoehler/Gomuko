#include "Components.hpp"

class PlayerComponent : public Component{
public:
    int playerID;
    const char* playerName;
    const char* playerPieceAsset;
    
public:
    PlayerComponent(int playerID, const char* playerName)
        : playerID(playerID), playerName(playerName){}

    PlayerComponent(int playerID, const char* playerName, const char* playerPieceAsset)
        : playerID(playerID), playerName(playerName), playerPieceAsset(playerPieceAsset){}




    void init(){}
    void update() override{}
    void draw() override{}
};