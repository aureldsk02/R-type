/*
** EPITECH PROJECT, 2025
** G-CPP-500-COT-5-1-rtype-20
** File description:
** Networkdata
*/

#ifndef NETWORKDATA_H_
#define NETWORKDATA_H_
#include <iostream>
#include <cstdint>
#include <asio.hpp>
using asio::ip::udp;
#define MAGIC_NUMBER 0x5678
using asio::ip::udp;
#define PROTOCOL_VERSION 1
enum PlayerMove {
    UP,
    DOWN,
    LEFT,
    RIGHT,
    STOP,
};

enum PacketType {
    CONNECT,
    DISCONNECT,
    GAME_START,
    PLAYER_HAS_MOVE,
    AI_HAS_MOVE,
    COLLISION,
    GAME_UPDATE,
    GAME_END,
    JOIN_PACK,
    BULLET_MOVE,
    BROADCAST,
    CONNECT_TO_ROOM,
    CCNT_TO_ROOM_ERROR,
    CREATE_ROOM,
    GET_ROOM_LIST,
    SEND_ROOM_LIST,
    BULLET_CREATE,
    PLAYER_IN_ROOM,
    GAME_CHANGE,
    MSG_PACKET,
};

enum MachineType {
    SERVER,
    CLIENT,
};

enum EntityType {
    PLAYER,
    ENEMY,
    PROJECTILE,
    POWERUP,
};

enum WHICH_AI {
    BASIC_ENEMY,
    SHOOTING_ENEMY,
    FOLLOWING_ENEMY,
};

enum updateType {
    POSITION,
    HEALTH,
    SCORE,
};

typedef struct PlayerEntity
{
    uint32_t id;
    char name[16];
    udp::endpoint _playerEndpoint;
    time_t last_activity;
    bool first_check;
    uint32_t last_check_seq;
    uint32_t seq;
    uint32_t player_score;
    uint32_t last_x;
    uint32_t last_y;
} player_entity_t;

typedef struct PlayerComponent
{
    uint32_t id;
    float position_x;
    float position_y;
    float velocity_x;
    float velocity_y;
    uint32_t health;
    uint32_t score;
} player_cmp_t;

#pragma pack(push, 1)



typedef struct header
{
    uint32_t magic;
    uint32_t type;
    uint32_t version;
    u_int32_t seq = 0;
}header_t;

struct CreateRoomPacket {
    header_t header;
};

struct MsgPacket {
    header_t header;
    uint32_t player_id;
    uint8_t message[256];
    uint32_t room_id;
}; 

struct RoomListRequestPacket {
    header_t header;
};

struct GameEndPacket {
    header_t header;
    uint32_t room_id;
    uint32_t winner_id;
};


struct RoomListPacket {
    header_t header;
    uint32_t num_rooms;
    uint32_t room_ids[4];
    uint32_t player_per_room[4];
};

struct Getplayer_in_room_Packet {
    header_t header;
    uint32_t room_id;
};

struct Sendplayer_in_room_Packet {
    header_t header;
    uint32_t room_id;
    uint32_t num_players;
    char *player_names;
};

struct JsonPlayerConnectedPacket {
    header_t header;
    uint32_t player_id;
    uint8_t player_name[16];
    std::string _ednpoints;
};

struct PlayerJoinPacket {
    header_t header;
    uint32_t player_id;
    uint8_t player_name[16];
    uint32_t ship_type;
    u_int32_t room_id;
    float x;
    float y;
};


struct PlayerJoinRoomPacket {
    header_t header;
    uint32_t player_id;
    uint8_t player_name[16];
    uint32_t ship_type;
    u_int32_t room_id;
    float x;
    float y;
};

struct PlayerConnectPacket {
    header_t header;
    uint8_t player_name[16];
};

struct GameStartPacket {
    header_t header;
    u_int32_t room_id;
    uint32_t num_players;
};

struct Connect_to_room_packet{
    header_t header;
    uint32_t room_id;
    uint8_t player_name[16];
};

struct GameAiPacket {
    header_t header;
    uint32_t entity_id;
    uint32_t entity_type;
    float position_x;
    float position_y;
    float velocity_x;
    float velocity_y;
};

struct BulletPacket {
    header_t header;
    uint32_t bullet_id;
    uint32_t player_id;
    float position_x;
    float position_y;
    float direction_x;
    uint32_t is_player_bullet;
    float direction_y;
};



struct CreateBulletPacket {
    header_t header;
    uint32_t room_id;
    uint32_t player_id;
};

struct MessagePacket {
    header_t header;
    uint32_t player_id;
    uint32_t message[256];
};

struct GameChangePacket {
    header_t header;
};

struct GameCollisionPacket {
    header_t header;
    uint32_t entity_id_a;
    uint32_t entity_id_b;
    uint32_t score;
    uint32_t player_id;
};

struct PlayerHasMovePacket {
    header_t header;
    uint32_t player_id;
    u_int32_t room_id;
    int16_t position_x;
    int16_t position_y;
    int8_t velocity_x;
    int8_t velocity_y;
};

struct PlayerMovePacket {
    header_t header;
    uint32_t player_id;
    u_int32_t room_id;
    uint32_t move;
};

struct PlayerShootPacket {
    header_t header;
    uint32_t player_id;
    uint32_t weapon_type;
    float direction_x;
    float direction_y;
};

struct PlayerDisconnectPacket {
    header_t header;
    uint32_t player_id;
};

struct BroadcastPacket {
    header_t header;
};

struct BroadcastResponsePacket {
    header_t header;
    udp::endpoint _endpoint;
    uint32_t type;
};
#pragma pack(pop)
#endif /* !NETWORKDATA_H_ */
