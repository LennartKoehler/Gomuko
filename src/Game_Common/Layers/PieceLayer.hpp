#pragma once

#include "Layer.hpp"
#include "../Textures.hpp"
#include <map>

class MouseButtonPressed;

// turns gamestate into a image
class PieceLayer : public Layer{
public:
    PieceLayer();
    PieceLayer(GameState*& gameState, int entity_size, const char* asset_white, const char* asset_black);
    void onEvent(Event& event);
    bool onMouseButtonPressed( MouseButtonPressedEvent& event);
    void syncGameState();
    // void playerWon(PlayerComponent& playercomp);
    GameState*& gameState;

    void update() override{
        syncGameState();
        manager.update();}

private:
    Matrix<std::shared_ptr<Entity>> entityMatrix; // basically a lookup of entity pointers for gamestate
    std::map<int, const char*> asset_map{{0, ""}, {1, ""}, {2, ""}}; // map for gamestate integers an the assets

    int entity_size;
    enum groupLabels : std::size_t{
        groupTiles,
        groupPieces,
        groupPlayers
    };

};

// do i want a piecelayer to change the gamestate?