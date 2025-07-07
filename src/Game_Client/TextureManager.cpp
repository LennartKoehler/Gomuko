#include "TextureManager.hpp"
#include "Game.hpp"


SDL_Texture* TextureManager::LoadTexture(const char* texture){
    SDL_Surface* tempSurface = IMG_Load(texture);
    SDL_Texture* tex = SDL_CreateTextureFromSurface(Game::renderer, tempSurface);
    SDL_FreeSurface(tempSurface);
    return tex;
}

SDL_Texture* TextureManager::WriteText(const char* text, int fontsize){
    TTF_Font* Sans = TTF_OpenFont("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf", fontsize);
    SDL_Color black = {0, 0, 0};
    SDL_Surface* surfaceMessage = TTF_RenderText_Blended(Sans, text, black);
    SDL_Texture* Message = SDL_CreateTextureFromSurface(Game::renderer, surfaceMessage);
    SDL_FreeSurface(surfaceMessage);
    TTF_CloseFont(Sans);
    return Message;
}

SDL_Texture* TextureManager::WriteTextWrapped(const char* text, int fontsize, int wrapLength){
    TTF_Font* Sans = TTF_OpenFont("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf", fontsize);
    SDL_Color black = {0, 0, 0};
    SDL_Surface* surfaceMessage = TTF_RenderText_Blended_Wrapped(Sans, text, black, wrapLength);
    SDL_Texture* Message = SDL_CreateTextureFromSurface(Game::renderer, surfaceMessage);
    SDL_FreeSurface(surfaceMessage);
    TTF_CloseFont(Sans);
    return Message;
}

// SDL_Texture* TextureManager::WriteText(const char* text){
//     SDL_Rect dest = {10,20,20,20};
//     SDL_Rect src = {10,20,10,20};

//     SDL_Surface* tempSurface = IMG_Load(textures::background);

//     TTF_Font* Sans = TTF_OpenFont("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf", 24);
//     SDL_Color black = {0, 0, 0};
//     SDL_Surface* surfaceMessage = TTF_RenderText_Blended(Sans, text, black);
//     SDL_BlitSurface( surfaceMessage, NULL, tempSurface, &dest );

//     SDL_Texture* Message = SDL_CreateTextureFromSurface(Game::renderer, surfaceMessage);
//     SDL_FreeSurface(surfaceMessage);
//     return Message;
// }

SDL_Texture* TextureManager::WriteTextOnBackground(const char* text, SDL_Surface* background){
    SDL_Rect dest = {10,20,0,0};
    SDL_Surface* tempSurface = IMG_Load(textures::background);

    TTF_Font* Sans = TTF_OpenFont("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf", 24);
    SDL_Color black = {0, 0, 0};
    SDL_Surface* surfaceMessage = TTF_RenderText_Blended(Sans, text, black);
    SDL_BlitSurface( surfaceMessage, NULL, background, &dest );

    SDL_Texture* Message = SDL_CreateTextureFromSurface(Game::renderer, surfaceMessage);
    SDL_FreeSurface(surfaceMessage);
    return Message;
}

SDL_Texture* TextureManager::WriteText(std::string text, int fontsize){
    return TextureManager::WriteText(text.c_str(), fontsize);
}

SDL_Texture* TextureManager::WriteTextWrapped(std::string text, int fontsize, int wrapLength){
    return TextureManager::WriteTextWrapped(text.c_str(), fontsize, wrapLength);
}

void TextureManager::Draw(SDL_Texture* tex, SDL_Rect src, SDL_Rect dest){
    SDL_RenderCopy(Game::renderer, tex, NULL, &dest); // insert src rectangle instead of NULL
}
