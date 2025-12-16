/*
** EPITECH PROJECT, 2025
** G-CPP-500-COT-5-1-rtype-20
** File description:
** AiSystem
*/

#include "AiSystem.hpp"
#include "Component.hpp"
#include "ScreenConfig.hpp"
#include <iostream>
#include <cmath>

void AiSystem::update(Registry& registry, float dt)
{
    auto aiEntities = registry.getEntitiesWith<Ai>();

    for (Entity e : aiEntities) {
        if (!registry.hasComponent<Position>(e) || 
            !registry.hasComponent<Velocity>(e) ||
            !registry.hasComponent<Team>(e)) {
            continue;
        }
        auto& ai = registry.getComponent<Ai>(e);
        auto& team = registry.getComponent<Team>(e);

        if (team.ind != Team::Individu::ENEMY) {
            continue;
        }
        ai.timer += dt;
        ai.stateTimer += dt;
        ai.shootCooldown -= dt;

        switch (ai.type) {
            case AiType::BASIC_ENEMY:
                BasicEnemy(registry, e, dt);
                break;
            case AiType::SHOOTING_ENEMY:
                ShootingEnemy(registry, e, dt);
                break;
            case AiType::FOLLOWING_ENEMY:
                FollowingEnemy(registry, e, dt);
                break;
        }
        auto& pos = registry.getComponent<Position>(e);
        if (isOutOfBounds(pos.x, pos.y)) {
            registry.addComponent(e, DestroyTag{});
        }
    }
}

void AiSystem::BasicEnemy(Registry& registry, Entity e, float dt)
{
    auto& vel = registry.getComponent<Velocity>(e);
    auto& ai = registry.getComponent<Ai>(e);

    vel.dx = -3.0f;
    vel.dy = 0.0f;

    if (ai.timer > 0.1f) {
        vel.dy = std::sin(ai.stateTimer * 2.0f) * 2.0f;
    }
}

void AiSystem::ShootingEnemy(Registry& registry, Entity e, float dt)
{
    auto& vel = registry.getComponent<Velocity>(e);
    auto& ai = registry.getComponent<Ai>(e);
    auto& pos = registry.getComponent<Position>(e);

    vel.dx = -3.0f;
    vel.dy = std::sin(ai.stateTimer * 1.5f) * 3.0f;

    if (ai.shootCooldown <= 0.0f && pos.x < SCREEN_WIDTH * 0.6f) {
        shoot(registry, e, -1.0f, 0.0f);
        ai.shootCooldown = 200.0f;
    }
}

void AiSystem::FollowingEnemy(Registry& registry, Entity e, float dt)
{
    auto& vel = registry.getComponent<Velocity>(e);
    auto& pos = registry.getComponent<Position>(e);
    auto& ai = registry.getComponent<Ai>(e);

    Entity p = findPlayer(registry, pos.x, pos.y);
    
    if (p != static_cast<Entity>(-1)) {
        auto& playerPos = registry.getComponent<Position>(p);

        float dx = playerPos.x - pos.x;
        float dy = playerPos.y - pos.y;
        float distance = std::sqrt(dx * dx + dy * dy);

        if (distance > 10.0f) {
            vel.dx = (dx / distance) * 3.0f;
            vel.dy = (dy / distance) * 3.0f;

            if (distance < 300.0f && ai.shootCooldown <= 0.0f) {
                shoot(registry, e, dx / distance, dy / distance);
                ai.shootCooldown = 200.0f;
            }
        } else {
            vel.dx = 0.0f;
            vel.dy = 0.0f;
        }
    } else {
        vel.dx = -3.0f;
        vel.dy = 0.0f;
    }
}

Entity AiSystem::findPlayer(Registry& registry, float x, float y)
{
    auto entities = registry.getEntitiesWith<Team>();
    Entity p = static_cast<Entity>(-1);
    float minDistance = 999999.0f;

    for (Entity e : entities) {
        auto& team = registry.getComponent<Team>(e);
        if (team.ind == Team::Individu::PLAYER && registry.hasComponent<Position>(e)) {
            auto& pos = registry.getComponent<Position>(e);
            float dx = pos.x - x;
            float dy = pos.y - y;
            float dist = dx * dx + dy * dy;

            if (dist < minDistance) {
                minDistance = dist;
                p = e;
            }
        }
    }
    return p;
}

void AiSystem::shoot(Registry& registry, Entity shooter, float dirX, float dirY)
{
    auto& pos = registry.getComponent<Position>(shooter);

    Entity missile = registry.createEntity();
    registry.addComponent(missile, Position{pos.x - 10.0f, pos.y});
    registry.addComponent(missile, Velocity{dirX * 15.0f, dirY * 15.0f});
    registry.addComponent(missile, Damage{10});
    registry.addComponent(missile, CollisionBox{8.0f, 8.0f});
    registry.addComponent(missile, Team{Team::Individu::ENEMY});

    registry.bullets[shooter].push_back(missile);
    std::cout << "[SHOOT] Enemy " << shooter << " fired missile!" << std::endl;
}

bool AiSystem::isOutOfBounds(float x, float y)
{
    return (x < -SCREEN_MARGIN || x > SCREEN_WIDTH + SCREEN_MARGIN || y < -SCREEN_MARGIN || y > SCREEN_HEIGHT + SCREEN_MARGIN);
}
