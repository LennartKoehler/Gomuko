#pragma once

#include "../ECS/Components.hpp"
#include "../GameState.hpp"
#include "../Events/MouseButtonPressedEvent.hpp"
class Layer{
public:
    virtual void draw(){
        manager.draw();
    }

    virtual void update(){
        manager.update();
    }

    virtual void refresh(){
        manager.refresh();
    }

    virtual void onEvent(Event& event) {}

    
    Entity* getEntityAtPosition(int x, int y, size_t group);
    bool entityOverlap(Entity* entity, int x, int y);
protected:
    Manager manager;
};
