/*
** EPITECH PROJECT, 2025
** G-CPP-500-COT-5-1-rtype-20
** File description:
** SpawnSystem
*/

#pragma once
#include "Registry.hpp"
#include "Entity.hpp"
#include "Component.hpp"

class SpawnSystem
{
    public:
        void update(Registry &registry, float dt);
        void reset();
        void setDifficulty(float interval, int maxEnemies, int totalEnemies);
        int getEnemiesSpawned() const { return m_enemiesSpawned; }
        bool isLevelComplete() const { return m_enemiesSpawned >= m_maxTotalEnemies; }
        const char* getEnemyTypeName(AiType type);
    private:
        void spawnEnemy(Registry &registry, AiType type, float x, float y);
        // void spawnPowerUp(Registry &registry, float x, float y);

        float m_spawnTimer = 0.0f;
        float m_spawnInterval = 3.0f;
        int m_enemiesSpawned = 0;
        int m_maxEnemies = 15;
        int m_maxTotalEnemies = 100;
};
