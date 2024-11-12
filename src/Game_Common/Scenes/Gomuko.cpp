#include "Gomuko.hpp"


Gomuko::Gomuko(SceneManager* sceneManager) : Scene(sceneManager){
        gameState = new GameState(20, 20, 1, 5); // IMPORTANT currently only works with fixed size: 20 x 20  this is because it needs to be the same file when recieved by server
        BackgroundLayer* backgroundLayer = new BackgroundLayer(gameState, 64, textures::tile);
        PieceLayer* pieceLayer = new PieceLayer(gameState, 64, textures::white_piece, textures::black_piece);
        layerStack.addLayer(backgroundLayer);
        layerStack.addLayer(pieceLayer);
    }
Gomuko::~Gomuko(){ delete gameState; }