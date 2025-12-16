/*
** EPITECH PROJECT, 2025
** G-CPP-500-COT-5-1-rtype-20
** File description:
** Broadcast
*/

#include "../include/Broadcast.hpp"

Broadcast::Broadcast()
{
}

Broadcast::~Broadcast()
{
}

void Broadcast::start()
{
    broadcaster.set_option(asio::socket_base::broadcast(true));
}

void Broadcast::send_broadcast_()
{
    BroadcastPacket packet;
    packet.header.magic = MAGIC_NUMBER;
    packet.header.version = PROTOCOL_VERSION;
    packet.header.type = BROADCAST;
    udp::endpoint broadcast_endpoint(asio::ip::address_v4::broadcast(), 12345);
    char msg[sizeof(packet)];
    std::memcpy(msg, &packet, sizeof(packet));
    broadcaster.send_to(asio::buffer(msg, sizeof(msg)), broadcast_endpoint);
}