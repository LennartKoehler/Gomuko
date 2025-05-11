#pragma once
#include "Scene.hpp"
#include "../Players/Player.h"
#include "../GameState.hpp"
#include "../Players/GameController.h"

class SceneManager;

class GomokuScene : public Scene{
public:
    GomokuScene(SceneManager* sceneManager);
    ~GomokuScene();

    void onPieceSelected(int i, int j);
    void update() override;

    PieceLayer* pieceLayer;
    BackgroundLayer* backgroundLayer;
private:
    std::vector<Player> players;
    GameController* gameController;
};