/*
** EPITECH PROJECT, 2025
** G-CPP-500-COT-5-1-rtype-20
** File description:
** NetworkClient
*/
#include "NetworkClient.hpp"
#include <mutex>
#include <cstring>

NetworkClient::NetworkClient(const std::string& serverIp, int serverPort)
    : _socket(_ioContext, udp::endpoint(udp::v4(), 0)),
      _serverEndpoint(asio::ip::make_address(serverIp), serverPort),
      _playerId(0),
      _lobbyId(0) {}

void NetworkClient::send_create_room()
{
    CreateRoomPacket packet{};
    packet.header.magic = MAGIC_NUMBER;
    packet.header.type = CREATE_ROOM;
    packet.header.version = PROTOCOL_VERSION;
    packet.header.seq = 0;
    std::cout << "Sending CREATE_ROOM request" << std::endl;
    send_data(&packet, sizeof(packet));
}

void NetworkClient::send_create_bullet_packet(uint32_t roomId)
{
    CreateBulletPacket packet{};

    packet.header.magic = MAGIC_NUMBER;
    packet.header.type = BULLET_CREATE;
    packet.header.version = PROTOCOL_VERSION;
    packet.header.seq = 0;

    packet.room_id = roomId;
    packet.player_id = _playerId;

    std::cout << "Sending CreateBulletPacket - Player ID: " << packet.player_id
              << ", Room: " << packet.room_id << std::endl;

    send_data(&packet, sizeof(packet));
}

void NetworkClient::send_connect_to_room(uint32_t roomId, const std::string& playerName)
{
    Connect_to_room_packet packet{};
    packet.header.magic = MAGIC_NUMBER;
    packet.header.type = CONNECT_TO_ROOM;
    packet.header.version = PROTOCOL_VERSION;
    packet.header.seq = 0;
    packet.room_id = roomId;
    std::memset(packet.player_name, 0, sizeof(packet.player_name));
    std::strncpy(reinterpret_cast<char*>(packet.player_name), playerName.c_str(), sizeof(packet.player_name) - 1);
    std::cout << "Sending CONNECT_TO_ROOM for room " << roomId << " as '" << playerName << "'" << std::endl;
    send_data(&packet, sizeof(packet));
}

void NetworkClient::send_get_room_list() {
    RoomListRequestPacket req{};
    req.header.magic = MAGIC_NUMBER;
    req.header.type = GET_ROOM_LIST;
    req.header.version = PROTOCOL_VERSION;
    req.header.seq = 0;
    std::cout << "Requesting room list from server" << std::endl;
    send_data(&req, sizeof(req));
}

void NetworkClient::send_get_players_in_room(uint32_t roomId) {
    Getplayer_in_room_Packet req{};
    req.header.magic = MAGIC_NUMBER;
    req.header.type = PLAYER_IN_ROOM;
    req.header.version = PROTOCOL_VERSION;
    req.header.seq = 0;
    req.room_id = roomId;
    std::cout << "Requesting players in room " << roomId << std::endl;
    send_data(&req, sizeof(req));
}

void NetworkClient::send_data(const void* data, size_t size) {
    if (size >= sizeof(header_t)) {
        const header_t* h = reinterpret_cast<const header_t*>(data);
        const char* typeName = "UNKNOWN";
        switch (h->type) {
            case CONNECT: typeName = "CONNECT"; break;
            case DISCONNECT: typeName = "DISCONNECT"; break;
            case GAME_START: typeName = "GAME_START"; break;
            case PLAYER_HAS_MOVE: typeName = "PLAYER_HAS_MOVE"; break;
            case AI_HAS_MOVE: typeName = "AI_HAS_MOVE"; break;
            case COLLISION: typeName = "COLLISION"; break;
            case GAME_UPDATE: typeName = "GAME_UPDATE"; break;
            case GAME_END: typeName = "GAME_END"; break;
            case JOIN_PACK: typeName = "JOIN_PACK"; break;
            case BULLET_MOVE: typeName = "BULLET_MOVE"; break;
            case BROADCAST: typeName = "BROADCAST"; break;
            case CONNECT_TO_ROOM: typeName = "CONNECT_TO_ROOM"; break;
            case CCNT_TO_ROOM_ERROR: typeName = "CCNT_TO_ROOM_ERROR"; break;
            case CREATE_ROOM: typeName = "CREATE_ROOM"; break;
            case GET_ROOM_LIST: typeName = "GET_ROOM_LIST"; break;
            case SEND_ROOM_LIST: typeName = "SEND_ROOM_LIST"; break;
            case BULLET_CREATE: typeName = "BULLET_CREATE"; break;
            case PLAYER_IN_ROOM: typeName = "PLAYER_IN_ROOM"; break;
            // case CHAT_MESSAGE: typeName = "CHAT_MESSAGE"; break;
            // case CHAT_BROADCAST: typeName = "CHAT_BROADCAST"; break;
            default: break;
        }
        std::cout << ">> SEND packet type=" << typeName << " (" << h->type << ") seq=" << h->seq << " size=" << size << std::endl;
    } else {
        std::cout << ">> SEND raw data size=" << size << std::endl;
    }
    _socket.send_to(asio::buffer(data, size), _serverEndpoint);
}

