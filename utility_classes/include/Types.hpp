/*
** EPITECH PROJECT, 2025
** G-CPP-500-COT-5-1-rtype-8
** File description:
** Network
*/

#include <asio.hpp>
#include <cstdlib>
#include <ctype.h>
#include <map>
#include <string>
#include <vector>
#include "entity.hpp"

#ifndef TYPES_HPP_
#define TYPES_HPP_

enum Direction : int16_t { UP = 0, DOWN = 1, LEFT = 2, RIGHT = 3 };

enum Action : int16_t { SPACE = 4 };

enum BULLET_TYPE : int16_t { SOLIDE = 0, NONE = 1 };

enum GAMESTATE : int16_t { WIN = 5, LOSE = 6};

struct Vector2D {
    int16_t x;
    int16_t y;
};

/**
 * @brief The different types of enemies and game entities
 * Each type has a unique value. For example, the first player has an value of 1
 * (TYPE_1)
 *
 */
enum EnemyType : int16_t {
    TYPE_1 = 1,
    TYPE_2 = 2,
    TYPE_3 = 3,
    TYPE_4 = 4,
    TYPE_5 = 5,
    TYPE_6 = 6,
    TYPE_7 = 7,
    TYPE_8 = 8,
    TYPE_9 = 9,
    TYPE_10 = 10,
    TYPE_11 = 11,
    TYPE_PLANE = 12,
    TYPE_FORCE = 13,
    TYPE_TROOPPER = 14,
    TYPE_TOURELLES = 15,
    TYPE_FINAL_BOSS = 16,
    TYPE_BIG_SHOOTER = 17,
    TYPE_SMALL_SHOOTER = 18,
    TYPE_BIG_MISSILE = 19,
    TYPE_SPACE_ENEMY = 20,
    PLAYER2_FLIPPED = 21,
    EVIL_PLAYER_MISSILE = 22,
    PLAYER3 = 23,
    PLAYER4 = 24
};

/**
 * @brief The type map. Maps the names of the enemies or game entities to their
 * types For example, the name of the first player is "player1" and its type is
 * TYPE_1
 */
extern std::map<std::string, EnemyType> type_map;

// Serveur -> Client
struct EnemyMovedResponse {
    int8_t type; // 0x37
    int16_t enemy_id;
    EnemyType enemy_type;
    Vector2D position;
};

// Serveur -> Client
struct MessageHeader {
    int8_t type; // 0x38
    uint32_t id;
    int16_t nbr;
    int64_t timeElapsed;
};

struct MoveResponse {
    int8_t type; // 0x24
    int16_t player_id;
    Direction direction;
};

struct GameState {
    int8_t type; // 0x40
    GAMESTATE gState;
};

struct ActionResponse {
    int8_t type; // 0x25
    int16_t player_id;
    Action input;
};

typedef struct client_info {
    asio::ip::udp::endpoint endpoint;
    int player_id;
    std::vector<int8_t> lastmsg;
} client_info_t;

/**
 * @brief A struct to store infos on an entity to be spawned in the level
 *
 */
typedef struct entity_info_s {
    entity entity_id;
    std::string type;
    double spawn_time;
    double spawn_y;
} entity_info_t;

#endif /* !TYPES_HPP_ */
