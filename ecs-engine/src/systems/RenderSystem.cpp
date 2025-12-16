/*
** EPITECH PROJECT, 2025
** render system
** File description:
** render system source file 
*/

#include "RenderSystem.hpp"
#include "Component.hpp"
#include <iostream>

void RenderSystem::update(Registry& registry, float dt)
{
    for (Entity e = 0; e < 1000; e++) {
        if (registry.hasComponent<Position>(e)) {
            auto& pos = registry.getComponent<Position>(e);
            std::cout << "Render Entity " << e << " at (" << pos.x << ", " << pos.y << ")\n";
        }
    }
}
