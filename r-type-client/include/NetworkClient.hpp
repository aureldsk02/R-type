/*
** EPITECH PROJECT, 2025
** G-CPP-500-COT-5-1-rtype-20
** File description:
** NetworkClient
*/

#ifndef NETWORKCLIENT_HPP_
#define NETWORKCLIENT_HPP_

#include <asio.hpp>
#include <iostream>
#include <string>
#include <thread>
#include <functional>
#include "Networkdata.h"
#include <queue>
#include <vector>
#include "Networkdata.h"
#include <chrono>

#define DEFAULT_SERVER_PORT 12345
#define DEFAULT_SERVER_IP "127.0.0.1"

using asio::ip::udp;

using PlayerConnectCallback = std::function<void(const PlayerConnectPacket&)>;
using PlayerJoinCallback = std::function<void(const PlayerJoinPacket&)>;
using PlayerDisconnectCallback = std::function<void(const PlayerDisconnectPacket&)>;
using GameStartCallback = std::function<void(const GameStartPacket&)>;
using GameAiCallback = std::function<void(const GameAiPacket&)>;
using GameCollisionCallback = std::function<void(const GameCollisionPacket&)>;
using PlayerHasMoveCallback = std::function<void(const PlayerHasMovePacket&)>;
using BulletCallback = std::function<void(const BulletPacket&)>;
using RoomListCallback = std::function<void(const RoomListPacket&)>;
using RoomPlayersCallback = std::function<void(const Sendplayer_in_room_Packet&)>;
using ChatBroadcastCallback = std::function<void(const MessagePacket&)>;

using PacketCallback = std::function<void(const void*, size_t)>;

class NetworkClient {
    private:
        asio::io_context _ioContext;
        udp::socket _socket;
        udp::endpoint _serverEndpoint;
        char _recvBuffer[1024];
        std::queue<std::vector<char>> _packetQueue;
        std::mutex _queueMutex;
        std::thread _ioThread;
        PlayerConnectCallback _onPlayerConnectCallback;
        PlayerJoinCallback _onPlayerJoinCallback;
        PlayerDisconnectCallback _onPlayerDisconnectCallback;
        GameStartCallback _onGameStartCallback;
        GameAiCallback _onGameAiCallback;
        GameCollisionCallback _onGameCollisionCallback;
        PlayerHasMoveCallback _onPlayerHasMoveCallback;
        BulletCallback _onBulletCallback;
        PacketCallback _onPacketCallback;
        RoomListCallback _onRoomListCallback;
        RoomPlayersCallback _onRoomPlayersCallback;
        ChatBroadcastCallback _onChatBroadcastCallback;
        void handle_received_packet(const char* data, size_t size);
        void process_packet_queue();
        
    public:
        NetworkClient(const std::string& serverIp = DEFAULT_SERVER_IP, int serverPort = DEFAULT_SERVER_PORT);
        void send_data(const void* data, size_t size);
        void start_receive();
        void send_connect_packet(const std::string& name);
        void send_player_move_packet(PlayerMove move, uint32_t sequence);
        void send_create_bullet_packet(float posx, float posy);
        void send_create_bullet_packet(uint32_t roomId);
        void send_connect_to_room(uint32_t roomId, const std::string& playerName);
        void send_create_room();
        void run();
        void stop();
        void set_player_connect_callback(PlayerConnectCallback callback);
        void set_player_join_callback(PlayerJoinCallback callback);
        void set_player_disconnect_callback(PlayerDisconnectCallback callback);
        void set_game_start_callback(GameStartCallback callback);
        void set_game_ai_callback(GameAiCallback callback);
        void set_game_collision_callback(GameCollisionCallback callback);
        void set_player_has_move_callback(PlayerHasMoveCallback callback);
        void set_bullet_callback(BulletCallback callback);
        void set_packet_callback(PacketCallback callback);
        void set_room_list_callback(RoomListCallback callback);
        void set_room_players_callback(RoomPlayersCallback callback);
        // void send_chat_message(const std::string& message);
        void set_chat_broadcast_callback(ChatBroadcastCallback callback) { _onChatBroadcastCallback = callback; }
        void send_chat_message(uint32_t roomId, const std::string& message);
        void send_disconnect_packet();
        void send_get_room_list();
        void send_get_players_in_room(uint32_t roomId);
        void process_packets();
        void initialize(const std::string& playerName);
        uint32_t get_player_id() const 
        { 
            return _playerId;
        }
        uint32_t get_lobby_id() const 
        { 
            return _lobbyId; 
        }
        
        ~NetworkClient();
        
    private:
        uint32_t _playerId = 0;
        uint32_t _lobbyId = 0;
        bool _connectPacketSent = false;
};


#endif /* !NETWORKCLIENT_HPP_ */
