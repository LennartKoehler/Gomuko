#pragma once

#include <vector>
#include <memory>
#include <array>
#include <bitset>
#include <algorithm>

class Component;
class Entity;
class Manager;

using ComponentID = std::size_t;
using Group = std::size_t;

inline ComponentID getNewComponentTypeID() {
    static ComponentID lastID = 0u;
    return lastID++;
}

template <typename T> inline ComponentID getComponentTypeID() noexcept {
    static ComponentID typeID = getNewComponentTypeID();
    return typeID;
}

constexpr std::size_t maxComponents = 32;
constexpr std::size_t maxGroups = 32;

using ComponentBitSet = std::bitset<maxComponents>;
using GroupBitSet = std::bitset<maxGroups>;
using ComponentArray = std::array<Component*, maxComponents>;

class Component {
public:
    Entity* entity;
    virtual void init() {}
    virtual void update() {}
    virtual void draw() {}
    virtual ~Component() {}
};

class Entity {
private:
    Manager& manager;
    bool active = true;
    std::vector<std::unique_ptr<Component>> components;
    ComponentArray componentArray;
    ComponentBitSet componentBitSet;
    GroupBitSet groupBitSet;

public:
    Entity(Manager& manager);
    void update();
    void draw();
    bool isActive() const;
    void destroy();
    void addGroup(Group mGroup);
    bool hasGroup(Group mGroup);
    void delGroup(Group mGroup);


    template <typename T>
    bool hasComponent() const{
        return componentBitSet[getComponentTypeID<T>()];
    }

    template <typename T, typename... TArgs>
    T& addComponent(TArgs&&... mArgs){
            
        T* c = new T(std::forward<TArgs>(mArgs)...);
        c->entity = this;
        std::unique_ptr<Component> uPtr{ c };

        components.emplace_back(std::move(uPtr));
        componentArray[getComponentTypeID<T>()] = c;
        componentBitSet[getComponentTypeID<T>()] = true;

        c->init();
        return *c;
    }

    template<typename T>
    T& getComponent() const{
        auto ptr(componentArray[getComponentTypeID<T>()]);
        return *static_cast<T*>(ptr);
    }
};

class Manager {
private:
    std::vector<std::shared_ptr<Entity>> m_entities; // PROBLEM: SHOULD BE UNIQUE-PTR
    std::array<std::vector<Entity*>, maxGroups> groupedEntities;

public:
    int number_entities() { return m_entities.size(); }
    void update();
    void draw();
    void refresh();
    Entity& addEntity();
    void addToGroup(Entity* entity, Group group);
    std::vector<Entity*>& getGroup(Group mGroup);
};

