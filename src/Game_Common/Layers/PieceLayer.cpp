#include "PieceLayer.hpp"
#include "../Events/MouseClickEvent.hpp"

PieceLayer::PieceLayer(){
}

PieceLayer::PieceLayer(GameState* gameState, int entity_size, const char* asset_white, const char* asset_black)
    : gameState(gameState),
     entityMatrix(gameState->stateMatrix.get_num_rows(), gameState->stateMatrix.get_num_cols(), std::shared_ptr<Entity>()),
     entity_size(entity_size){
    asset_map[1] = asset_white;
    asset_map[2] = asset_black;

    //init_players();

    for (int row = 0; row < gameState->stateMatrix.get_num_rows(); row++){
        for (int col = 0; col < gameState->stateMatrix.get_num_cols(); col++){
            Entity& piece(manager.addEntity());
            piece.addComponent<RectComponent>(row*entity_size, col*entity_size, entity_size, entity_size);
            piece.addComponent<MatrixPositionComponent>(row, col);
            piece.addComponent<SpriteComponent>();
            piece.addGroup(groupPieces);
            entityMatrix.setValue(row, col, std::shared_ptr<Entity> (&piece));
        }
    }
}


// void PieceLayer::init_players(){
//     p_player_white = &manager.addEntity();
//     p_player_white->addComponent<PlayerComponent>(1, "white", asset_white);
//     p_player_white->addGroup(groupPlayers);

//     p_player_black = &manager.addEntity();
//     p_player_black->addComponent<PlayerComponent>(2, "black", asset_black);
//     p_player_black->addGroup(groupPlayers);
// }


Entity* PieceLayer::getEntityAtPosition(int x, int y, Group group){
    for (Entity* entity : manager.getGroup(group)){
        bool found = entityOverlap(entity, x, y);
        if (found){
            return entity;
        }
    }
    return nullptr;
}



bool PieceLayer::entityOverlap(Entity* entity, int x, int y){
    RectComponent& tc = entity->getComponent<RectComponent>();
    if (x > tc.x && x < tc.x + tc.w
        && y > tc.y && y < tc.y + tc.h )
        return true;
    else return false;
}

// Entity* PieceLayer::getPlayerFromID(int playerID){
//     for (Entity* player : manager.getGroup(groupPlayers)){
//         PlayerComponent playercomp = player->getComponent<PlayerComponent>();
//         if (playercomp.playerID == playerID){
//             return player;
//         }
//     }
//     return nullptr;
// }


void PieceLayer::onEvent(MouseClickEvent event){
    Entity* tile = getEntityAtPosition(event.x, event.y, groupPieces);

    if (tile != nullptr){

        MatrixPositionComponent& position = tile->getComponent<MatrixPositionComponent>();
        gameState->placePiece(position.i, position.j, gameState->player_at_turn);
        syncGameState();
    }
}

void PieceLayer::syncGameState(){
    for (int row = 0; row < gameState->stateMatrix.get_num_rows(); row++){
        for (int col = 0; col < gameState->stateMatrix.get_num_cols(); col++){
            int piece_int = gameState->getValue(row,col);
            std::shared_ptr<Entity> entity = entityMatrix.getValue(row, col);
    
            SpriteComponent& spritecomp = entity->getComponent<SpriteComponent>();
            
            spritecomp.setTexture(asset_map[piece_int]);
        }
    }
}



void PieceLayer::playerWon(PlayerComponent& playercomp){
    //pretty ugly
    Entity& won_entity = manager.addEntity();
    won_entity.addComponent<RectComponent>(200, 200, entity_size*2, entity_size*2);
    won_entity.addComponent<SpriteComponent>(textures::won);
    Entity& won_tile = manager.addEntity();
    won_tile.addComponent<RectComponent>(200, 200-2*entity_size, entity_size*2, entity_size*2);
    won_tile.addComponent<SpriteComponent>(playercomp.playerPieceAsset);
}