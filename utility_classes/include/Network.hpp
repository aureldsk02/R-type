/*
** EPITECH PROJECT, 2025
** G-CPP-500-COT-5-1-rtype-8
** File description:
** Network
*/

#ifndef NETWORK_HPP_
#define NETWORK_HPP_

#include <asio.hpp>
#include <cstdlib>
#include <ctype.h>
#include <fcntl.h>
#include <filesystem>
#include <iostream>
#include <limits.h>
#include <queue>
#include <sstream>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <thread>
#include <time.h>
#include <vector>

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
// Alias pour compatibilité
typedef uint8_t u_int8_t;
typedef uint16_t u_int16_t;
typedef uint32_t u_int32_t;
typedef uint64_t u_int64_t;
#else
#include <poll.h>
#include <unistd.h>

#include <arpa/inet.h>
#include <netinet/ip.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#endif
#include <SFML/Graphics.hpp>

#include "Types.hpp"

class NetworkManager {
  public:
    // Constructeurs and destructeurs
    NetworkManager(
        int port, std::string address, std::vector<int8_t>& lastmsg_,
        std::mutex& mtx_, asio::io_context & _context
    );

    NetworkManager(
        int port,
        std::vector<std::pair<asio::ip::udp::endpoint, std::vector<int8_t>>>&
            clients_lastmsg_,
        std::mutex& mtx_, asio::io_context & _context
    );
    ~NetworkManager();

    // Methodes for running the network io_context

    /**
     * @brief Start the context and networking operations
     */
    void run();

    /**
     * @brief Stop the context and networking operations
     */
    void stop();

    /**
     * @brief Receive messages from clients
     */
    void receive_from_clients();
    void send_to_client(
        const std::vector<int8_t>& msg, size_t size,
        const asio::ip::udp::endpoint& client
    );

    /**
     * @brief Receive messages from the server
     */
    void receive_from_server();

    /**
     * @brief Send a message to the server
     * @param msg The message to send
     * @param size The size of the message
     */
    void send_to_server( const std::vector<int8_t>& msg, size_t size );

    asio::ip::udp::endpoint getEndpoint();

  private:
    /**
     * @brief Context for asynchronous operations
     */
    asio::io_context& context;

    /**
     * @brief The UDP socket used for communication
     */
    asio::ip::udp::socket socket;

    /**
     * @brief Buffer for storing incoming messages
     */
    std::array<int8_t, 1024> buff{};

    /**
     * @brief The last sender's endpoint (used for server)
     */
    asio::ip::udp::endpoint last_sender_;

    /**
     * @brief The server's endpoint (used for client)
     */
    asio::ip::udp::endpoint server_endpoint_;

    /**
     * @brief Flag indicating if the network manager is running
     */
    bool isrunning;

    /**
     * @brief Mutex for thread safety
     */
    std::mutex& mtx;

    /**
     * @brief List of pairs of clients and their last messages (used for server)
     */
    std::vector<std::pair<asio::ip::udp::endpoint, std::vector<int8_t>>>
        tmp_clients;
    std::vector<std::pair<asio::ip::udp::endpoint, std::vector<int8_t>>>&
        clients_lastmsg;

    /**
     * @brief Reference to the last message received from the server (used for client)
     */
    std::vector<int8_t> tmp_server;
    std::vector<int8_t> &lastmsg;
};

#endif /* !NETWORK_HPP_ */
