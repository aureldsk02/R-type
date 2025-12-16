/*
** EPITECH PROJECT, 2025
** G-CPP-500-COT-5-1-rtype-20
** File description:
** NetworkManager
*/

#include "../include/NetworkManager.hpp"

NetworkManager::NetworkManager(int port)
    : _socket(_ioContext, udp::endpoint(udp::v4(), port)),
      _httpsocket(_ioContext),
      _httpAcceptor(_ioContext, tcp::endpoint(tcp::v4(), HTTP_PORT)) {
  std::cout << "Server started on port " << port << std::endl;
  parse_banned_players();
  signal(SIGSEGV, signalHandler);
  signal(SIGABRT, signalHandler);
  nextid = 0;
}

NetworkManager::~NetworkManager() {}

void NetworkManager::parse_banned_players() {
  std::ifstream file("banned_players");
  std::string line;
  if (_bannedPlayers.size() > 0)
    _bannedPlayers.clear();
  while (std::getline(file, line)) {
    _bannedPlayers.push_back(line);
  }
  file.close();
}

void NetworkManager::set_room_manager(RoomManager *room_manager) {
  _room_manager = room_manager;
}

asio::io_context &NetworkManager::getIOContext() { return _ioContext; }
void NetworkManager::start_http_accept() {
  _httpAcceptor.async_accept(_httpsocket, [this](std::error_code ec) {
    if (!ec) {
      std::cout << "HTTP connection accepted from "
                << _httpsocket.remote_endpoint().address().to_string() << ":"
                << _httpsocket.remote_endpoint().port() << std::endl;
      try {
        asio::streambuf buffer;
        asio::read(_httpsocket, buffer, asio::transfer_at_least(1));
        std::string http_data(asio::buffers_begin(buffer.data()),
                              asio::buffers_begin(buffer.data()) +
                                  buffer.size());
        std::cout << "HTTP data received:\n" << http_data << std::endl;
        if (http_data.find("OPTIONS") != std::string::npos) {
          std::cout << "CORS Preflight request detected" << std::endl;
          std::string response =
              "HTTP/1.1 200 OK\r\n"
              "Access-Control-Allow-Origin: *\r\n"
              "Access-Control-Allow-Methods: GET, POST, OPTIONS\r\n"
              "Access-Control-Allow-Headers: Content-Type\r\n"
              "Content-Length: 0\r\n"
              "\r\n";
          asio::write(_httpsocket, asio::buffer(response));
          std::cout << "CORS Preflight response sent" << std::endl;
        } else if (http_data.find("POST") != std::string::npos) {
          size_t json_start = http_data.find("{");
          if (json_start != std::string::npos) {
            std::string json_str = http_data.substr(json_start);
            std::cout << "JSON found: " << json_str << std::endl;
            try {
              json command = json::parse(json_str);
              handle_http_json(command);
            } catch (const std::exception &e) {
              std::cout << "JSON error: " << e.what() << std::endl;
            }
          }
          std::cout << "POST request processed" << std::endl;
          // exit(0);
        } else {
          std::cout << "Unknown request type" << std::endl;
        }
      } catch (const std::exception &e) {
        std::cout << "HTTP error: " << e.what() << std::endl;
        exit(84);
      }
    }
    _httpsocket.close();
    start_http_accept();
  });
}

