/*
** EPITECH PROJECT, 2025
** G-CPP-500-COT-5-1-rtype-20
** File description:
** GameRoom
*/

#include "../include/GameRoom.hpp"
#include <iostream>

GameRoom::GameRoom(int room_id, udp::socket *socket): _room_id(room_id), _socket(socket)
{
    _check = time(NULL);
    game_start = false;
    for (int i = 0; i < 4; i++)
        _player_move[i] = PlayerMove::STOP;
    signal(SIGSEGV, signalHandler);
    signal(SIGABRT, signalHandler);
}

GameRoom::~GameRoom()
{
}


void GameRoom::addPlayer(player_entity_t *player)
{
    std::cout << "the player endpoint is " << player->_playerEndpoint.address().to_string() << "and port " << player->_playerEndpoint.port() << std::endl;
    _endpoints.push_back(player->_playerEndpoint);
    player->last_check_seq = 0;
    player->player_score = 0;
    player->last_activity = time(NULL);
    _player_entities.push_back(player);
    _players.push_back(registry.createEntity());
    player->id = _players[_players.size() - 1];
    std::cout << "Player " << player->name << " with ID " << player->id << " added to room " << _room_id << std::endl;
    registry.addComponent(_players[_players.size() - 1], Position{200.0f, 200.0f * _player_entities.size()});
    registry.addComponent(_players[_players.size() - 1], Velocity{0.0f, 0.0f});
    registry.addComponent(_players[_players.size() - 1], Health{100, 100});
    registry.addComponent(_players[_players.size() - 1], CollisionBox{32.0f, 32.0f});
    registry.addComponent(_players[_players.size() - 1], Team{Team::Individu::PLAYER});
    _player_entities[player->id]->last_x = 200.0f;
    _player_entities[player->id]->last_y = 200.0f * _player_entities.size();
}


bool GameRoom::can_game_start()
{
    std::cout << "Asking for game start\n";
    std::cout << "time diff :" << difftime(time(NULL), _check);
    int i = 0;
    for (auto _ent: registry.getEntitiesWith<Team>()){
        auto& team = registry.getComponent<Team>(_ent);
            i++;
    }
    std::cout << "number of players in room: " << i << std::endl;
    if (difftime(time(NULL), _check) > 30 || _players.size() == 4) {
        scene.loadScene(registry, "firstScene", spawn);
        std::cout << "game can start\n";
        return true;
    }
    return false;
}

void GameRoom::removePlayer(int id)
{
    for (int i = 0; i < _endpoints.size(); i++) {
        if (_players[i] == id) {
            PlayerDisconnectPacket disconnectPacket;
            disconnectPacket.header.magic = MAGIC_NUMBER;
            disconnectPacket.header.version = PROTOCOL_VERSION;
            disconnectPacket.header.type = DISCONNECT;
            disconnectPacket.player_id = id;
            char _msg[sizeof(disconnectPacket)];
            std::memcpy(_msg, &disconnectPacket, sizeof(disconnectPacket));
            for (auto _endpoint : _endpoints) {
                _socket->send_to(asio::buffer(_msg, sizeof(_msg)), _endpoint);
            }
            _endpoints.erase(_endpoints.begin() + i);
            if (_endpoints.size() == 0)
                game_over = true;
            break;
        }
    }
    for (auto it = _player_entities.begin(); it != _player_entities.end(); ++it) {
        if ((*it)->id == id) {
            _player_entities.erase(it);
            return;
        }
    }
    for (auto it = _players.begin(); it != _players.end(); ++it) {
        if (*it == id) {
            _players.erase(it);
            return;
        }
    }
}

void GameRoom::start_game()
{
    game_start = true;
}

int GameRoom::find_player_id_by_endpoint(std::string end)
{
    for (auto pla: _player_entities){
        std::cout << "checking endpoint: " << pla->_playerEndpoint.address().to_string() << "!" << std::to_string(pla->_playerEndpoint.port()) << std::endl;
        std::cout << "against endpoint: " << end << std::endl;
        if (pla->_playerEndpoint.address().to_string() + "!" + std::to_string(pla->_playerEndpoint.port()) == end){
            std::cout << "found player id: " << pla->id << std::endl;
            return pla->id;
        }
    }
    return -1;
}

