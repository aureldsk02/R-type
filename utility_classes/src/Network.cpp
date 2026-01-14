/*
** EPITECH PROJECT, 2025
** G-CPP-500-COT-5-1-rtype-8
** File description:
** Network
*/

#include "../include/Network.hpp"

std::map<std::string, EnemyType> type_map = {
    {"player1", TYPE_1},        {"player2", TYPE_2},
    {"red_trooper", TYPE_3},    {"walker", TYPE_4},
    {"player_missile", TYPE_5}, {"enemy_missile", TYPE_6},
    {"explosion", TYPE_7},      {"hit_effect", TYPE_8},
    {"floor", TYPE_9},          {"ceiling", TYPE_10},
    {"boss", TYPE_11},          {"plane", TYPE_PLANE},
    {"force", TYPE_FORCE},      {"green_trooper", TYPE_TROOPPER},
    {"tourelles", TYPE_TOURELLES},
    {"big_missile", TYPE_BIG_MISSILE},      {"big_shooter", TYPE_BIG_SHOOTER},
    {"small_shooter", TYPE_SMALL_SHOOTER},      {"final_boss", TYPE_FINAL_BOSS},
    {"space_enemy", TYPE_SPACE_ENEMY},
    {"player2_flipped", PLAYER2_FLIPPED},
    {"evil_player_missile", EVIL_PLAYER_MISSILE},
    {"player3", PLAYER3},
    {"player4", PLAYER4}
};

NetworkManager::NetworkManager(
    int port,
    std::string address,
    std::vector<int8_t>& lastmsg_,
    std::mutex& mtx_,
    asio::io_context & _context
)
    : context(_context),
      socket(context, asio::ip::udp::endpoint(asio::ip::udp::v4(), 0)),
      isrunning(true),
      lastmsg(lastmsg_),
      mtx(mtx_),
      clients_lastmsg(tmp_clients)
{
    try {
        auto ip = asio::ip::make_address(address);
        server_endpoint_ = asio::ip::udp::endpoint(ip, port);
        std::cout << "Client lancé " << std::endl;
        socket.connect(server_endpoint_);
        receive_from_server();
    } catch (const std::exception &e) {
        std::cerr << "Invalid ip address" << std::endl;
        exit(84);
    }
}

NetworkManager::NetworkManager(
    int port,
    std::vector<std::pair<asio::ip::udp::endpoint, std::vector<int8_t>>>&
        clients_lastmsg_,
    std::mutex& mtx_,
    asio::io_context & _context
)
    : context(_context),
      socket(context, asio::ip::udp::endpoint(asio::ip::udp::v4(), port)),
      isrunning(true), clients_lastmsg(clients_lastmsg_), mtx(mtx_),
      lastmsg(tmp_server) {
    std::cout << "Serveur pret à être lancé " << port << std::endl;

    receive_from_clients();
}

NetworkManager::~NetworkManager() {
    isrunning = false;
    socket.close();
}

void NetworkManager::run() {
    if (isrunning) {
        context.run();
    }
}

void NetworkManager::stop()
{
    isrunning = false;
    context.stop();
}

asio::ip::udp::endpoint NetworkManager::getEndpoint() {
    return socket.local_endpoint();
}

void NetworkManager::receive_from_clients() {
    socket.async_receive_from(
        asio::buffer(buff), last_sender_,

        [this](std::error_code error, std::size_t bytes_receive) {
            if (!error && bytes_receive > 0) {
                std::cout << "Receive :" << bytes_receive << std::endl;

                {
                    std::lock_guard<std::mutex> lock(mtx);
                    clients_lastmsg.push_back(
                        {last_sender_,
                         std::vector<int8_t>(
                             buff.begin(), buff.begin() + bytes_receive
                         )}
                    );
                }
            }
            if (isrunning) {
                receive_from_clients();
            }
        }
    );
}

void NetworkManager::send_to_client(
    const std::vector<int8_t>& msg, size_t size,
    const asio::ip::udp::endpoint& to_client
) {
    socket.async_send_to(
        asio::buffer(msg, size * sizeof(int8_t)), to_client,
        [msg](std::error_code error, std::size_t byte_send) {
            std::cout << "Send :" << byte_send << std::endl;

            if (!error) {
            } else {
                std::cerr << error.message() << std::endl;
            }
        }
    );
}

void NetworkManager::receive_from_server() {
    socket.async_receive_from(
        asio::buffer(buff), last_sender_,

        [this](std::error_code error, std::size_t bytes_receive) {
            if (!error && bytes_receive > 0) {
                std::cout << "Receive :" << bytes_receive << std::endl;

                {
                    std::lock_guard<std::mutex> lock(mtx);
                    lastmsg.insert(
                        lastmsg.end(), buff.begin(),
                        buff.begin() + bytes_receive
                    );
                }
            }

            if (isrunning) {
                receive_from_server();
            }
        }
    );
}

void NetworkManager::send_to_server(
    const std::vector<int8_t>& msg, size_t size
) {
    socket.async_send_to(
        asio::buffer(msg, size * sizeof(int8_t)), server_endpoint_,
        [msg](std::error_code error, std::size_t byte_send) {
            std::cout << "Send :" << byte_send << std::endl;

            if (!error) {
            } else {
                std::cerr << error.message() << std::endl;
            }
        }
    );
}