void NetworkManager::handle_http_json(const json &command) {
  if (command.is_null() || command.empty()) {
    std::cout << "JSON vide ou null reçu" << std::endl;
    return;
  }
  std::string action = command.value("action", "unknown");
  std::cout << "Action: " << action << std::endl;
  if (action == "stop") {
    std::cout << "Arrêt demandé" << std::endl;
    exit(0);
  } else if (action == "get_players") {
    json response;
    std::cout << "Getting players list" << std::endl;
    response["name"] = json::array();
    response["id"] = json::array();
    response["endpoints"] = json::array();
    response["banned"] = json::array();
    response["scores"] = json::array();
    for (auto end : _bannedPlayers) {
      response["banned"].push_back(end);
    }
    for (const auto &player : _players) {
      // json player_json;
      // player_json["id"].push_back(player->id);
      // std::cout << "Player ID: " << player->id << std::endl;
      // player_json["name"].push_back(player->name);
      // std::cout << "Player Name: " << player->name << std::endl;
      response["name"].push_back(player->name);
      response["id"].push_back(player->id);
      response["endpoints"].push_back(
          player->_playerEndpoint.address().to_string() + "!" +
          std::to_string(player->_playerEndpoint.port()));
    }
    std::string response_str = response.dump();
    std::string http_response = "HTTP/1.1 200 OK\r\n"
                                "Content-Type: application/json\r\n"
                                "Content-Length: " +
                                std::to_string(response_str.size()) +
                                "\r\n"
                                "Access-Control-Allow-Origin: *\r\n"
                                "\r\n" +
                                response_str;
    asio::write(_httpsocket, asio::buffer(http_response));
    std::cout << "Sent players list via HTTP" << std::endl;
  } else if (action == "ban_player") {
    std::string endpoint = command.value("endpoints", "unknown");
    std::cout << "banned_player: " << endpoint << std::endl;
    if (!check_if_endpoint_banned(endpoint)) {
      std::ofstream file("banned_players", std::ios::app);
      file << endpoint + "\n";
      file.close();
      parse_banned_players();
    }
    for (auto p : _players) {
      std::string player_endpoint = p->_playerEndpoint.address().to_string() +
                                    "!" +
                                    std::to_string(p->_playerEndpoint.port());
      int c = 0;
      if (player_endpoint == endpoint) {
        _players.erase(_players.begin() + c);
        c++;
      }
    }
  } else if (action == "get_banned") {
    json response;
    // exit(0);
    response["banned"] = json::array();
    for (auto ban : _bannedPlayers) {
      std::cout << "a ban player" << ban << std::endl;
      response["banned"].push_back(ban);
    }
    std::string response_str = response.dump();
    std::string http_response = "HTTP/1.1 200 OK\r\n"
                                "Content-Type: application/json\r\n"
                                "Content-Length: " +
                                std::to_string(response_str.size()) +
                                "\r\n"
                                "Access-Control-Allow-Origin: *\r\n"
                                "\r\n" +
                                response_str;
    asio::write(_httpsocket, asio::buffer(http_response));
    std::cout << "[ALERT]: sending banned players list";
    // exit(0);
  } else if (action == "unban") {
    std::string unban_endpoint = command.value("endpoints", "unknown");
    std::ofstream file("banned_players");
    int i = 0;
    for (auto end : _bannedPlayers) {
      if (end == unban_endpoint) {
        _bannedPlayers.erase(_bannedPlayers.begin() + i);
      } else
        file << end + "\n";
      i++;
    }
    file.close();
  }
}

void NetworkManager::start_receive() {
  start_http_accept();
  _socket.async_receive_from(
      asio::buffer(_recvBuffer), _remoteEndpoint,
      [this](std::error_code ec, std::size_t bytes_recvd) {
        std::cout << "receive packer form"
                  << _remoteEndpoint.address().to_string();
        handle_receive(ec, bytes_recvd);
      });
}

void NetworkManager::handle_broadcast_packet(BroadcastPacket *packet,
                                             udp::endpoint player_endpoint) {
  std::cout << "Handling BROADCAST packet from "
            << player_endpoint.address().to_string() << ":"
            << player_endpoint.port() << std::endl;

  BroadcastResponsePacket responsePacket;
  responsePacket.header.magic = MAGIC_NUMBER;
  responsePacket.header.version = PROTOCOL_VERSION;
  responsePacket.header.type = BROADCAST;
  responsePacket._endpoint = _socket.local_endpoint();
  responsePacket.type = SERVER;

  char msg[sizeof(responsePacket)];
  std::memcpy(msg, &responsePacket, sizeof(responsePacket));
  _socket.send_to(asio::buffer(msg, sizeof(msg)), player_endpoint);
  std::cout << "Sent BROADCAST response to "
            << player_endpoint.address().to_string() << ":"
            << player_endpoint.port() << std::endl;
}