void NetworkClient::send_connect_packet(const std::string& player_name)
{
    if (_connectPacketSent) {
        return;
    }

    PlayerConnectPacket connectPacket;

    connectPacket.header.magic = MAGIC_NUMBER;
    connectPacket.header.type = CONNECT;
    connectPacket.header.version = PROTOCOL_VERSION;

    strncpy(reinterpret_cast<char*>(connectPacket.player_name), player_name.c_str(), sizeof(connectPacket.player_name));
    connectPacket.player_name[sizeof(connectPacket.player_name) - 1] = '\0';

    std::cout << "Sending PlayerConnectPacket to server..." << std::endl;
    send_data(&connectPacket, sizeof(connectPacket));

    _connectPacketSent = true;
}

void NetworkClient::send_player_move_packet(PlayerMove move, uint32_t sequence)
{
    PlayerMovePacket movePacket;

    movePacket.header.magic = MAGIC_NUMBER;
    movePacket.header.type = PLAYER_HAS_MOVE;
    movePacket.header.version = PROTOCOL_VERSION;
    movePacket.header.seq = sequence;

    movePacket.player_id = _playerId;
    movePacket.room_id = _lobbyId;
    movePacket.move = static_cast<uint32_t>(move);

    std::cout << "Sending PlayerMovePacket - Player ID: " << movePacket.player_id
              << ", Room: " << movePacket.room_id
              << ", Move: " << movePacket.move
              << ", Sequence: " << movePacket.header.seq << std::endl;

    send_data(&movePacket, sizeof(movePacket));
}

void NetworkClient::send_create_bullet_packet(float posx, float posy)
{
    (void)posx;
    (void)posy;
    CreateBulletPacket packet;

    packet.header.magic = MAGIC_NUMBER;
    packet.header.type = BULLET_CREATE;
    packet.header.version = PROTOCOL_VERSION;
    packet.header.seq = 0;

    packet.room_id = _lobbyId;
    packet.player_id = _playerId;

    std::cout << "Sending CreateBulletPacket - Player ID: " << packet.player_id
              << ", Room: " << packet.room_id << std::endl;

    send_data(&packet, sizeof(packet));
}

void NetworkClient::handle_received_packet(const char* data, size_t size) {
    if (size < sizeof(header_t)) {
        std::cerr << "Paquet reçu trop petit pour contenir un en-tête" << std::endl;
        return;
    }

    const header_t* header = reinterpret_cast<const header_t*>(data);

    if (header->magic != MAGIC_NUMBER) {
        std::cerr << "Magic number invalide dans le paquet reçu" << std::endl;
        return;
    }

    std::vector<char> packet(data, data + size);
    {
        std::lock_guard<std::mutex> lock(_queueMutex);
        _packetQueue.push(packet);
    }

    if (_onPacketCallback) {
        _onPacketCallback(data, size);
    }
}

void NetworkClient::start_receive() {
    _socket.async_receive_from(
        asio::buffer(_recvBuffer, 1024), _serverEndpoint,
        [this](std::error_code ec, std::size_t bytes_recvd) {
            if (!ec) {
                handle_received_packet(_recvBuffer, bytes_recvd);
                start_receive();
            } else {
                std::cerr << "Erreur de réception : " << ec.message() << std::endl;
            }
        });
}

void NetworkClient::set_player_connect_callback(PlayerConnectCallback callback) {
    _onPlayerConnectCallback = callback;
}

void NetworkClient::set_player_join_callback(PlayerJoinCallback callback) {
    _onPlayerJoinCallback = callback;
}

void NetworkClient::set_player_disconnect_callback(PlayerDisconnectCallback callback) {
    _onPlayerDisconnectCallback = callback;
}

void NetworkClient::set_game_start_callback(GameStartCallback callback) {
    _onGameStartCallback = callback;
}

void NetworkClient::set_game_ai_callback(GameAiCallback callback) {
    _onGameAiCallback = callback;
}

