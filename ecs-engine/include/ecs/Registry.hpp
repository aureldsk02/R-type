/*
** EPITECH PROJECT, 2025
** registry
** File description:
** registry
*/

#pragma once
#include "Entity.hpp"
#include <map>
#include <unordered_map>
#include <memory>
#include <typeindex>
#include <vector>

class Registry
{
    public:
        Entity createEntity();
        void destroyEntity(Entity e);

        template<typename T>
        void addComponent(Entity e, T component);

        template<typename T>
        T& getComponent(Entity e);

        template<typename T>
        bool hasComponent(Entity e);

        template<typename T>
        void removeComponent(Entity e);

        template<typename T>
        std::vector<Entity> getEntitiesWith();
        std::map<Entity, std::vector<Entity>> bullets;
        std::vector<Entity> whoDiedNigga;

    private:
        Entity nextEntity = 0;
        std::unordered_map<std::type_index, std::unordered_map<Entity, std::shared_ptr<void>>> components;
};

inline Entity Registry::createEntity() {
    return nextEntity++;
}

inline void Registry::destroyEntity(Entity e) {
    for (auto& [type, entityMap] : components) {
        entityMap.erase(e);
    }
}

template<typename T>
void Registry::addComponent(Entity e, T component) {
    components[typeid(T)][e] = std::make_shared<T>(component);
}

template<typename T>
T& Registry::getComponent(Entity e) {
    return *std::static_pointer_cast<T>(components[typeid(T)][e]);
}

template<typename T>
bool Registry::hasComponent(Entity e) {
    return components[typeid(T)].count(e) > 0;
}

template<typename T>
void Registry::removeComponent(Entity e) {
    auto a = components.find(typeid(T));
    if (a != components.end()) {
        a->second.erase(e);
    }
}

template<typename T>
std::vector<Entity> Registry::getEntitiesWith() {
    std::vector<Entity> result;
    auto a = components.find(typeid(T));
    if (a == components.end()) {
        return result;
    }
    for (auto& [entity, b] : a->second) {
        result.push_back(entity);
    }
    return result;
}
