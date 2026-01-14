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
 *       _|_|_|_|  _|_|_|    _|_|_|  _|_|_|_|_|  _|_|_|_|    _|_|_|  _|    _| *
 *       _|        _|    _|    _|        _|      _|        _|        _|    _| *
 *       _|_|_|    _|_|_|      _|        _|      _|_|_|    _|        _|_|_|_| *
 *       _|        _|          _|        _|      _|        _|        _|    _| *
 *       _|_|_|_|  _|        _|_|_|      _|      _|_|_|_|    _|_|_|  _|    _| *
 *                                                                                      *
 * ------------------------------------------------------------------------------------
 */

#include "../include/gameInstance.hpp"

MoveResponse GameInstance::decodeMoveResponse(std::vector<int8_t>& buffer) {

    std::vector<int8_t> tmp;

    tmp.insert(tmp.begin(), buffer.begin(), buffer.begin() + 5);

    if (tmp[0] != 0x24) {
        throw std::runtime_error("Type de message invalide !");
    }

    buffer.clear();

    MoveResponse pos;

    pos.type = tmp[0];
    pos.player_id = (tmp[1] << 8) | tmp[2];
    pos.direction = static_cast<Direction>((tmp[3] << 8) | tmp[4]);

    return pos;
}

ActionResponse GameInstance::decodeActionResponse(std::vector<int8_t>& buffer) {

    std::vector<int8_t> tmp;

    tmp.insert(tmp.begin(), buffer.begin(), buffer.begin() + 5);

    if (tmp[0] != 0x25) {
        throw std::runtime_error("Type de message invalide !");
    }

    buffer.clear();

    ActionResponse pos;

    pos.type = tmp[0];
    pos.player_id = (tmp[1] << 8) | tmp[2];
    pos.input = static_cast<Action>((tmp[3] << 8) | tmp[4]);

    return pos;
}
