/*
** EPITECH PROJECT, 2025
** scene_loader
** File description:
** load scene
*/
#include "Registry.hpp"
#include "Entity.hpp"
#include "Component.hpp"
#include "ScreenConfig.hpp"
#include "Scene_loader.hpp"
#include "SpawnSystem.hpp"
#include "MovementSystem.hpp"
#include "CollisionSystem.hpp"
#include "HealthSystem.hpp"
#include "DestroySystem.hpp"
#include "AiSystem.hpp"
#include "Component.hpp"
#include <random>
#include <iostream>
#include <algorithm>
#include <thread>
#include <chrono>

std::vector<Entity> SceneLoader::getPlayers(Registry& registry)
{
    std::vector<Entity> players;
    auto entities = registry.getEntitiesWith<Team>();

    for (Entity e : entities) {
        auto& team = registry.getComponent<Team>(e);
        if (team.ind == Team::Individu::PLAYER) {
            players.push_back(e);
        }
    }
    return players;
}

void SceneLoader::unloadScene(Registry& registry)
{
    std::vector<Entity> players = getPlayers(registry);
    auto entities = registry.getEntitiesWith<Position>();

    for (Entity e : entities) {
        bool isPlayer = false;
        for (Entity player : players) {
            if (e == player) {
                isPlayer = true;
                break;
            }
        }
        if (!isPlayer) {
            registry.destroyEntity(e);
        }
    }
}

void SceneLoader::loadScene(Registry& registry, const std::string& sceneName, SpawnSystem& spawn)
{
    // unloadScene(registry);
    std::cout << "[SCENE] Juju scene: " << sceneName << std::endl;
    if (sceneName == "firstScene") {
        std::cout << "Juju est là---------------------------------------------------: \n";
        loadFirstScene(registry, spawn);
    } else if (sceneName == "secondScene") {
        std::cout << "Juju est là---------------------------------------------------: \n";
        loadSecondScene(registry, spawn);
    } else {
        std::cout << "Juju est là---------------------------------------------------: \n";
        std::cout << "[SCENE] Unknown scene: " << sceneName << std::endl;
    }
}

void SceneLoader::loadFirstScene(Registry& registry, SpawnSystem& spawn)
{
    spawn.reset();
    spawn.setDifficulty(5.0f, 10, 20);
    //std::cout << "[SCENE] First scene loaded with difficulty settings." << std::endl;
    std::vector<Entity> players = getPlayers(registry);
    for (size_t i = 0; i < players.size(); i++) {
        Entity player = players[i];

        if (registry.hasComponent<Position>(player)) {
            auto& pos = registry.getComponent<Position>(player);
            pos.x = SCREEN_WIDTH * 0.1f;
            pos.y = (SCREEN_HEIGHT * 0.3f) + (i * 150.0f);
        }
        if (registry.hasComponent<Velocity>(player)) {
            auto& vel = registry.getComponent<Velocity>(player);
            vel.dx = 0.0f;
            vel.dy = 0.0f;
        }
        if (registry.hasComponent<Health>(player)) {
            auto& hp = registry.getComponent<Health>(player);
            hp.currentHP = hp.maxHP;
        }
    }
}

void SceneLoader::loadSecondScene(Registry& registry, SpawnSystem& spawn)
{
    spawn.reset();
    spawn.setDifficulty(3.0f, 15, 40);

    std::vector<Entity> players = getPlayers(registry);
    for (size_t i = 0; i < players.size(); i++) {
        Entity player = players[i];

        if (registry.hasComponent<Velocity>(player)) {
            auto& vel = registry.getComponent<Velocity>(player);
            vel.dx = 0.0f;
            vel.dy = 0.0f;
        }
    }
}
