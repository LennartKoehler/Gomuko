#include "PieceLayer.hpp"
#include "../Events/MouseClickEvent.hpp"

PieceLayer::PieceLayer(){
}

PieceLayer::PieceLayer(GameState* gameState, int entity_size, const char* asset_white, const char* asset_black)
    : gameState(gameState), entity_size(entity_size), asset_white(asset_white), asset_black(asset_black){
    init_players();

    for (int row = 0; row < gameState->stateMatrix.get_num_rows(); row++){
        for (int col = 0; col < gameState->stateMatrix.get_num_cols(); col++){
            Entity& piece(manager.addEntity());
            piece.addComponent<RectComponent>(row*entity_size, col*entity_size, entity_size, entity_size);
            piece.addComponent<MatrixPositionComponent>(row, col);
            piece.addGroup(groupPieces);
        }
    }
}


void PieceLayer::init_players(){
    p_player_white = &manager.addEntity();
    p_player_white->addComponent<PlayerComponent>(1, "white", asset_white);
    p_player_white->addGroup(groupPlayers);

    p_player_black = &manager.addEntity();
    p_player_black->addComponent<PlayerComponent>(2, "black", asset_black);
    p_player_black->addGroup(groupPlayers);
}


Entity* PieceLayer::getEntityAtPosition(int x, int y, Group group){
    for (Entity* entity : manager.getGroup(group)){ // this doesnt return any entity
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

Entity* PieceLayer::getPlayerFromID(int playerID){
    for (Entity* player : manager.getGroup(groupPlayers)){
        PlayerComponent playercomp = player->getComponent<PlayerComponent>();
        if (playercomp.playerID == playerID){
            return player;
        }
    }
    return nullptr;
}


void PieceLayer::onEvent(GE::MouseClickEvent event){
    Entity* player_at_turn = getPlayerFromID(gameState->player_at_turn);
    Entity* tile = getEntityAtPosition(event.x, event.y, groupPieces);

    if (tile != nullptr){

        MatrixPositionComponent& position = tile->getComponent<MatrixPositionComponent>();

        if (gameState->getValue(position.i, position.j) == 0){

            gameState->placePiece(position.i, position.j);
            PlayerComponent playercomp = player_at_turn->getComponent<PlayerComponent>();
            tile->addComponent<SpriteComponent>(playercomp.playerPieceAsset);
            
            if (gameState->checkIfWon(gameState->player_at_turn)){
                playerWon(playercomp);
            }
            else{ gameState->player_at_turn = (gameState->player_at_turn % 2) + 1;}
        }
    }
}

void PieceLayer::playerWon(PlayerComponent& playercomp){
    //pretty ugly
    Entity& won_entity = manager.addEntity();
    won_entity.addComponent<RectComponent>(200, 200, entity_size*2, entity_size*2);
    won_entity.addComponent<SpriteComponent>("../assets/won.png");
    Entity& won_tile = manager.addEntity();
    won_tile.addComponent<RectComponent>(200, 200-2*entity_size, entity_size*2, entity_size*2);
    won_tile.addComponent<SpriteComponent>(playercomp.playerPieceAsset);
}