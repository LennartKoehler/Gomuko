#pragma once
#include "Components.hpp"
#include <functional>

class FunctionComponent : public Component{
public:
    FunctionComponent(){}
    FunctionComponent(std::function<void()> func){
        setFunction(func);
    }
    void setFunction(std::function<void()> func){
        this->func = func;
    }
    void executeFunction(){
        func();
    }

protected:
    std::function<void()> func;
};