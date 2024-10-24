#pragma once
#include "Layerinclude.hpp"


class Layer{
public:
    void draw(){
        manager.draw();
    }

    void update(){
        manager.update();
    }

    void refresh(){
        manager.refresh();
    }
protected:
    Manager manager;
};
