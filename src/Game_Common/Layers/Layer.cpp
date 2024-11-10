#include "Layer.hpp"

Entity* Layer::getEntityAtPosition(int x, int y, Group group){
    for (Entity* entity : manager.getGroup(group)){
        bool found = entityOverlap(entity, x, y);
        if (found){
            return entity;
        }
    }
    return nullptr;
}



bool Layer::entityOverlap(Entity* entity, int x, int y){
    RectComponent& tc = entity->getComponent<RectComponent>();
    if (x > tc.x && x < tc.x + tc.w
        && y > tc.y && y < tc.y + tc.h )
        return true;
    else return false;
}