Entity *GameRoom::getPlayer(int id)
{
    int c = 0;
    for (auto player : _players) {
        if (player == id) {
            return &_players[0];
        }
        c++;
    }
    return nullptr;
}

bool GameRoom::is_game_running()
{
    return game_start;
}

bool GameRoom::isFull()
{
    return _players.size() >= 4;
}

std::vector<Entity> GameRoom::getPlayers()
{
    return _players;
}

void GameRoom::player_move(int id, u_int32_t move, uint32_t seq){
    std::cout << "Player " << id << " move: " << move << " with seq: " << seq << std::endl;
    // if (_player_entities[id]->last_check_seq > seq)
    //     return;
    // if (_player_entities[id]->last_check_seq + 1 == seq){
    //     mt.lock();
        _player_move[id] = move;
        // mt.unlock();
    // }
    _player_entities[id]->last_check_seq = seq;
}

void GameRoom::send_game_start_packet()
{
    char _msg[sizeof(GameStartPacket)];
    GameStartPacket start;
    start.header.magic = MAGIC_NUMBER;
    start.header.version = PROTOCOL_VERSION;
    start.header.type = GAME_START;
    start.room_id = _room_id;
    start.num_players = _players.size();
    std::memcpy(_msg, &start, sizeof(start));
    for (auto _endpoint : _endpoints) {
        _socket->send_to(asio::buffer(_msg, sizeof(_msg)), _endpoint);
    }

}

void GameRoom::create_bullet(uint32_t player_id)
{
    Entity shooter = player_id;
    shooting_enemy.shoot(registry, shooter);
}

Sendplayer_in_room_Packet GameRoom::send_player_list_to_room(udp::endpoint asker)
{
    Sendplayer_in_room_Packet packet;
    packet.header.magic = MAGIC_NUMBER;
    packet.header.version = PROTOCOL_VERSION;
    packet.header.type = PLAYER_IN_ROOM;
    packet.room_id = _room_id;
    packet.num_players = _players.size();
    std::string player_names;
    std::cout << "num of player = " << _players.size();
    std::cout << "asker endpoint: " << asker.address().to_string() << " port: " << asker.port() << std::endl;
    
    for (size_t i = 0; i < _players.size(); ++i) {
        std::cout << "player name: " << _player_entities[i]->name << std::endl;
        player_names += _player_entities[i]->name;
        player_names += '\n';
    }
    packet.player_names = (char *)malloc(sizeof(char) * (player_names.size() + 1));
    packet.player_names[player_names.size()] = '\0';
    std::memcpy(packet.player_names, player_names.c_str(), player_names.size());
    return packet;
}


void GameRoom::check_player_activity()
{
    // time_t now = time(NULL);

    // for (auto it = _player_entities.begin(); it != _player_entities.end(); ) {
    //     player_entity_t *player = *it;
    //     double diff = difftime(now, player->last_activity);
    //     if (diff > 60) {
    //         std::cout << "Player " << player->name << " timed out due to inactivity." << std::endl;
    //         removePlayer(player->id);
    //         it = _player_entities.erase(it);
    //     } else {
    //         ++it;
    //     }
    // }
}

bool GameRoom::does_game_end()
{
    return game_over;
}

void GameRoom::send_sms_to_player(int player_id, const std::string &sms)
{
    MsgPacket msgPacket;
    msgPacket.header.magic = MAGIC_NUMBER;
    msgPacket.header.version = PROTOCOL_VERSION;
    msgPacket.header.type = MSG_PACKET;
    msgPacket.player_id = player_id;
    msgPacket.room_id = _room_id;
    std::memcpy(msgPacket.message, sms.c_str(), sms.size());
    
    char buffer[sizeof(msgPacket)];
    std::memcpy(buffer, &msgPacket, sizeof(msgPacket));
    int c = 0;   
    for (auto &endpoint : _endpoints) {
        // if (c == player_id) {
        //     continue;
        // }
        _socket->send_to(asio::buffer(buffer, sizeof(buffer)), endpoint);
        c++;
    }
}

