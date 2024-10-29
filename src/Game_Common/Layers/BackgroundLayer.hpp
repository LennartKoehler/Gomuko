#pragma once
#include "Layerinclude.hpp"


class BackgroundLayer : public Layer{
public:
    BackgroundLayer();
    BackgroundLayer(const GameState* gameState, float entity_size, const char* tile_asset);


private:
    Matrix<Entity*> entityMatrix;
    const char* tile_asset;
    int entity_size;
    enum groupLabels : std::size_t{
        groupTiles,
        groupPieces,
        groupPlayers
    };
};