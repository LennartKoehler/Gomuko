#include "GomokuScene.hpp"


GomokuScene::GomokuScene(SceneManager* sceneManager) : Scene(sceneManager){
        gameController = new GameController();
        backgroundLayer = new BackgroundLayer(gameController->getState(), 64, textures::tile);
        pieceLayer = new PieceLayer(gameController->getState(), 64, textures::white_piece, textures::black_piece);
        layerStack.addLayer(backgroundLayer);
        layerStack.addLayer(pieceLayer);

        pieceLayer->setPieceSelectedCallback([this](int i, int j) {
            onPieceSelected(i, j);
        });
    }

void GomokuScene::onPieceSelected(int i, int j){
    gameController->pieceSelectedInput(i,j);
}




void GomokuScene::update(){
    pieceLayer->syncGameState(gameController->getState());
    if (gameController->winner != 0){
        pieceLayer->playerWon(gameController->winner);
    }
    layerStack.updateLayers();
    
}

// void GomokuScene::turn(){

//     players
// }

GomokuScene::~GomokuScene(){
    delete gameController;
    delete backgroundLayer;
    delete pieceLayer;
}