/*
** EPITECH PROJECT, 2025
** G-CPP-500-COT-5-1-rtype-20
** File description:
** RoomManger
*/

#include "../include/RoomManager.hpp"
#define PORT 12345
#include "../include/Networkdata.h"
RoomManager::RoomManager()
{
    _next_id = 0;
    _active_rooms = 0;
    _is_running = true;
    signal(SIGSEGV, signalHandler);
    signal(SIGABRT, signalHandler);
}

RoomManager::~RoomManager()
{
}

void RoomManager::send_player_list_to_room(int room_id, udp::endpoint asker)
{
    GameRoom* room = getRoomById(room_id);
    Sendplayer_in_room_Packet packet = room->send_player_list_to_room(asker);
    char buffer[sizeof(packet)];
    if (packet.num_players == 0)
        return;
    std::memcpy(buffer, &packet, sizeof(packet));
    std::cout << "Sending player list to room " << packet.room_id << " with " << packet.player_names << " players." << std::endl;
    _socket->send_to(asio::buffer(buffer, sizeof(buffer)), asker);
}

int RoomManager::room_id_get_available_room()
{
    if (_active_rooms == 0){
        _game_rooms.push_back(std::make_unique<GameRoom>(_active_rooms, _socket));
        _active_rooms++;
        return 0;
    }
    for (int i = 0; i < _game_rooms.size(); i++) {
        if (_game_rooms[i]->getPlayers().size() < 4 && !_game_rooms[i]->is_game_running()) {
            return i;
        }
    }
    return -1;
}

bool RoomManager::removePlayerFromRoom(uint32_t player_id, int room_id)
{
    std::cout << "Removing player " << player_id << " from room " << room_id << std::endl;
    if (room_id < 0 || room_id >= _game_rooms.size()) {
        throw std::out_of_range("Room ID out of range");
    }
    _game_rooms[room_id]->removePlayer(player_id);
    if (_game_rooms[room_id]->getPlayers().empty()) {
        _game_rooms.erase(_game_rooms.begin() + room_id);
        _active_rooms--;
    }
    return true;
}

