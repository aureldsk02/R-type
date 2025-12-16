/*
** EPITECH PROJECT, 2025
** scene_loader
** File description:
** load scene
*/
#pragma once
#include "Registry.hpp"
#include "Entity.hpp"
#include "Component.hpp"
#include "SpawnSystem.hpp"

class SceneLoader
{
    public:
        void loadScene(Registry& registry, const std::string& sceneName, SpawnSystem& system);
        void unloadScene(Registry& registry);

    private:
        std::vector<Entity> getPlayers(Registry& registry);
        void loadFirstScene(Registry& registry, SpawnSystem& system);
        void loadSecondScene(Registry& registry, SpawnSystem& system);
};