void NetworkClient::set_game_collision_callback(GameCollisionCallback callback) {
    _onGameCollisionCallback = callback;
}

void NetworkClient::set_player_has_move_callback(PlayerHasMoveCallback callback) {
    _onPlayerHasMoveCallback = callback;
}

void NetworkClient::set_bullet_callback(BulletCallback callback) {
    _onBulletCallback = callback;
}

void NetworkClient::set_packet_callback(PacketCallback callback) {
    _onPacketCallback = callback;
}

void NetworkClient::set_room_list_callback(RoomListCallback callback) {
    _onRoomListCallback = callback;
}

void NetworkClient::set_room_players_callback(RoomPlayersCallback callback) {
    _onRoomPlayersCallback = callback;
}

void NetworkClient::process_packets() {
    std::lock_guard<std::mutex> lock(_queueMutex);
    while (!_packetQueue.empty()) {
        const std::vector<char>& packet = _packetQueue.front();
        const char* data = packet.data();
        size_t size = packet.size();

        const header_t* header = reinterpret_cast<const header_t*>(data);

        switch (header->type) {
            case CONNECT: {
                if (size >= sizeof(PlayerJoinPacket)) {
                    const PlayerJoinPacket* joinPacket = reinterpret_cast<const PlayerJoinPacket*>(data);
                    // Update local ids from join
                    _playerId = joinPacket->player_id;
                    _lobbyId = joinPacket->room_id;
                    std::cout << "Updated client ids from JOIN (CONNECT): playerId=" << _playerId
                              << " roomId=" << _lobbyId << std::endl;
                    if (_onPlayerJoinCallback) {
                        _onPlayerJoinCallback(*joinPacket);
                    }
                } else if (size >= sizeof(PlayerConnectPacket)) {
                    const PlayerConnectPacket* pc = reinterpret_cast<const PlayerConnectPacket*>(data);
                    if (_onPlayerConnectCallback) {
                        _onPlayerConnectCallback(*pc);
                    }
                }
                break;
            }
            case JOIN_PACK: {
                if (size >= sizeof(PlayerJoinPacket)) {
                    const PlayerJoinPacket* joinPacket = reinterpret_cast<const PlayerJoinPacket*>(data);
                    // Update local ids from join
                    _playerId = joinPacket->player_id;
                    _lobbyId = joinPacket->room_id;
                    std::cout << "Updated client ids from JOIN: playerId=" << _playerId
                              << " roomId=" << _lobbyId << std::endl;
                    if (_onPlayerJoinCallback) {
                        _onPlayerJoinCallback(*joinPacket);
                    }
                }
                break;
            }
            case DISCONNECT: {
                if (size >= sizeof(PlayerDisconnectPacket)) {
                    const PlayerDisconnectPacket* disconnectPacket = reinterpret_cast<const PlayerDisconnectPacket*>(data);
                    std::cout << "Player " << disconnectPacket->player_id << " disconnected" << std::endl;
                    if (_onPlayerDisconnectCallback) {
                        _onPlayerDisconnectCallback(*disconnectPacket);
                    }
                }
                break;
            }
            case GAME_START: {
                if (size >= sizeof(GameStartPacket)) {
                    const GameStartPacket* startPacket = reinterpret_cast<const GameStartPacket*>(data);
                    if (_onGameStartCallback) {
                        _onGameStartCallback(*startPacket);
                    }
                }
                break;
            }
            case AI_HAS_MOVE: {
                if (size >= sizeof(GameAiPacket)) {
                    const GameAiPacket* aiPacket = reinterpret_cast<const GameAiPacket*>(data);
                    if (_onGameAiCallback) {
                        _onGameAiCallback(*aiPacket);
                    }
                }
                break;
            }
            case COLLISION: {
                if (size >= sizeof(GameCollisionPacket)) {
                    const GameCollisionPacket* collisionPacket = reinterpret_cast<const GameCollisionPacket*>(data);
                    if (_onGameCollisionCallback) {
                        _onGameCollisionCallback(*collisionPacket);
                    }
                }
                break;
            }
            case PLAYER_HAS_MOVE: {
                if (size >= sizeof(PlayerHasMovePacket)) {
                    const PlayerHasMovePacket* movePacket = reinterpret_cast<const PlayerHasMovePacket*>(data);
                    if (_onPlayerHasMoveCallback) {
                        _onPlayerHasMoveCallback(*movePacket);
                    }
                }
                break;
            }
            case BULLET_MOVE: {
                // std::cout << "NetworkClient: BULLET_MOVE packet received, size: " << size << std::endl;
                if (size >= sizeof(BulletPacket)) {
                    const BulletPacket* bulletPacket = reinterpret_cast<const BulletPacket*>(data);
                    if (_onBulletCallback) {
                        _onBulletCallback(*bulletPacket);
                    } else {
                        // No bullet callback set
                    }
                } else {
                    // std::cout << "NetworkClient: Packet too small for BulletPacket" << std::endl;
                }
                break;
            }
            
            case SEND_ROOM_LIST: {
                if (size >= sizeof(RoomListPacket)) {
                    const RoomListPacket* listPacket = reinterpret_cast<const RoomListPacket*>(data);
                    if (_onRoomListCallback) {
                        _onRoomListCallback(*listPacket);
                    }
                }
                break;
            }
            case PLAYER_IN_ROOM: {
                const size_t baseOffset = sizeof(header_t);
                if (size < baseOffset + sizeof(uint32_t) * 2) {
                    break;
                }
                uint32_t room_id = 0;
                uint32_t num_players = 0;
                std::memcpy(&room_id, data + baseOffset, sizeof(uint32_t));
                std::memcpy(&num_players, data + baseOffset + sizeof(uint32_t), sizeof(uint32_t));
                if (num_players > 4) num_players = 4;
                const size_t namesOffset = baseOffset + sizeof(uint32_t) * 2;
                const size_t needed = namesOffset + num_players * 16;
                if (size < needed) {
                    break;
                }
                std::vector<uint8_t> names(num_players * 16);
                std::memcpy(names.data(), data + namesOffset, num_players * 16);
                if (_onRoomPlayersCallback) {
                    Sendplayer_in_room_Packet tmp{};
                    tmp.header = *reinterpret_cast<const header_t*>(data);
                    tmp.room_id = room_id;
                    tmp.num_players = num_players;
                    tmp.player_names = reinterpret_cast<char*>(names.data());
                    _onRoomPlayersCallback(tmp);
                }
                break;
            }
            case MSG_PACKET: {
                if (size >= sizeof(MsgPacket)) {
                    const MsgPacket* msgPacket = reinterpret_cast<const MsgPacket*>(data);
                    std::cout << "[CHAT] Message reçu du joueur " << msgPacket->player_id << std::endl;
                    if (_onChatBroadcastCallback) {
                        MessagePacket msgPkt{};
                        msgPkt.header = msgPacket->header;
                        msgPkt.player_id = msgPacket->player_id;
                        std::memcpy(msgPkt.message, msgPacket->message, sizeof(msgPkt.message));
                        _onChatBroadcastCallback(msgPkt);
                    }
                }
                break;
            }
            default:
                std::cout << "Paquet de type inconnu reçu: " << header->type << std::endl;
                break;
        }
        _packetQueue.pop();
    }
}

