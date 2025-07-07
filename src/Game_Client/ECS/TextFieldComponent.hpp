#pragma once
#include "../TextureManager.hpp"
#include "Components.hpp"
#include <SDL2/SDL.h>

class TextFieldComponent : public Component{
private:
    static TextFieldComponent* focusedField;
    std::string text;
    std::string defaultText;
    RectComponent* rectComp;
    SDL_Rect srcRect, destRect;
    FunctionComponent* funcComp;
    SDL_Texture* textTexture;
    SDL_Texture* backgroundTexture;
    int fontsize = 28;
    std::unordered_map<SDL_Keycode, std::function<void()>> keyActions;

public:
    TextFieldComponent() = default;
    TextFieldComponent(std::string text, const char* backgroundPath, int fontsize): fontsize(fontsize), defaultText(text){
        setTextTexture(text);
        setBackgroundTexture(backgroundPath);
    }

    void init(){
        rectComp = &entity->getComponent<RectComponent>(); 
        funcComp = &entity->getComponent<FunctionComponent>();
        srcRect.x = srcRect.y = 0;
        srcRect.w = srcRect.h = 50;
        funcComp->setFunction(std::bind(&TextFieldComponent::setOnClick, this));
        addAction(SDLK_BACKSPACE, std::bind(&TextFieldComponent::setBackspace, this));
        addAction(SDLK_RETURN, std::bind(&TextFieldComponent::setReturn, this));
    }

    void update() override{
        destRect.x = (int)rectComp->x;
        destRect.y = (int)rectComp->y;
    }


    void draw() override{
        // TextureManager::Draw(backgroundTexture, srcRect, destRect);
        TextureManager::Draw(textTexture, srcRect, destRect);
    }


    void setBackgroundTexture(const char* path){
        backgroundTexture = TextureManager::LoadTexture(path);
    }

    void setTextTexture(const std::string& newtext){
        textTexture = TextureManager::WriteText(newtext, fontsize);
        SDL_QueryTexture(textTexture, NULL, NULL, &destRect.w, &destRect.h);
    }

    std::string getText() const{
        return text;
    }

    void addLetter(std::string letter){
        text.append(letter);
        setTextTexture(text);
    }

    void reset(){
        text.clear();
        text = defaultText;
        setTextTexture(text);
    }

    void clear(){
        text.clear();
        setTextTexture(text);
    }


    void setBackspace(){
        if (text.length() > 0){
            text.pop_back();
        }
    }

    void setReturn(){
        clear();
    }

    void setOnClick(){
        if (focusedField != nullptr){
            focusedField->reset();
        }
        focusedField = this;
        clear();
    }
    void addAction(SDL_Keycode key, std::function<void()> func){
        keyActions[key] = std::move(func);
    }


    void keyInput(SDL_Keycode key){
        auto it = keyActions.find(key);
        if (it != keyActions.end()) {
            it->second(); // Call the registered lambda
            setTextTexture(this->text);
        }
    }

    std::string getText(){
        return text;
    }

    static TextFieldComponent* getFocusedField() {
        return focusedField;
    }


    bool isFocused() const {
        return focusedField == this;
    }

};