bool NetworkManager::check_if_endpoint_banned(std::string endpoint) {
  bool check = false;

  for (auto end : _bannedPlayers)
    if (end == endpoint)
      check = true;
  if (check) {
    int nb_rooms = _room_manager->numberOfRooms();
    for (int i = 0; i < nb_rooms; i++) {
      GameRoom *_room = _room_manager->getRoomById(i);
      std::cout << "\nChecking room " << i << " for banned endpoint\n";
      if (_room->find_player_id_by_endpoint(endpoint) > -1) {
        std::cout << "Find the player id in a room\n";
        _room->removePlayer(i);
      }
    }
  }
  return check;
}

void NetworkManager::handle_receive(std::error_code ec,
                                    std::size_t bytes_recvd) {
  if (check_if_endpoint_banned(_remoteEndpoint.address().to_string() + "!" +
                               std::to_string(_remoteEndpoint.port()))) {
    std::cout << "[WARNING] banned PLAYER trying to connect\n";
    return;
  }
  if (!ec && bytes_recvd > 0) {
    std::string message(_recvBuffer, bytes_recvd);
    std::cout << "receive\n";
    header_t *header = reinterpret_cast<header_t *>(_recvBuffer);
    std::cout << "Packet type: " << header->type << std::endl;
    std::cout << "Magic: " << std::hex << header->magic << std::dec
              << std::endl;
    std::cout << "Version: " << header->version << std::endl;
    if (header->magic != MAGIC_NUMBER || header->version != PROTOCOL_VERSION) {
      std::cerr << "Invalid packet received from "
                << _remoteEndpoint.address().to_string() << ":"
                << _remoteEndpoint.port() << std::endl;
      start_receive();
      return;
    }
    if (header->type == CONNECT) {
      std::cout << "Handling CONNECT packet" << std::endl;
      PlayerConnectPacket *joinPacket =
          reinterpret_cast<PlayerConnectPacket *>(_recvBuffer);
      player_entity_t *newPlayer =
          (player_entity_t *)malloc(sizeof(player_entity_t));
      newPlayer->id = nextid;
      std::memcpy(newPlayer->name, joinPacket->player_name, 16);
      newPlayer->_playerEndpoint = _remoteEndpoint;
      newPlayer->player_score = 0;
      _players.push_back(newPlayer);
      _room_manager->handlePlayerConnectPacket(joinPacket, _remoteEndpoint);
      std::cout << "Player " << newPlayer->name << " connected with ID "
                << newPlayer->id << std::endl;
      nextid++;
    } else if (header->type == BROADCAST) {
      BroadcastPacket *broadcastPacket =
          reinterpret_cast<BroadcastPacket *>(_recvBuffer);
      handle_broadcast_packet(broadcastPacket, _remoteEndpoint);
      std::cout << "Broadcast packet received" << std::endl;
    } else if (header->type == DISCONNECT) {
      PlayerDisconnectPacket *disconnectpacket =
          reinterpret_cast<PlayerDisconnectPacket *>(_recvBuffer);
      uint32_t player_id = disconnectpacket->player_id;
      int count = 0;
      _room_manager->handlePlayerDisconnectPacket(player_id);
      std::cout << "Player with ID " << player_id << " disconnected."
                << std::endl;
    } else if (header->type == PLAYER_HAS_MOVE) {
      PlayerMovePacket *movePacket =
          reinterpret_cast<PlayerMovePacket *>(_recvBuffer);
      _room_manager->handlePlayerMovePacket(movePacket, _remoteEndpoint);
      std::cout << "Player " << movePacket->player_id << " moved in room "
                << movePacket->room_id << std::endl;
    } else if (header->type == CONNECT_TO_ROOM) {
      Connect_to_room_packet *roomPacket =
          reinterpret_cast<Connect_to_room_packet *>(_recvBuffer);
      player_entity_t *newPlayer =
          (player_entity_t *)malloc(sizeof(player_entity_t));
      newPlayer->id = nextid;
      std::memcpy(newPlayer->name, roomPacket->player_name, 16);
      newPlayer->_playerEndpoint = _remoteEndpoint;
      if (_room_manager->connected_to_a_specific_room(newPlayer,
                                                      roomPacket->room_id)) {
        std::cout << "Player " << newPlayer->name << " connected to room "
                  << roomPacket->room_id << std::endl;
        nextid++;
      } else {
        std::cout << "Failed to connect player " << newPlayer->name
                  << " to room " << roomPacket->room_id << std::endl;
      }
    } else if (header->type == CREATE_ROOM) {
      if (_room_manager->createRoom()) {
        std::cout << "New room created successfully." << std::endl;
      } else {
        std::cout << "Failed to create a new room." << std::endl;
      }
    } else if (header->type == GET_ROOM_LIST) {
      std::cout << "Handling GET_ROOM_LIST packet" << std::endl;
      RoomListPacket roomListPacket;
      roomListPacket.header.magic = MAGIC_NUMBER;
      roomListPacket.header.version = PROTOCOL_VERSION;
      roomListPacket.header.type = SEND_ROOM_LIST;
      roomListPacket.num_rooms = _room_manager->numberOfRooms();
      for (int i = 0; i < roomListPacket.num_rooms; ++i) {
        roomListPacket.room_ids[i] = i;
        roomListPacket.player_per_room[i] =
            _room_manager->getRoomById(i)->getPlayers().size();
      }
      _socket.send_to(asio::buffer(&roomListPacket, sizeof(roomListPacket)),
                      _remoteEndpoint);
      std::cout << "Sent room list to "
                << _remoteEndpoint.address().to_string();
    } else if (header->type == BULLET_CREATE) {
      CreateBulletPacket *bulletPacket =
          reinterpret_cast<CreateBulletPacket *>(_recvBuffer);
      GameRoom *room = _room_manager->getRoomById(bulletPacket->room_id);
      if (room) {
        room->create_bullet(bulletPacket->player_id);
        std::cout << "Player " << bulletPacket->player_id
                  << " created a bullet in room " << bulletPacket->room_id
                  << std::endl;
      } else {
        std::cout << "Room " << bulletPacket->room_id
                  << " not found for bullet creation." << std::endl;
      }

    } else if (header->type == PLAYER_IN_ROOM) {
      Sendplayer_in_room_Packet *sendPacket =
          reinterpret_cast<Sendplayer_in_room_Packet *>(_recvBuffer);
      _room_manager->send_player_list_to_room(sendPacket->room_id,
                                              _remoteEndpoint);
    } else if (header->type == MSG_PACKET) {
      MsgPacket *msgPacket = reinterpret_cast<MsgPacket *>(_recvBuffer);
      GameRoom *room = _room_manager->getRoomById(msgPacket->room_id);
      std::string message(reinterpret_cast<const char *>(msgPacket->message),
                          sizeof(msgPacket->message));
      if (message.empty()) {
        std::cout << "Empty message received from player "
                  << msgPacket->player_id << std::endl;
        return;
      }
      room->send_sms_to_player(msgPacket->player_id, message);
      std::cout << "Received message from player " << msgPacket->player_id
                << ": " << msgPacket->message << std::endl;
    } else {
      std::cout << "Unknown packet type received: " << header->type
                << std::endl;
    }
  }
  start_receive();
}

void NetworkManager::send_to_room() {
  int id = _room_manager->room_id_get_available_room();
  GameRoom *room = _room_manager->getRoomById(id);
}

void NetworkManager::send_response(const std::string &message) {
  _socket.send_to(asio::buffer(message), _remoteEndpoint);
}

void NetworkManager::run() { _ioContext.run(); }