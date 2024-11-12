#include "BackgroundLayer.hpp"

BackgroundLayer::BackgroundLayer(){
}

BackgroundLayer::BackgroundLayer(GameState*& gameState, float entity_size, const char* tile_asset)
    : entity_size(entity_size), tile_asset(tile_asset){
    for (int row = 0; row < gameState->stateMatrix.get_num_rows(); row++){
        for (int col = 0; col < gameState->stateMatrix.get_num_cols(); col++){
            Entity& tile(manager.addEntity());
            tile.addComponent<RectComponent>(row*entity_size, col*entity_size, entity_size, entity_size);
            tile.addComponent<SpriteComponent>(tile_asset);
        }
    }
}