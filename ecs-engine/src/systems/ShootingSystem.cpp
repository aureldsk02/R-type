/*
** EPITECH PROJECT, 2025
** G-CPP-500-COT-5-1-rtype-20
** File description:
** ShootingSystem
*/

#include "ShootingSystem.hpp"
#include "Component.hpp"
#include <iostream>
#include <cmath>

void ShootingSystem::shoot(Registry& registry, Entity shooter)
{
    if (!registry.hasComponent<Position>(shooter) || !registry.hasComponent<Team>(shooter)) {
        return;
    }
    auto& pos = registry.getComponent<Position>(shooter);
    auto& team = registry.getComponent<Team>(shooter);
    Entity missile = registry.createEntity();
    float startX = pos.x + 40.0f;
    float startY = pos.y;

    registry.addComponent(missile, Position{startX, startY});
    registry.addComponent(missile, Velocity{20.0f, 0.0f});
    registry.addComponent(missile, Damage{9999});
    registry.addComponent(missile, CollisionBox{8.0f, 8.0f});
    registry.addComponent(missile, Team{team.ind});

    registry.bullets[shooter].push_back(missile);

    std::cout << "[SHOOT] Entity " << shooter << " fired missile " << missile << " at (" << startX << ", " << startY << ")" << std::endl;
}
