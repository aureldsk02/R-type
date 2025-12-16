/*
** EPITECH PROJECT, 2025
** G-CPP-500-COT-5-1-rtype-20
** File description:
** AiSystem
*/

#pragma once
#include "Registry.hpp"
#include "Entity.hpp"
#include "System.hpp"

class AiSystem
{
    public:
        void update(Registry& registry, float dt);

    private:
        void BasicEnemy(Registry& registry, Entity e, float dt);
        void ShootingEnemy(Registry& registry, Entity e, float dt);
        void FollowingEnemy(Registry& registry, Entity e, float dt);

        Entity findPlayer(Registry& registry, float x, float y);
        void shoot(Registry& registry, Entity shooter, float dirX, float dirY);
        bool isOutOfBounds(float x, float y);
};

