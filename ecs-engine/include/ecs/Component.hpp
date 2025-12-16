/*
** EPITECH PROJECT, 2025
** component
** File description:
** c'est la structure basique pour les composants
*/

#pragma once

struct Position
{
    float x, y;
};

struct Velocity
{
    float dx, dy;
};

struct Damage
{
    int hit;
};

struct Health
{
    int currentHP;
    int maxHP;
};

struct DestroyTag{};

struct CollisionBox {
    float width;
    float height;
    float offset_x = 0;
    float offset_y = 0;
};

struct Team {
    enum Individu {PLAYER, ENEMY, NEUTRAL};
    Individu ind;
};

enum class AiType {
    BASIC_ENEMY,
    SHOOTING_ENEMY,
    FOLLOWING_ENEMY
};


enum class AiState {
    MOVING_LEFT,
    MOVING_RIGHT,
    FOLLOWING_PLAYER,
    ATTACKING
};

struct Ai {
    AiType type;
    AiState state;
    float timer;
    float stateTimer;
    float shootCooldown;
    float spawnY;
};

struct PowerUp
{
    int bonus;
};

struct Weapon
{
    float fireRate;
    float shootCooldown;
    int damage;
};

//Composant réseau pour marquer une entité synchronisée
struct Networked
{
    int id;
}; 
