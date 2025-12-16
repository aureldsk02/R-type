/*
** EPITECH PROJECT, 2025
** G-CPP-500-COT-5-1-rtype-20
** File description:
** NetworkManager
*/

#ifndef NETWORKMANAGER_HPP_
#define NETWORKMANAGER_HPP_
#include <asio.hpp>
#include <iostream>
#include <vector>
#include <list>
#include "Networkdata.h"
#include <cstdlib>
#include "RoomManager.hpp"
#include <fstream>
#include "../include/json.hpp"
#define PORT 12345
#define HTTP_PORT 8080
using asio::ip::udp;
using asio::ip::tcp;
using json = nlohmann::json;

class NetworkManager {
    protected:
        asio::io_context _ioContext;
        udp::socket _socket;
        udp::endpoint _remoteEndpoint;
        tcp::socket _httpsocket;
        int nextid  = 0;
        char _recvBuffer[1024];
        RoomManager *_room_manager;
        tcp::acceptor _httpAcceptor;
        std::vector<player_entity_t *> _players;
        std::vector <std::string>  _bannedPlayers;
    private:
    public:
        NetworkManager(int port);
        // void handle_stop_http_request();
        // void handle_get_players_request();
        void parse_banned_players();
        bool check_if_endpoint_banned(std::string endpoint);
        void set_room_manager(RoomManager *room_manager);
        void send_to_room();
        void start_receive();
        void handle_broadcast_packet(BroadcastPacket* packet, udp::endpoint player_endpoint);
        void run();
        asio::io_context& getIOContext();
        udp::socket* getSocket() { return &_socket; }
        void handle_receive(std::error_code ec, std::size_t bytes_recvd);
        void send_response(const std::string& message);
        std::list<int> _playerIds;
        void start_http_accept();
        void handle_http_json(const json& command = {});
        ~NetworkManager();
};

#endif /* !NETWORKMANAGER_HPP_ */
