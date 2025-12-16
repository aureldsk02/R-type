/*
** EPITECH PROJECT, 2025
** movement system
** File description:
** movement system source file 
*/

#include "MovementSystem.hpp"
#include "Component.hpp"

void MovementSystem::update(Registry& registry, float dt)
{
    auto entities = registry.getEntitiesWith<Position>();
    
    for (Entity e : entities) {
        if (registry.hasComponent<Velocity>(e)) {
            auto& pos = registry.getComponent<Position>(e);
            auto& vel = registry.getComponent<Velocity>(e);
            pos.x += vel.dx * dt;
            pos.y += vel.dy * dt;
        }
    }
}