int GameRoom::update_game()
{
    std::cout << "Updating game in room " << _room_id << std::endl;
    if (!game_start) {
        if (can_game_start()) {
            start_game();
            send_game_start_packet();
            std::cout << "Game started in room " << _room_id << std::endl;
            for (auto ent : _player_entities){
                ent->first_check = true;
                ent->last_activity =  time(NULL);
            }
        } else {
            std::cout << "Waiting for players to join..." << std::endl;
            return 0;
        }
    }
    // check_player_activity();
    const int MAX_FRAMES = 50;
    const float DT = 0.5f;
    int frame = 0;
    bool levelComplete = false;
    int c = 0;
    if (_endpoints.size() == 0)
        game_over = true;
    if (game_over == true){
        return 1;
    }
    // while (frame < MAX_FRAMES && !levelComplete) {
        std::cout << " Frame " << frame << " Time: " << (frame * DT) << "s" << std::endl;

        spawn.update(registry, DT);
        ai.update(registry, DT);
        for (auto _entity :  registry.getEntitiesWith<Team>()){
            c++;
            auto& team = registry.getComponent<Team>(_entity);
            if (team.ind == Team::Individu::PLAYER && registry.hasComponent<Health>(_entity)){
                Velocity &vel = registry.getComponent<Velocity>(_entity);
                std::cout << "Player entity " << _entity << " move command: " << _player_move[_entity] << std::endl;
                if (_player_move[_entity] == UP)
                vel.dy = -30.0f;
                else if (_player_move[_entity] == DOWN)
                    vel.dy = 30.0f;
                    else if (_player_move[_entity] == LEFT)
                    vel.dx = -30.0f;
                else if (_player_move[_entity] == RIGHT)
                    vel.dx = 30.0f;
                else if (_player_move[_entity] == STOP){
                    vel.dx = 0.0f;
                        vel.dy = 0.0f;
                    }
                    move.update(registry, DT);
                Position pos = registry.getComponent<Position>(_entity);
                char _msg[sizeof(PlayerHasMovePacket)];
                PlayerHasMovePacket info;
                info.header.magic = MAGIC_NUMBER;
                info.header.version = PROTOCOL_VERSION;
                info.header.type = PLAYER_HAS_MOVE;
                info.header.seq = _player_entities[_entity]->last_check_seq;
                info.player_id = _entity;
                info.room_id = _room_id;
                std::cout << "new pos x " << pos.x << " and new pos_y " << pos.y << std::endl;
                std::cout << "old pos x " << _player_entities[_entity]->last_x << " and old pos_y " << _player_entities[_entity]->last_y  << std::endl;
                float delta_x = pos.x - (float)_player_entities[_entity]->last_x;
                info.position_x = (int16_t) pos.x;
                float delta_y = pos.y - (float)_player_entities[_entity]->last_y;
                info.position_y = (int16_t) pos.y;
                std::cout << "Player " << _entity << " moved to (info.position_x" << static_cast<int>(info.position_x) << ", " << static_cast<int>(info.position_y) << ")" << std::endl;
                bool send = true;
                if (delta_x == 0 && delta_y == 0){
                    send = false;
                }
                _player_entities[_entity]->last_x = pos.x;
                _player_entities[_entity]->last_y = pos.y;
                info.velocity_x = vel.dx;
                info.velocity_y = vel.dy;
                std::memcpy(_msg, &info, sizeof(info));
                if (send == true){
                    for (auto _end: _endpoints)
                        _socket->send_to(asio::buffer(_msg, sizeof(_msg)), _end);
                }
                _player_move[_entity] = STOP;
                }
            }
        bool collide = false;
        auto collisionPairs = collision.update(registry, DT);
        for (auto _pair: collisionPairs){
            GameCollisionPacket _col_packet;
            _col_packet.header.magic = MAGIC_NUMBER;
            _col_packet.header.version = PROTOCOL_VERSION;
            _col_packet.header.type = COLLISION;
            _col_packet.header.seq = 0;
            _col_packet.score = -1;
            _col_packet.player_id = -1;
            auto team_a = registry.getComponent<Team>(_pair.first);
            auto team_b = registry.getComponent<Team>(_pair.second);
            if (team_a.ind == Team::Individu::PLAYER && registry.hasComponent<Health>(_pair.first)) {
                PlayerDisconnectPacket disconnectPacket;
                disconnectPacket.header.magic = MAGIC_NUMBER;
                disconnectPacket.header.version = PROTOCOL_VERSION;
                disconnectPacket.header.type = DISCONNECT;
                disconnectPacket.player_id = _pair.first;
                _player_entities.erase(_player_entities.begin() + _pair.first);
                char _msg[sizeof(disconnectPacket)];
                std::memcpy(_msg, &disconnectPacket, sizeof(disconnectPacket));
                _socket->send_to(asio::buffer(_msg, sizeof(_msg)), _endpoints[_pair.first]);
                _endpoints.erase(_endpoints.begin() + _pair.first);
                if (_endpoints.size() == 0){
                    game_over = true;
                }
            }
            if (team_b.ind == Team::Individu::PLAYER && registry.hasComponent<Health>(_pair.second)) {
                PlayerDisconnectPacket disconnectPacket;
                disconnectPacket.header.magic = MAGIC_NUMBER;
                disconnectPacket.header.version = PROTOCOL_VERSION;
                disconnectPacket.header.type = DISCONNECT;
                disconnectPacket.player_id = _pair.second;
                _player_entities.erase(_player_entities.begin() + _pair.second);
                char _msg[sizeof(disconnectPacket)];
                std::memcpy(_msg, &disconnectPacket, sizeof(disconnectPacket));
                _socket->send_to(asio::buffer(_msg, sizeof(_msg)), _endpoints[_pair.second]);
                _endpoints.erase(_endpoints.begin() + _pair.second);
                if (_endpoints.size() == 0){
                    game_over = true;
                }
            }
            std::cout << "COLLISION between" << _pair.first << " and " << _pair.second << std::endl;
            if (!registry.hasComponent<Health>(_pair.first) && team_a.ind == Team::Individu::PLAYER) { 
                std::cout << "[WHOSE BULLET] Entity with id :" << _pair.first << " is a player bullet" << std::endl;
                for (int i = 0; i < _player_entities.size(); i++){
                    for (int j = 0;  j < registry.bullets[i].size(); j++){
                        if (registry.bullets[i][j] == _pair.first){
                            _player_entities[i]->player_score += 50;
                            _col_packet.score = _player_entities[i]->player_score;
                            _col_packet.player_id = i;
                            registry.bullets[i].erase(registry.bullets[i].begin() + j);
                        }
                    }
                }
            }
            else if (!registry.hasComponent<Health>(_pair.second) && team_a.ind == Team::Individu::PLAYER){
                for (int i = 0; i < _player_entities.size(); i++){
                    for (int j = 0;  j < registry.bullets[i].size(); j++){
                        if (registry.bullets[i][j] == _pair.second){
                            _player_entities[i]->player_score += 50;
                            _col_packet.score = _player_entities[i]->player_score;
                            _col_packet.player_id = i;
                            std::cout << "[PLAYER] Bullet packet has made collision" << std::endl;
                        }
                    }
                }
            }
            _col_packet.entity_id_a = _pair.first;
            _col_packet.entity_id_b = _pair.second;
            char _msg[sizeof(_col_packet)];
            std::memcpy(_msg, &_col_packet, sizeof(_col_packet));
            for (auto _endpoint : _endpoints) {
                _socket->send_to(asio::buffer(_msg, sizeof(_msg)), _endpoint);
            }
        }
        destroy.update(registry, DT);
        auto entities = registry.getEntitiesWith<Team>();
        for (Entity e : entities) {
            auto& team = registry.getComponent<Team>(e);
            if (team.ind == Team::Individu::PLAYER && !registry.hasComponent<Health>(e)) {
                                        char _msg[sizeof(BulletPacket)];
                        BulletPacket _bullet_packet;
                        _bullet_packet.header.magic = MAGIC_NUMBER;
                        _bullet_packet.header.version = PROTOCOL_VERSION;
                        _bullet_packet.header.type = BULLET_MOVE;
                        std::cout << "[INFO] bULLET WITH id:" << e << "moved again" << std::endl; 
                        _bullet_packet.is_player_bullet = 1;
                        _bullet_packet.bullet_id = e;
                        _bullet_packet.player_id = e;
                        auto& pos = registry.getComponent<Position>(e);
                        auto& vel = registry.getComponent<Velocity>(e);
                        _bullet_packet.position_x = pos.x;
                        _bullet_packet.position_y = pos.y;
                        _bullet_packet.direction_x = vel.dx;
                        _bullet_packet.direction_y = vel.dy;
                        std::memcpy(_msg, &_bullet_packet, sizeof(_bullet_packet));
                        for (auto _endpoint : _endpoints) {
                            _socket->send_to(asio::buffer(_msg, sizeof(_msg)), _endpoint);
                        }

            } else if (team.ind == Team::Individu::ENEMY) {
                // Distinguer missiles des ennemis
                // exit(24);
                if (registry.hasComponent<Damage>(e) && 
                    !registry.hasComponent<Health>(e)) {
                        char _msg[sizeof(BulletPacket)];
                        BulletPacket _bullet_packet;
                        _bullet_packet.header.magic = MAGIC_NUMBER;
                        _bullet_packet.header.version = PROTOCOL_VERSION;
                        _bullet_packet.header.type = BULLET_MOVE;
                        std::cout << "[INFO] bULLET WITH id:" << e << "moved again" << std::endl; 
                            _bullet_packet.is_player_bullet = 0;
                        _bullet_packet.bullet_id = e;
                        _bullet_packet.player_id = e;
                        auto& pos = registry.getComponent<Position>(e);
                        auto& vel = registry.getComponent<Velocity>(e);
                        _bullet_packet.position_x = pos.x;
                        _bullet_packet.position_y = pos.y;
                        _bullet_packet.direction_x = vel.dx;
                        _bullet_packet.direction_y = vel.dy;
                        std::memcpy(_msg, &_bullet_packet, sizeof(_bullet_packet));
                        for (auto _endpoint : _endpoints) {
                            _socket->send_to(asio::buffer(_msg, sizeof(_msg)), _endpoint);
                        }
                        // std::cout << "Sending bullet: " << "x" << _bullet_packet.position_x << " y: " << _bullet_packet.position_y << " move packet for entity " << e << std::endl;
                } else {
                    if (registry.hasComponent<Health>(e)) {
                        char _msg[sizeof(GameAiPacket)];
                        GameAiPacket _ai_packet;
                        _ai_packet.header.magic = MAGIC_NUMBER;
                        _ai_packet.header.version = PROTOCOL_VERSION;
                        _ai_packet.header.type = AI_HAS_MOVE;
                        _ai_packet.entity_id = e;
                        if (std::string(spawn.getEnemyTypeName(registry.getComponent<Ai>(e).type)) == "BasicEnemy"){
                            _ai_packet.entity_type = WHICH_AI::BASIC_ENEMY;
                            std::cout << "[INFO] Basic enemy with id: " << e << " moved again" << std::endl;
                        }
                        else if (std::string(spawn.getEnemyTypeName(registry.getComponent<Ai>(e).type)) == "ShootingEnemy"){
                            _ai_packet.entity_type = WHICH_AI::SHOOTING_ENEMY;
                            std::cout << "[INFO] Shooting enemy with id: " << e << " moved again" << std::endl;
                        }
                        else if (std::string(spawn.getEnemyTypeName(registry.getComponent<Ai>(e).type)) == "FollowingEnemy"){
                            _ai_packet.entity_type = WHICH_AI::FOLLOWING_ENEMY;
                            std::cout << "[INFO] Following enemy with id: " << e << " moved again" << std::endl;
                        }
                        auto& pos = registry.getComponent<Position>(e);
                        auto& vel = registry.getComponent<Velocity>(e);
                        _ai_packet.position_x = pos.x;
                        _ai_packet.position_y = pos.y;
                        _ai_packet.velocity_x = vel.dx;
                        _ai_packet.velocity_y = vel.dy;
                        std::memcpy(_msg, &_ai_packet, sizeof(_ai_packet));
                        for (auto _endpoint : _endpoints) {
                            _socket->send_to(asio::buffer(_msg, sizeof(_msg)), _endpoint);
                        }
                    }
                }
            }
        }
        destroy.update(registry, DT);
    // }
    return 0;
}
