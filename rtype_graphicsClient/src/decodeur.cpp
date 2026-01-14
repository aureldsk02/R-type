/* ------------------------------------------------------------------------------------
 * *
 *                                                                                      *
 * EPITECH PROJECT - Wed, Sep, 2025 * Title           -
 * G-CPP-500-COT-5-1-rtype-8                                          *
 * Description     - * decodeur *
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

#include "../include/graphicsClient.hpp"

EnemyMovedResponse GraphicsClient::decodeEnemyMovedResponse(std::vector<int8_t>& buffer
) {
    std::vector<int8_t> tmp;

    {
        std::lock_guard<std::mutex> lock(mtx);

        tmp.insert(tmp.begin(), buffer.begin(), buffer.end() + 9);
    }

    if (tmp[0] != 0x37) {
        throw std::runtime_error("Invalid message type !");
    }

    {
        std::lock_guard<std::mutex> lock(mtx);

        buffer.erase(buffer.begin(), buffer.begin() + 9);
    }

    auto toInt16 = [](int8_t high, int8_t low) -> int16_t {
        return static_cast<int16_t>(
            (static_cast<uint8_t>(high) << 8) | static_cast<uint8_t>(low)
        );
    };

    EnemyMovedResponse pos;

    pos.type = tmp[0];
    pos.enemy_id = toInt16(tmp[1], tmp[2]);
    pos.enemy_type = static_cast<EnemyType>(toInt16(tmp[3], tmp[4]));
    pos.position.x = toInt16(tmp[5], tmp[6]);
    pos.position.y = toInt16(tmp[7], tmp[8]);

    // std::cout << "Enemy_Type: " << static_cast<int>(pos.enemy_type) << " ";
    // std::cout << "Enemy_Pos_x: " << pos.position.x << " ";
    // std::cout << "Enemy_Pos_y: " << pos.position.y << std::endl;

    return pos;
}

GameState GraphicsClient::decodeGameState(std::vector<int8_t>& buffer) {
    std::vector<int8_t> tmp;

    {
        std::lock_guard<std::mutex> lock(mtx);

        tmp.insert(tmp.begin(), buffer.begin(), buffer.begin() + 3);
    }

    if (tmp[0] != 0x40) {
        throw std::runtime_error("Invalid message type 0x38 !");
    }

    {
        std::lock_guard<std::mutex> lock(mtx);

        buffer.erase(buffer.begin(), buffer.begin() + 3);
    }

    GameState pos;

    pos.type = tmp[0];
    pos.gState = static_cast<GAMESTATE>((tmp[1] << 8) | tmp[2]);

    return pos;
}

MessageHeader GraphicsClient::decodeMessageHeader(std::vector<int8_t>& buffer) {
    std::vector<int8_t> tmp;

    {
        std::lock_guard<std::mutex> lock(mtx);
        tmp.insert(tmp.begin(), buffer.begin(), buffer.begin() + 15);
    }

    if (tmp[0] != 0x38) {
        throw std::runtime_error("Invalid message type 0x38 !");
    }

    {
        std::lock_guard<std::mutex> lock(mtx);
        buffer.erase(buffer.begin(), buffer.begin() + 15);
    }

    MessageHeader msg;
    size_t i = 0;

    msg.type = buffer[i++];

    msg.id = (static_cast<uint8_t>(tmp[i]) << 24) |
             (static_cast<uint8_t>(tmp[i+1]) << 16) |
             (static_cast<uint8_t>(tmp[i+2]) << 8) |
             (static_cast<uint8_t>(tmp[i+3]));
    i += 4;

    msg.nbr = (static_cast<uint8_t>(tmp[i]) << 8) |
              (static_cast<uint8_t>(tmp[i+1]));
    i += 2;

    msg.timeElapsed = 0;
    for (int j = 0; j < 8; ++j)
        msg.timeElapsed = (msg.timeElapsed << 8) | static_cast<uint8_t>(tmp[i++]);

    return msg;
}
