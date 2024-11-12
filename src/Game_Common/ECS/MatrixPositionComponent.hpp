#pragma once
#include "Components.hpp"

struct MatrixPositionComponent : public Component{
    int i;
    int j;

    MatrixPositionComponent(){
    }

    MatrixPositionComponent(int i, int j) : i(i), j(j){}

    void init() override{}
    void update() override{}
    void draw() override{}
};