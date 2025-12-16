/*
** EPITECH PROJECT, 2025
** G-CPP-500-COT-5-1-rtype-20
** File description:
** RoomManager
*/

#ifndef ROOMMANAGER_HPP_
#define ROOMMANAGER_HPP_
#include "GameRoom.hpp"
#include <thread>
#include <memory>
using asio::ip::udp;

class RoomManager {
    protected:
        std::vector <std::unique_ptr<GameRoom>> _game_rooms;
        int _next_id;
        udp::socket *_socket;
        int _active_rooms;
        bool _is_running;
        public:
        RoomManager();
        ~RoomManager();
        void send_player_list_to_room(int room_id, udp::endpoint asker);
        // void send_to(char *buffer, void *data);
        int room_id_get_available_room();
        void set_socket(udp::socket *socket) { _socket = socket; };
        GameRoom* getRoomById(int id);
        bool removeRoomById(int id);
        int numberOfRooms() const;
        bool addPlayerToRoom(player_entity_t *player, int room_id);
        bool removePlayerFromRoom(uint32_t player_id, int room_id);
        void updateRooms();
        void handlePlayerConnectPacket(PlayerConnectPacket* packet, udp::endpoint player_endpoint);
        void handlePlayerMovePacket(PlayerMovePacket* packet, udp::endpoint player_endpoint);
        void handlePlayerDisconnectPacket(uint32_t player_id);
        bool createRoom();
        bool connected_to_a_specific_room(player_entity_t *player, int room_id);
        GameRoom* findRoomByPlayerId(uint32_t player_id);
        bool isRoomFull(int room_id);
        const std::vector<GameRoom>& getAllRooms() const;
    private:
};

#endif /* !ROOMMANAGER_HPP_ */
