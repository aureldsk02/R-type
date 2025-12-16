/*
** EPITECH PROJECT, 2025
** G-CPP-500-COT-5-1-rtype-20
** File description:
** Broadcast
*/

#ifndef BROADCAST_HPP_
#define BROADCAST_HPP_
#include <iostream>
#include <asio.hpp>
#include "Networkdata.h"
using asio::ip::udp;
class Broadcast {
    protected:
        asio::io_context io_context;
        udp::socket broadcaster{io_context, udp::v4()};
    public:
        Broadcast();
        ~Broadcast();
        void start();
        void send_broadcast_();
    private:
};

#endif /* !BROADCAST_HPP_ */
