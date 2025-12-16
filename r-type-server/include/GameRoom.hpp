/*
** EPITECH PROJECT, 2025
** G-CPP-500-COT-5-1-rtype-20
** File description:
** GameRoom
*/

#ifndef GAMEROOM_HPP_
#define GAMEROOM_HPP_
#include "./Networkdata.h"
#include "ecs/Registry.hpp"
#include <chrono>
#include <ctime>
#include <iostream>
#include <thread>
#include <vector>

#include "ecs/AiSystem.hpp"
#include "ecs/CollisionSystem.hpp"
#include "ecs/Component.hpp"
#include "ecs/DestroySystem.hpp"
#include "ecs/HealthSystem.hpp"
#include "ecs/MovementSystem.hpp"
#include "ecs/Scene_loader.hpp"
#include "ecs/ShootingSystem.hpp"
#include "ecs/SpawnSystem.hpp"
using asio::ip::udp;
class GameRoom {
protected:
  uint32_t _room_id;
  uint32_t _total_id;
  bool game_start;
  time_t _check;
  std::vector<Entity> _players;
  Entity player;
  SceneLoader scene;
  SpawnSystem spawn;
  AiSystem ai;
  MovementSystem move;
  ShootingSystem shooting_enemy;
  CollisionSystem collision;
  HealthSystem health;
  DestroySystem destroy;
  Registry registry;
  std::mutex mt;
  std::vector<player_entity_t *> _player_entities;
  std::vector<udp::endpoint> _endpoints;
  bool game_over = false;
  enum PlayerMove { UP, DOWN, LEFT, RIGHT, STOP };
  uint32_t _player_move[4] = {PlayerMove::STOP, PlayerMove::STOP,
                              PlayerMove::STOP, PlayerMove::STOP};
  udp::socket *_socket;

public:
  bool does_game_end();
  GameRoom(int room_id, udp::socket *socket);
  ~GameRoom();
  Sendplayer_in_room_Packet send_player_list_to_room(udp::endpoint asker);
  int find_player_id_by_endpoint(std::string end);
  void check_player_activity();
  bool is_game_running();
  bool can_game_start();
  void start_game();
  void player_move(int id, uint32_t move, uint32_t seq);
  void player_shoot(int id);
  void addPlayer(player_entity_t *player);
  void send_player_has_move_packet(uint32_t player_id, uint32_t pos_x,
                                   uint32_t pos_y, u_int32_t vel_x,
                                   uint32_t vel_y);
  void create_bullet(uint32_t player_id);
  void send_ai_data();
  void send_collision_data();
  void removePlayer(int id);
  void send_game_start_packet();
  void set_socker(udp::socket *socket) { _socket = socket; };
  void send_sms_to_player(int player_id, const std::string &sms);
  void scoreboard();
  int update_game();
  Entity *getPlayer(int id);
  std::vector<Entity> getPlayers();
  bool isFull();
};

#endif /* !GAMEROOM_HPP_ */
