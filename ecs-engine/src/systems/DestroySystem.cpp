/*
** EPITECH PROJECT, 2025
** G-CPP-500-COT-5-1-rtype-20
** File description:
** DestroySystem
*/

#include "DestroySystem.hpp"
#include "Component.hpp"
#include "ScreenConfig.hpp"
#include <iostream>

void DestroySystem::update(Registry& registry, float dt)
{
    destroyInvisibleEntities(registry);
    auto healthers = registry.getEntitiesWith<Health>();
    
    for (Entity e : healthers) {
        auto& health = registry.getComponent<Health>(e);
        
        if (health.currentHP == 0) {
            registry.addComponent(e, DestroyTag{});
        }
    }

    auto entities = registry.getEntitiesWith<DestroyTag>();
    
    for (Entity e : entities) {
        registry.whoDiedNigga.push_back(e);
        std::cout << "Destroying entity " << e << std::endl;
        registry.destroyEntity(e);
    }
}

void DestroySystem::destroyInvisibleEntities(Registry& registry)
{
    auto entities = registry.getEntitiesWith<Position>();

    for (Entity e : entities) {
        auto& pos = registry.getComponent<Position>(e);
        bool isOffScreen = (pos.x < -SCREEN_MARGIN || pos.x > SCREEN_WIDTH + SCREEN_MARGIN || 
                           pos.y < -SCREEN_MARGIN || pos.y > SCREEN_HEIGHT + SCREEN_MARGIN);
        if (isOffScreen) {
            if (registry.hasComponent<Damage>(e) && !registry.hasComponent<Ai>(e)) {
                std::cout << "[DESTROY] Entity " << e << " went off-screen at (" << pos.x << ", " << pos.y << ")" << std::endl;
                registry.addComponent(e, DestroyTag{});
            }
        }
    }
}
