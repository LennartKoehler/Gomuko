#include "ECS.hpp"

void Entity::addGroup(Group mGroup){
    groupBitSet[mGroup] = true;
    manager.addToGroup(this, mGroup);
}


Entity::Entity(Manager& manager) : manager(manager) {}

void Entity::update() {
    for (auto& c : components) c->update();
}

void Entity::draw() {
    for (auto& c : components) c->draw();
}

bool Entity::isActive() const {
    return active;
}

void Entity::destroy() {
    active = false;
}

bool Entity::hasGroup(Group mGroup){
    return groupBitSet[mGroup];
}

void Entity::delGroup(Group mGroup){
    groupBitSet[mGroup] = false;
}




///////////////////////////////


void Manager::update() {
    for (auto& e : m_entities) e->update();
}

void Manager::draw() {
    for (auto& e : m_entities) e->draw();
}

void Manager::refresh(){
    for(auto i(0u); i < maxGroups; i++){
        auto& v(groupedEntities[i]);
        v.erase(
            std::remove_if(std::begin(v), std::end(v),
            [i](Entity* mEntity)
            {
                return !mEntity->isActive() || !mEntity->hasGroup(i);
            }),
            std::end(v));
    }

    m_entities.erase(std::remove_if(std::begin(m_entities), std::end(m_entities),
    [](const std::shared_ptr<Entity> &mEntity){
        return !mEntity->isActive();
    }),
    std::end(m_entities));
}


void Manager::addToGroup(Entity* mEntity, Group mGroup){
    groupedEntities[mGroup].emplace_back(mEntity);
}

std::vector<Entity*>& Manager::getGroup(Group mGroup){
    return groupedEntities[mGroup];
}

Entity& Manager::addEntity(){
    Entity* e = new Entity(*this);
    std::shared_ptr<Entity> uPtr { e };
    m_entities.emplace_back(std::move(uPtr));
    return *e;
}