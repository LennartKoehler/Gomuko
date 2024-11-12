#pragma once
#include "Components.hpp"
#include <functional>

class FunctionComponent : public Component{
public:
    FunctionComponent(){}
    FunctionComponent(std::function<void()> func) : func(func){}
    void executeFunction(){
        func();
    }

private:
    std::function<void()> func;
};