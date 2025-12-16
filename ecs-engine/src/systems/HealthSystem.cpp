/*
** EPITECH PROJECT, 2025
** G-CPP-500-COT-5-1-rtype-20
** File description:
** HealthSystem
*/

#include "HealthSystem.hpp"
#include "Component.hpp"
#include <iostream>

void HealthSystem::update(Registry& registry, float dt) {
    auto entities = registry.getEntitiesWith<Health>();
    
    for (Entity e : entities) {
        auto& health = registry.getComponent<Health>(e);

        if (health.currentHP <= 0) {
            std::cout << "Entity " << e << " died!\n";
            registry.addComponent(e, DestroyTag{});
        }
        if (health.currentHP > health.maxHP) {
            health.currentHP = health.maxHP;
        }
    }
}