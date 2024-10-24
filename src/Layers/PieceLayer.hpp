#pragma once

#include "Layerinclude.hpp"

namespace GE{
    class MouseClickEvent;
}

class PieceLayer : public Layer{
public:
    PieceLayer();
    PieceLayer(GameState* gameState, int entity_size, const char* asset_white, const char* asset_black);
    //~PieceLayer();

    Entity* getEntityAtPosition(int x, int y, size_t group);
    bool entityOverlap(Entity* entity, int x, int y);
    void onEvent(GE::MouseClickEvent event);
    void init_players();
    Entity* getPlayerFromID(int playerID);
    void playerWon(PlayerComponent& playercomp);



private:
    Entity* p_player_white;
    Entity* p_player_black;
    const char* asset_white;
    const char* asset_black;
    int entity_size;
    GameState* gameState;
    enum groupLabels : std::size_t{
        groupTiles,
        groupPieces,
        groupPlayers
    };
};

// do i want a piecelayer to change the gamestate?