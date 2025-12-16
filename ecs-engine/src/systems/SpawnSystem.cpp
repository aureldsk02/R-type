/*
** EPITECH PROJECT, 2025
** G-CPP-500-COT-5-1-rtype-20
** File description:
** SpawnSystem
*/

#include "SpawnSystem.hpp"
#include "Component.hpp"
#include "ScreenConfig.hpp"
#include <random>
#include <iostream>
#include <algorithm>

void SpawnSystem::update(Registry &registry, float dt)
{
    int aliveEnemies = 0;
    auto entities = registry.getEntitiesWith<Team>();
    for (Entity e : entities) {
        auto& team = registry.getComponent<Team>(e);
        if (team.ind == Team::Individu::ENEMY) {
            aliveEnemies++;
        }
    }
    m_spawnTimer += dt;
    if (m_spawnTimer >= m_spawnInterval && aliveEnemies < m_maxEnemies && m_enemiesSpawned < m_maxTotalEnemies) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<float> yDist(50.0f, SCREEN_HEIGHT - 50.0f);
        std::uniform_int_distribution<int> typeDist(0, 2);
        std::uniform_int_distribution<int> countDist(1, 5);

        m_spawnTimer = 0.0f;
        int spawnCount = std::min(countDist(gen), m_maxEnemies - aliveEnemies);

        for (int i = 0; i < spawnCount; i++) {
            AiType type = static_cast<AiType>(typeDist(gen));
            float y = yDist(gen);
            float x = SCREEN_WIDTH - 70.0f + (i * 50.0f);
            spawnEnemy(registry, type, x, y);
            m_enemiesSpawned++;
        }
        if (m_enemiesSpawned % 20 == 0 && m_spawnInterval > 0.5f) {
            m_spawnInterval -= 0.2f;
            std::cout << "Difficulty increased! Spawn interval: " << m_spawnInterval << "s\n";
        }
    }
    if (m_enemiesSpawned >= m_maxTotalEnemies && aliveEnemies == 0) {
        std::cout << "[SPAWN] ✓ Level complete! All enemies defeated." << std::endl;
    }
}

void SpawnSystem::spawnEnemy(Registry &registry, AiType type, float x, float y)
{
    Entity enemy = registry.createEntity();

    registry.addComponent(enemy, Position{x, y});
    registry.addComponent(enemy, Velocity{0.0f, 0.0f});
    registry.addComponent(enemy, Team{Team::Individu::ENEMY});

    switch (type) {
        case AiType::BASIC_ENEMY:
            registry.addComponent(enemy, Health{20, 20});
            registry.addComponent(enemy, CollisionBox{32.0f, 32.0f});
            registry.addComponent(enemy, Ai{
                type,
                AiState::MOVING_LEFT,
                0.0f,
                0.0f,
                99999.0f,
                y
            });
            break;
            
        case AiType::SHOOTING_ENEMY:
            registry.addComponent(enemy, Health{30, 30});
            registry.addComponent(enemy, CollisionBox{36.0f, 36.0f});
            registry.addComponent(enemy, Ai{
                type,
                AiState::MOVING_LEFT,
                0.0f,
                0.0f,
                200.0f,
                y
            });
            break;
            
        case AiType::FOLLOWING_ENEMY:
            registry.addComponent(enemy, Health{40, 40});
            registry.addComponent(enemy, CollisionBox{40.0f, 40.0f});
            registry.addComponent(enemy, Ai{
                type,
                AiState::FOLLOWING_PLAYER,
                0.0f,
                0.0f,
                200.0f,
                y
            });
            break;
    }    
    std::cout << "Spawned " << getEnemyTypeName(type) << " (ID: " << enemy << ") at (" << x << ", " << y << ")\n";
}

/* void SpawnSystem::spawnPowerUp(Registry &registry, float x, float y)
{
    Entity powerUp = registry.createEntity();

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> bonusDist(10, 50);
    
    int bonusValue = bonusDist(gen);
    
    registry.addComponent(powerUp, Position{x, y});
    registry.addComponent(powerUp, Velocity{-80.0f, 0.0f});
    registry.addComponent(powerUp, PowerUp{bonusValue});
    registry.addComponent(powerUp, CollisionBox{24.0f, 24.0f});
    registry.addComponent(powerUp, Team{Team::Individu::NEUTRAL});

    std::cout << "Spawned PowerUp (+" << bonusValue << " HP) at (" << x << ", " << y << ")\n";
} */

const char* SpawnSystem::getEnemyTypeName(AiType type) {
    switch (type) {
        case AiType::BASIC_ENEMY: return "BasicEnemy";
        case AiType::SHOOTING_ENEMY: return "ShootingEnemy";
        case AiType::FOLLOWING_ENEMY: return "FollowingEnemy";
        default: return "Unknown";
    }
}

void SpawnSystem::reset()
{
    m_spawnTimer = 0.0f;
    m_enemiesSpawned = 0;
    std::cout << "[SPAWN] SpawnSystem reset for new level" << std::endl;
}

void SpawnSystem::setDifficulty(float interval, int maxEnemies, int totalEnemies)
{
    m_spawnInterval = interval;
    m_maxEnemies = maxEnemies;
    m_maxTotalEnemies = totalEnemies;
    std::cout << "[SPAWN] Difficulty set: interval=" << interval << "s, max=" << maxEnemies << ", total=" << totalEnemies << std::endl;
}

