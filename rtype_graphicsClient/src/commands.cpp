/* ------------------------------------------------------------------------------------
 * *
 *                                                                                      *
 * EPITECH PROJECT - Wed, Sep, 2025 * Title           -
 * G-CPP-500-COT-5-1-rtype-8                                          *
 * Description     - * commands *
 *                                                                                      *
 * ------------------------------------------------------------------------------------
 * *
 *                                                                                      *
 *             ███████╗██████╗  ██╗████████╗███████╗ ██████╗ ██╗  ██╗ *
 *             ██╔════╝██╔══██║ ██║╚══██╔══╝██╔════╝ ██╔═══╝ ██║  ██║ *
 *             █████╗  ██████╔╝ ██║   ██║   █████╗   ██║     ███████║                     *
 *             ██╔══╝  ██╔═══╝  ██║   ██║   ██╔══╝   ██║     ██╔══██║                     *
 *             ███████╗██║      ██║   ██║   ███████╗ ██████╗ ██║  ██║ *
 *             ╚══════╝╚═╝      ╚═╝   ╚═╝   ╚══════╝ ╚═════╝ ╚═╝  ╚═╝ *
 *                                                                                      *
 * ------------------------------------------------------------------------------------
 */

#include "../include/graphicsClient.hpp"

std::vector<int8_t> GraphicsClient::encodeMoveResponse(const MoveResponse& pos) {
    std::vector<int8_t> buffer;

    buffer.emplace_back(pos.type);

    buffer.emplace_back((pos.player_id >> 8) & 0xFF);
    buffer.emplace_back(pos.player_id & 0xFF);

    buffer.emplace_back((pos.direction >> 8) & 0xFF);
    buffer.emplace_back(pos.direction & 0xFF);

    return buffer;
}

std::vector<int8_t> GraphicsClient::encodeActionResponse(const ActionResponse& pos) {
    std::vector<int8_t> buffer;

    buffer.emplace_back(pos.type);

    buffer.emplace_back((pos.player_id >> 8) & 0xFF);
    buffer.emplace_back(pos.player_id & 0xFF);

    buffer.emplace_back((pos.input >> 8) & 0xFF);
    buffer.emplace_back(pos.input & 0xFF);

    return buffer;
}
