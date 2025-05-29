#include "PieceLayer.hpp"
#include "../Events/MouseButtonPressedEvent.hpp"



PieceLayer::PieceLayer(const Matrix<int>& gameState, int entity_size, const char* asset_white, const char* asset_black)
    : entityMatrix(gameState.get_num_rows(), gameState.get_num_cols(), std::shared_ptr<Entity>()),
     entity_size(entity_size){
    asset_map[1] = asset_white;
    asset_map[2] = asset_black;


    for (int row = 0; row < gameState.get_num_rows(); row++){
        for (int col = 0; col < gameState.get_num_cols(); col++){
            Entity& piece(manager.addEntity());
            piece.addComponent<RectComponent>(row*entity_size, col*entity_size, entity_size, entity_size);
            piece.addComponent<MatrixPositionComponent>(row, col);
            piece.addComponent<SpriteComponent>();
            piece.addGroup(groupPieces);
            entityMatrix.setValue(row, col, std::shared_ptr<Entity> (&piece));
        }
    }
}


void PieceLayer::onEvent(Event& event){
    EventDispatcher dispatcher(event);
    dispatcher.dispatch<MouseButtonPressedEvent>(HZ_BIND_EVENT_FN(PieceLayer::onMouseButtonPressed));
}


bool PieceLayer::onMouseButtonPressed(MouseButtonPressedEvent& event){

    Entity* tile = getEntityAtPosition(event.x, event.y, groupPieces);
    if (tile != nullptr){

        MatrixPositionComponent& position = tile->getComponent<MatrixPositionComponent>();

        if (onPieceSelected) {
            onPieceSelected(position.i, position.j);
        }
    }
    return true;
}

void PieceLayer::syncGameState(const Matrix<int>& gameState){
    for (int row = 0; row < gameState.get_num_rows(); row++){
        for (int col = 0; col < gameState.get_num_cols(); col++){
            const int piece_int = gameState.getValue(row,col);
            std::shared_ptr<Entity> entity = entityMatrix.getValue(row, col);
    
            SpriteComponent& spritecomp = entity->getComponent<SpriteComponent>();
            
            spritecomp.setTexture(asset_map[piece_int]);
        }
    }
}



void PieceLayer::playerWon(int playerID){
    //pretty ugly
    Entity& won_entity = manager.addEntity();
    won_entity.addComponent<RectComponent>(200, 200, entity_size*2, entity_size*2);
    won_entity.addComponent<SpriteComponent>(textures::won);
    Entity& won_tile = manager.addEntity();
    won_tile.addComponent<RectComponent>(200, 200-2*entity_size, entity_size*2, entity_size*2);
    won_tile.addComponent<SpriteComponent>(asset_map[playerID]);
}

void PieceLayer::setPieceSelectedCallback(PieceSelectedCallback cb) {
    onPieceSelected = std::move(cb);
}