void RoomManager::updateRooms()
{
    while (_is_running) {
        std::cout << "Updating rooms..." << std::endl;
        int c = 0;
        for (auto &room : _game_rooms) {
            if (room->does_game_end()) {
                std::cout << "Game over in room " << c << std::endl;
                _game_rooms.erase(_game_rooms.begin() + c);
                c++;
                continue;
            }
            c++;
            room->update_game();
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }
}

GameRoom* RoomManager::getRoomById(int id)
{
    std::cout << "Getting room by ID: " << id << std::endl;
    if (id < 0 || id >= _game_rooms.size()) {
        throw std::out_of_range("Room ID out of range");
    }
    return _game_rooms[id].get();
}

bool RoomManager::removeRoomById(int id )
{
    if (id < 0 || id >= _game_rooms.size()) {
        return false; // Invalid room ID
    }
    _game_rooms.erase(_game_rooms.begin() + (id ));
    return true;
}

bool RoomManager::isRoomFull(int room_id)
{
    std::cout << "Checking if room " << room_id << " is full." << std::endl;
    if (room_id < 0 || room_id >= _game_rooms.size()) {
        throw std::out_of_range("Room ID out of range");
    }
    return _game_rooms[room_id]->isFull();
}

int RoomManager::numberOfRooms() const
{
    return _game_rooms.size();
}

bool RoomManager::addPlayerToRoom(player_entity_t *player, int room_id)
{
    if (room_id < 0 || room_id >= _game_rooms.size()) {
        return false;
    }
    if (_game_rooms[room_id]->isFull()) {
        return false;
    }
    _game_rooms[room_id]->addPlayer(player);
    return true;
}

bool RoomManager::createRoom(){
    if (_game_rooms.size() < 4){
        _game_rooms.push_back(std::make_unique<GameRoom>(_active_rooms, _socket));
        return true;
    }
    return false;
}

bool RoomManager::connected_to_a_specific_room(player_entity_t *player, int room_id){
    GameRoom* room = getRoomById(room_id);
    if (!room->isFull()) {
        room->addPlayer(player);
        return true;
    }
    _game_rooms[room_id]->addPlayer(player);
    PlayerJoinRoomPacket join;
    join.header.magic = MAGIC_NUMBER;
    join.header.version = PROTOCOL_VERSION;
    join.header.type = CONNECT;
    join.player_id = player->id;
    join.room_id = room_id;
    join.x = 200;
    join.y = player->id + 1 * 200;
    std::memcpy(join.player_name, player->name, 16);
    join.ship_type = 1; // Default ship type
    char buffer[sizeof(join)];
    std::memcpy(buffer, &join, sizeof(join));
    std::cout << "Packet send to" <<  player->_playerEndpoint.address().to_string() << " and port: " << player->_playerEndpoint.port() << std::endl;
    _socket->send_to(asio::buffer(buffer, sizeof(buffer)), player->_playerEndpoint);
    std::cout << "Player " << player->name << " connected with ID " << player->id << " to room " << room_id << std::endl;
    return false;
}

GameRoom* RoomManager::findRoomByPlayerId(uint32_t player_id)
{
    for (auto &room : _game_rooms) {
        for (auto player : room->getPlayers()) {
            if (player == player_id) {
                return room.get();
            }
        }
    }
    return nullptr;
}

void RoomManager::handlePlayerConnectPacket(PlayerConnectPacket* packet, udp::endpoint player_endpoint)
{
    player_entity_t *newPlayer = (player_entity_t *)malloc(sizeof(player_entity_t));
    newPlayer->id = _next_id;
    std::memcpy(newPlayer->name, packet->player_name, 16);
    newPlayer->_playerEndpoint = player_endpoint;
    int room_id = room_id_get_available_room();
    std::cout << "room id = " << room_id << std::endl;
    bool minus = false;
    // _game_rooms[room_id - 1].set_socker(_socket);
    if (room_id == -1) {
        _game_rooms.push_back(std::make_unique<GameRoom>(_active_rooms, _socket));
        room_id = _game_rooms.size();
        minus = true;
        _active_rooms++;
    }
    std::cout << "[INFO] room id after creation = " << room_id << std::endl;
    if (minus == false)
        _game_rooms[room_id]->addPlayer(newPlayer);
    else
        _game_rooms[room_id - 1]->addPlayer(newPlayer);
    PlayerJoinPacket join;
    join.header.magic = MAGIC_NUMBER;
    join.header.version = PROTOCOL_VERSION;
    join.header.type = CONNECT;
    join.player_id = newPlayer->id;
    join.room_id = room_id;
    join.x = 200;
    join.y = (newPlayer->id + 1) * 200;
    std::memcpy(join.player_name, newPlayer->name, 16);
    join.ship_type = 1; // Default ship type
    char buffer[sizeof(join)];
    std::memcpy(buffer, &join, sizeof(join));
    std::cout << "Packet send to" <<  player_endpoint.address().to_string() << " and port: " << player_endpoint.port() << std::endl;
    _socket->send_to(asio::buffer(buffer, sizeof(buffer)), player_endpoint);
    std::cout << "Player " << newPlayer->name << " connected with ID " << newPlayer->id << " to room " << room_id << std::endl;
}

void RoomManager::handlePlayerMovePacket(PlayerMovePacket* packet, udp::endpoint player_endpoint)
{
    GameRoom* room = getRoomById(packet->room_id);
    if (room) {
        room->player_move(packet->player_id, packet->move, packet->header.seq);
        std::cout << "Player " << packet->player_id << " moved in room " << packet->room_id << std::endl;
    } else {
        std::cout << "Player with ID " << packet->player_id << " not found in any room." << std::endl;
    }
}

void RoomManager::handlePlayerDisconnectPacket(uint32_t player_id)
{
    GameRoom* room = findRoomByPlayerId(player_id);
    if (room) {
        room->removePlayer(player_id);
        std::cout << "Player with ID " << player_id << " disconnected." << std::endl;
        for (auto it = _game_rooms.begin(); it != _game_rooms.end(); ++it) {
            if ((*it)->getPlayers().empty()) {
                _game_rooms.erase(it);
                _active_rooms--;
                break;
            }
        }
    } else {
        std::cout << "Player with ID " << player_id << " not found in any room." << std::endl;
    }
}
