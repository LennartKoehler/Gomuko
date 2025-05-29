#pragma once
#include "../TextureManager.hpp"
#include "Components.hpp"
#include <SDL2/SDL.h>

class TextFieldComponent : public Component{
private:
    bool takingInput = false;
    std::string text;
    TextComponent* textComp;
    FunctionComponent* funcComp;

public:
    TextFieldComponent() = default;
    TextFieldComponent(std::string text): text(text){}

    void init(){
        textComp = &entity->getComponent<TextComponent>(); 
        funcComp = &entity->getComponent<FunctionComponent>();

        funcComp->setFunction([this](){takingInput = !takingInput;});
        setTextTexture(text);
    }



    void setTextTexture(const std::string& newtext){
        textComp->setTexture(newtext);
    }

    std::string getText() const{
        return text;
    }

    void addLetter(std::string letter){
        if (takingInput){
            text.append(letter);
            setTextTexture(text);
        }
    }

    void keyInput(SDL_Keycode key){
        char symbol = static_cast<char>(key);
        if (takingInput){
            switch (key){
                case SDLK_BACKSPACE:
                    if (text.length() > 0){
                        text.pop_back();
                    }
                    break;

                // default: 
                //     text.append(&symbol);
                //     break;
                }
            setTextTexture(text);
        }
    }


};