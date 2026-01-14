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
 *         ░        ░       ░░        ░        ░        ░░      ░░  ░░░░  ░ * ▒
 * ▒▒▒▒▒▒▒  ▒▒▒▒  ▒▒▒▒  ▒▒▒▒▒▒▒  ▒▒▒▒  ▒▒▒▒▒▒▒  ▒▒▒▒  ▒  ▒▒▒▒  ▒             *
 *         ▓      ▓▓▓       ▓▓▓▓▓  ▓▓▓▓▓▓▓  ▓▓▓▓      ▓▓▓  ▓▓▓▓▓▓▓        ▓ * █
 * ███████  ██████████  ███████  ████  ███████  ████  █  ████  █             *
 *         █        █  ███████        ████  ████        ██      ██  ████  █ *
 *                                                                                      *
 * ------------------------------------------------------------------------------------
 */

#include "../include/gameInstance.hpp"

std::vector<int8_t>
GameInstance::encodeEnemyMovedResponse(const EnemyMovedResponse& pos) {
    std::vector<int8_t> buffer;

    buffer.emplace_back(0x37);

    buffer.emplace_back((pos.enemy_id >> 8) & 0xFF);
    buffer.emplace_back(pos.enemy_id & 0xFF);

    buffer.emplace_back((pos.enemy_type >> 8) & 0xFF);
    buffer.emplace_back(pos.enemy_type & 0xFF);

    buffer.emplace_back((pos.position.x >> 8) & 0xFF);
    buffer.emplace_back(pos.position.x & 0xFF);

    buffer.emplace_back((pos.position.y >> 8) & 0xFF);
    buffer.emplace_back(pos.position.y & 0xFF);

    return buffer;
}

std::vector<int8_t>
GameInstance::encodeGameState(const GameState& pos) {
    std::vector<int8_t> buffer;

    buffer.emplace_back(0x40);

    buffer.emplace_back((pos.gState >> 8) & 0xFF);
    buffer.emplace_back(pos.gState & 0xFF);

    return buffer;
}

std::vector<int8_t> GameInstance::encodeMessageHeader(const MessageHeader& pos)
{
    std::vector<int8_t> buffer;

    // type
    buffer.push_back(pos.type);

    // id (uint32_t)
    buffer.push_back((pos.id >> 24) & 0xFF);
    buffer.push_back((pos.id >> 16) & 0xFF);
    buffer.push_back((pos.id >> 8)  & 0xFF);
    buffer.push_back(pos.id & 0xFF);

    // nbr (int16_t)
    buffer.push_back((pos.nbr >> 8) & 0xFF);
    buffer.push_back(pos.nbr & 0xFF);

    // timeElapsed (int64_t)
    for (int i = 7; i >= 0; --i)
        buffer.push_back((pos.timeElapsed >> (8 * i)) & 0xFF);

    return buffer;
}
