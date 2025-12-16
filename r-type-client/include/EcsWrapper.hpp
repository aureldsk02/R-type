/*
** EPITECH PROJECT, 2025
** ECS Wrapper
** File description:
** Wrapper pour éviter les conflits de noms avec l'ECS
*/

#pragma once
#include <cstddef>
#include <vector>

class Registry;
class MovementSystem;
struct Position;
struct Velocity;
struct Team;

class EcsManager {
private:
    Registry* registry;
    MovementSystem* movementSystem;
    
public:
    using EntityId = std::size_t;
    
    EcsManager();
    ~EcsManager();
    
    EntityId createEntity();
    void destroyEntity(EntityId entity);
    
    void addPosition(EntityId entity, float x, float y);
    void addVelocity(EntityId entity, float dx, float dy);
    void addTeam(EntityId entity, int teamType);
    
    bool hasPosition(EntityId entity);
    bool hasVelocity(EntityId entity);
    bool hasTeam(EntityId entity);
    
    void setPosition(EntityId entity, float x, float y);
    void setVelocity(EntityId entity, float dx, float dy);
    void getPosition(EntityId entity, float& x, float& y);
    void getVelocity(EntityId entity, float& dx, float& dy);
    
    void updateMovement(float deltaTime);
    
    std::vector<EntityId> getEntitiesWithPosition();
    std::vector<EntityId> getPlayersEntities();
};