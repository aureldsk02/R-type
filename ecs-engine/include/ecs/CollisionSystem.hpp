/*
** EPITECH PROJECT, 2025
** G-CPP-500-COT-5-1-rtype-20
** File description:
** CollisionSystem
*/

#pragma once
#include "Registry.hpp"

class CollisionSystem
{
    public:
        std::vector<std::pair<Entity, Entity>> update(Registry& registry, float dt);
        void handleCollision(Registry& registry, Entity e1, Entity e2);

    private:
        bool checkCollision(float x1, float y1, float w1, float h1, float x2, float y2, float w2, float h2);
};
