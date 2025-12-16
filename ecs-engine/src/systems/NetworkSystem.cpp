/*
** EPITECH PROJECT, 2025
** network
** File description:
** network sync file
*/

#include "NetworkSystem.hpp"
#include "Component.hpp"

void NetworkSystem::update(Registry& registry, float dt)
{
    //Envoi des positions des entités synchronisées
    
    for (Entity e = 0; e < 1000; e++) {
        if (registry.hasComponent<Position>(e) && registry.hasComponent<Networked>(e)) {
            auto& pos = registry.getComponent<Position>(e);
            auto& netComp = registry.getComponent<Networked>(e);
            net.sendEntityUpdate(netComp.id, pos.x, pos.y);
        }
    }

    //La Réception des mises à jour
    auto updates = net.receiveUpdates();
    for (auto& upd : updates) {
        if (registry.hasComponent<Position>(upd.entityId)) {
            auto& pos = registry.getComponent<Position>(upd.entityId);
            pos.x = upd.x;
            pos.y = upd.y;
        }
    }
}
