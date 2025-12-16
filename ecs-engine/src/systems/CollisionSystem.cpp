/*
** EPITECH PROJECT, 2025
** G-CPP-500-COT-5-1-rtype-20
** File description:
** CollisionSystem
*/

#include "CollisionSystem.hpp"
#include "Component.hpp"
#include <iostream>

bool CollisionSystem::checkCollision(float x1, float y1, float w1, float h1, float x2, float y2, float w2, float h2)
{
    return (x1 < x2 + w2 && x1 + w1 > x2 && y1 < y2 + h2 && y1 + h1 > y2);
}

std::vector<std::pair<Entity, Entity>> CollisionSystem::update(Registry& registry, float dt)
{
    std::vector<std::pair<Entity, Entity>> collisions;
    auto entities = registry.getEntitiesWith<Position>();

    for (size_t i = 0; i < entities.size(); ++i) {
        Entity e1 = entities[i];
        if (!registry.hasComponent<CollisionBox>(e1) || 
            !registry.hasComponent<Team>(e1)) {
            continue;
        }
        auto& pos1 = registry.getComponent<Position>(e1);
        auto& box1 = registry.getComponent<CollisionBox>(e1);
        auto& team1 = registry.getComponent<Team>(e1);
        for (size_t j = i + 1; j < entities.size(); ++j) {
            Entity e2 = entities[j];
            if (!registry.hasComponent<CollisionBox>(e2) || 
                !registry.hasComponent<Team>(e2)) {
                continue;
            }
            auto& pos2 = registry.getComponent<Position>(e2);
            auto& box2 = registry.getComponent<CollisionBox>(e2);
            auto& team2 = registry.getComponent<Team>(e2);
            if (team1.ind == team2.ind) {
                continue;
            }
            if (checkCollision(
                pos1.x + box1.offset_x, pos1.y + box1.offset_y, box1.width, box1.height,
                pos2.x + box2.offset_x, pos2.y + box2.offset_y, box2.width, box2.height)) {
                collisions.push_back({e1, e2});
                handleCollision(registry, e1, e2);
            }
        }
    }
    return collisions;
}

void CollisionSystem::handleCollision(Registry& registry, Entity e1, Entity e2)
{
    bool e1_has_damage = registry.hasComponent<Damage>(e1);
    bool e2_has_damage = registry.hasComponent<Damage>(e2);
    bool e1_has_health = registry.hasComponent<Health>(e1);
    bool e2_has_health = registry.hasComponent<Health>(e2);

    if (e1_has_damage && e2_has_health) {
        auto& health = registry.getComponent<Health>(e2);
        health.currentHP = 0;
        registry.addComponent(e1, DestroyTag{});
        std::cout << "[COLLISION] Entity " << e1 << " killed entity " << e2 << " instantly!" << std::endl;
    }
    if (e2_has_damage && e1_has_health) {
        auto& health = registry.getComponent<Health>(e1);
        health.currentHP = 0;
        registry.addComponent(e2, DestroyTag{});
        std::cout << "[COLLISION] Entity " << e2 << " killed entity " << e1 << " instantly!" << std::endl;
    }
    if (!e1_has_damage && !e2_has_damage && e1_has_health && e2_has_health) {
        auto& health1 = registry.getComponent<Health>(e1);
        auto& health2 = registry.getComponent<Health>(e2);
        health1.currentHP = 0;
        health2.currentHP = 0;
        std::cout << "[COLLISION] Physical collision killed both entity " << e1 << " and entity " << e2 << std::endl;
    }
}
