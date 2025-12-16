/*
** EPITECH PROJECT, 2025
** G-CPP-500-COT-5-1-rtype-20
** File description:
** DestroySystem
*/

#pragma once
#include "Registry.hpp"

class DestroySystem
{
    public:
        void update(Registry& registry, float dt);
    private:
        void destroyInvisibleEntities(Registry& registry);
};
