#pragma once
#include "../Layers/LayerStack.hpp"

class SceneManager;

class Scene{
public:
    Scene(SceneManager* sceneManager) : sceneManager(sceneManager){
    }
    void refresh(){
        layerStack.refreshLayers();
    }
    virtual void update(){
        layerStack.updateLayers();
    }
    void draw(){
        layerStack.drawLayers();
    }
    void handleEvent(Event& event){
        layerStack.handleEvent(event);
    }

    bool getStatus(){
        return isActive;
    }

protected:
    bool isActive = false;
    LayerStack layerStack;
    SceneManager* sceneManager;
};