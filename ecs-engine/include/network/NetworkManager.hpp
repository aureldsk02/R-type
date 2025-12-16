/*
** EPITECH PROJECT, 2025
** Network manager
** File description:
** manage Network
*/

#pragma once
#include <vector>

struct EntityUpdate {
  int entityId;
  float x, y;
};

class NetworkManager {
public:
  virtual ~NetworkManager() = default;
  virtual void sendEntityUpdate(int id, float x, float y) = 0;
  virtual std::vector<EntityUpdate> receiveUpdates() = 0;
};
