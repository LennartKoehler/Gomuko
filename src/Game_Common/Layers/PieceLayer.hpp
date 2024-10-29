#pragma once

#include "Layerinclude.hpp"
#include "../Textures.hpp"


class MouseClickEvent;

class PieceLayer : public Layer{
public:
    PieceLayer();
    PieceLayer(GameState* gameState, int entity_size, const char* asset_white, const char* asset_black);
    //~PieceLayer();

    // void update() override {
    //     syncGameState();
    //     manager.update();
    // }

    Entity* getEntityAtPosition(int x, int y, size_t group);
    bool entityOverlap(Entity* entity, int x, int y);
    void onEvent(MouseClickEvent event);
    void init_players();
    Entity* getPlayerFromID(int playerID);
    void playerWon(PlayerComponent& playercomp);
    void syncGameState();




private:
    Matrix<std::shared_ptr<Entity>> entityMatrix; // basically a lookup of entity pointers for gamestate
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