void NetworkClient::initialize(const std::string& playerName) {
    try {
        (void)playerName;
        start_receive();
        run();
        std::cout << "Network initialized. Deferred PlayerConnect until user action." << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Network initialization error: " << e.what() << std::endl;
    }
}

void NetworkClient::run() {
    if (_ioThread.joinable()) return;
    _ioThread = std::thread([this]() { _ioContext.run(); });
}

void NetworkClient::stop() {
    _ioContext.stop();
    if (_ioThread.joinable()) {
        _ioThread.join();
    }
}

NetworkClient::~NetworkClient() {
    try {
        stop();
        _socket.close();
    } catch (...) {
    }
}

void NetworkClient::send_chat_message(uint32_t roomId, const std::string& message)
{
    MsgPacket pkt{};
    pkt.header.magic = MAGIC_NUMBER;
    pkt.header.type = MSG_PACKET;
    pkt.header.version = PROTOCOL_VERSION;
    pkt.header.seq = 0;
    pkt.room_id = roomId;
    pkt.player_id = _playerId;
    std::memset(pkt.message, 0, sizeof(pkt.message));
    std::strncpy(reinterpret_cast<char*>(pkt.message), message.c_str(), sizeof(pkt.message) - 1);
    send_data(&pkt, sizeof(pkt));
    std::cout << "[CHAT] Message envoyé: " << message << std::endl;
}

void NetworkClient::send_disconnect_packet()
{
    PlayerDisconnectPacket pkt{};
    pkt.header.magic = MAGIC_NUMBER;
    pkt.header.type = DISCONNECT;
    pkt.header.version = PROTOCOL_VERSION;
    pkt.header.seq = 0;
    pkt.player_id = _playerId;
    send_data(&pkt, sizeof(pkt));
    std::cout << "[DISCONNECT] Paquet de déconnexion envoyé au serveur (player_id=" << _playerId << ")" << std::endl;
}
