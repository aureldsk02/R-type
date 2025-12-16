/*
** EPITECH PROJECT, 2025
** ECS Wrapper Implementation
** File description:
** Implémentation du wrapper ECS
*/

#include "EcsWrapper.hpp"
#include "ecs/Registry.hpp"
#include "ecs/Component.hpp"
#include "ecs/MovementSystem.hpp"

EcsManager::EcsManager() {
    registry = new Registry();
    movementSystem = new MovementSystem();
}

EcsManager::~EcsManager() {
    delete movementSystem;
    delete registry;
}

EcsManager::EntityId EcsManager::createEntity() {
    return static_cast<EntityId>(registry->createEntity());
}

void EcsManager::destroyEntity(EntityId entity) {
    registry->destroyEntity(static_cast<Entity>(entity));
}

void EcsManager::addPosition(EntityId entity, float x, float y) {
    Position pos = {x, y};
    registry->addComponent<Position>(static_cast<Entity>(entity), pos);
}

void EcsManager::addVelocity(EntityId entity, float dx, float dy) {
    Velocity vel = {dx, dy};
    registry->addComponent<Velocity>(static_cast<Entity>(entity), vel);
}

void EcsManager::addTeam(EntityId entity, int teamType) {
    Team team;
    team.ind = static_cast<Team::Individu>(teamType);
    registry->addComponent<Team>(static_cast<Entity>(entity), team);
}

bool EcsManager::hasPosition(EntityId entity) {
    return registry->hasComponent<Position>(static_cast<Entity>(entity));
}

bool EcsManager::hasVelocity(EntityId entity) {
    return registry->hasComponent<Velocity>(static_cast<Entity>(entity));
}

bool EcsManager::hasTeam(EntityId entity) {
    return registry->hasComponent<Team>(static_cast<Entity>(entity));
}

void EcsManager::setPosition(EntityId entity, float x, float y) {
    if (hasPosition(entity)) {
        Position& pos = registry->getComponent<Position>(static_cast<Entity>(entity));
        pos.x = x;
        pos.y = y;
    }
}

void EcsManager::setVelocity(EntityId entity, float dx, float dy) {
    if (hasVelocity(entity)) {
        Velocity& vel = registry->getComponent<Velocity>(static_cast<Entity>(entity));
        vel.dx = dx;
        vel.dy = dy;
    }
}

void EcsManager::getPosition(EntityId entity, float& x, float& y) {
    if (hasPosition(entity)) {
        Position& pos = registry->getComponent<Position>(static_cast<Entity>(entity));
        x = pos.x;
        y = pos.y;
    }
}

void EcsManager::getVelocity(EntityId entity, float& dx, float& dy) {
    if (hasVelocity(entity)) {
        Velocity& vel = registry->getComponent<Velocity>(static_cast<Entity>(entity));
        dx = vel.dx;
        dy = vel.dy;
    }
}

void EcsManager::updateMovement(float deltaTime) {
    movementSystem->update(*registry, deltaTime);
}

std::vector<EcsManager::EntityId> EcsManager::getEntitiesWithPosition() {
    std::vector<EntityId> result;
    auto entities = registry->getEntitiesWith<Position>();
    for (Entity e : entities) {
        result.push_back(static_cast<EntityId>(e));
    }
    return result;
}

std::vector<EcsManager::EntityId> EcsManager::getPlayersEntities() {
    std::vector<EntityId> result;
    auto entities = registry->getEntitiesWith<Team>();
    for (Entity e : entities) {
        Team& team = registry->getComponent<Team>(e);
        if (team.ind == Team::Individu::PLAYER) {
            result.push_back(static_cast<EntityId>(e));
        }
    }
    return result;
}