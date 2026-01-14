/* ------------------------------------------------------------------------------------
 * *
 *                                                                                      *
 * EPITECH PROJECT - Sat, Sep, 2025 * Title           -
 * G-CPP-500-COT-5-1-rtype-8                                          *
 * Description     - * level_parser *
 *                                                                                      *
 * ------------------------------------------------------------------------------------
 * *
 *                                                                                      *
 *       ▄▀▀█▄▄▄▄  ▄▀▀▄▀▀▀▄  ▄▀▀█▀▄    ▄▀▀▀█▀▀▄  ▄▀▀█▄▄▄▄  ▄▀▄▄▄▄   ▄▀▀▄ ▄▄ * ▐
 * ▄▀   ▐ █   █   █ █   █  █  █    █  ▐ ▐  ▄▀   ▐ █ █    ▌ █  █   ▄▀ * █▄▄▄▄▄  ▐
 * █▀▀▀▀  ▐   █  ▐  ▐   █       █▄▄▄▄▄  ▐ █      ▐  █▄▄▄█             * █    ▌
 * █          █        █        █    ▌    █         █   █             * ▄▀▄▄▄▄
 * ▄▀        ▄▀▀▀▀▀▄   ▄▀        ▄▀▄▄▄▄    ▄▀▄▄▄▄▀   ▄▀  ▄▀             * █    ▐
 * █         █       █ █          █    ▐   █     ▐   █   █               * ▐ ▐
 * ▐       ▐ ▐          ▐        ▐         ▐   ▐               *
 *                                                                                      *
 * ------------------------------------------------------------------------------------
 */

/**
 * @file level_handling.cpp
 * @author Aurel DOSSOU KOHI
 * @brief This file contains the definition of the level handling functions
 * @version 0.1
 * @date 2025-10-13
 *
 * @copyright Copyright (c) 2026 - Aurel DOSSOU KOHI
 *
 */

#include <libconfig.h++>

#include "Factory.hpp"
#include "logic_functions.hpp"
#include "./../include/gameInstance.hpp"

using namespace component;

using namespace libconfig;

#define LEVEL1_EASY "assets/levels/easy.txt"
#define LEVEL2_EASY "assets/levels/easy_2.txt"
#define LEVEL3_EASY "assets/levels/easy_3.txt"

#define LEVEL1_MEDIUM "assets/levels/medium.txt"
#define LEVEL2_MEDIUM "assets/levels/medium_2.txt"
#define LEVEL3_MEDIUM "assets/levels/medium_3.txt"

#define LEVEL1_HARD "assets/levels/hard.txt"
#define LEVEL2_HARD "assets/levels/hard_2.txt"
#define LEVEL3_HARD "assets/levels/hard_3.txt"

bool all_entities_spawned = false;

/**
 * @brief This function loads a level from a configuration file
 * It stores all the entities of the level in a vector with their spawn time
 * and spawn y position
 *
 * @param path
 */
void GameInstance::loadLevel() {
    if (diff_mode == PVP) {
        return;
    }

    Config conf;
    if (state == LEVEL1) {
        if (diff_mode == EASY)
            conf.readFile(LEVEL1_EASY);
        if (diff_mode == MEDIUM)
            conf.readFile(LEVEL1_MEDIUM);
        if (diff_mode == HARD)
            conf.readFile(LEVEL1_HARD);
    } else if (state == LEVEL2) {
        if (diff_mode == EASY)
            conf.readFile(LEVEL2_EASY);
        if (diff_mode == MEDIUM)
            conf.readFile(LEVEL2_MEDIUM);
        if (diff_mode == HARD)
            conf.readFile(LEVEL2_HARD);
    } else if (state == LEVEL3) {
        if (diff_mode == EASY)
            conf.readFile(LEVEL3_EASY);
        if (diff_mode == MEDIUM)
            conf.readFile(LEVEL3_MEDIUM);
        if (diff_mode == HARD)
            conf.readFile(LEVEL3_HARD);
    } else if (state == CUSTOM_LEVEL) {
        try {
            conf.readFile(custom_conf_path.c_str());
        } catch (const FileIOException& fioex) {
            std::cerr << "I/O error while reading file." << std::endl;
            exit(84);
        }
    }

    Setting& root = conf.getRoot();

    Setting& level_entities = root.lookup("entities");

    entities.clear();

    for (auto& entity : level_entities) {
        entity_info_t info;
        info.type = entity["name"].c_str();
        info.spawn_time = entity["spawn_time"];
        info.spawn_y = entity["y"];
        entities.push_back(info);
    }
    // initializePlayers();
    levelTimer = -2.0;
}

/**
 * @brief This functions send to the clients information about each game entity
 * alive
 *
 */
void GameInstance::logGameEntities() {
    std::vector<std::string> special_entities = {
        "background", "menu_background_music"
    };
    int real_entities = 0;

    counter = 0;
    result.clear();

    for (size_t i = 0; i < reg.getEntityNum(); i++) {
        try {
            position& pos =
                reg.get_components<component::position>()[entity(i)].value();
            name& name_ =
                reg.get_components<component::name>()[entity(i)].value();
            EnemyType type = type_map[name_._name];
            unique_id& uid =
                reg.get_components<component::unique_id>()[entity(i)].value();

            if (name_._name == "boss") {
                hurtbox& hb = reg.get_components<hurtbox>()[entity(i)].value();
             
                if (hb.health <= 0) {
                    Factory fac(reg);
                    entity explosion = fac.make_explosion();
                    position& explosion_pos =
                        reg.get_components<position>()[explosion].value();
                    explosion_pos.x = pos.x;
                    explosion_pos.y = pos.y;

                    boss_dead = true;
                    reg.kill_entity(entity(i));
                    continue;
                }
            }
            if (name_._name == "small_shooter") {
                hurtbox& hb = reg.get_components<hurtbox>()[entity(i)].value();
            
                if (hb.health <= 0) {
                    Factory fac(reg);
                    entity explosion = fac.make_explosion();
                    position& explosion_pos =
                        reg.get_components<position>()[explosion].value();
                    explosion_pos.x = pos.x;
                    explosion_pos.y = pos.y;

                    boss1_dead = true;
                    reg.kill_entity(entity(i));
                    continue;
                }
            }
            if (name_._name == "big_shooter") {
                hurtbox& hb = reg.get_components<hurtbox>()[entity(i)].value();
            
                if (hb.health <= 0) {
                    Factory fac(reg);
                    entity explosion = fac.make_explosion();
                    position& explosion_pos =
                        reg.get_components<position>()[explosion].value();
                    explosion_pos.x = pos.x;
                    explosion_pos.y = pos.y;

                    boss2_dead = true;
                    reg.kill_entity(entity(i));
                    continue;
                }
            }
            if (name_._name == "final_boss") {
                hurtbox& hb = reg.get_components<hurtbox>()[entity(i)].value();
            
                if (hb.health <= 0) {
                    Factory fac(reg);
                    entity explosion = fac.make_explosion();
                    position& explosion_pos =
                        reg.get_components<position>()[explosion].value();
                    explosion_pos.x = pos.x;
                    explosion_pos.y = pos.y;

                    final_boss_dead = true;
                    reg.kill_entity(entity(i));
                    continue;
                }
            }

            // Ignore special entities
            if (std::find(
                    special_entities.begin(), special_entities.end(),
                    name_._name
                ) != special_entities.end()) {
                continue;
            }

            // Clean up out of screen entities
            if (diff_mode != PVP) {
                if (pos.x < -200 || pos.x > 1000 || pos.y < -200 || pos.y > 1000) {
                    if (name_._name != "ceiling" && name_._name != "floor") {
                        reg.kill_entity(entity(i));
                        continue;
                    }
                }
            }
            if (name_._name != "player2" && name_._name != "player1"
                && name_._name != "ceiling" && name_._name != "floor" && name_._name != "force") {
                real_entities++;
            }

            ++counter;

            vector<int8_t> tmp = encodeEnemyMovedResponse(
                {0x37,
                 static_cast<int16_t>(uid),
                 type,
                 {static_cast<int16_t>(pos.x), static_cast<int16_t>(pos.y)}}
            );
            result.insert(result.end(), tmp.begin(), tmp.end());
        } catch (...) {
        }
    }
    auto now = std::chrono::steady_clock::now();

    vector<int8_t> tmp = encodeMessageHeader(
        {0x38,
            packageId,
            static_cast<int16_t>(counter),
            std::chrono::duration_cast<std::chrono::milliseconds>(now - gameStarted).count()
        });
    result.insert(result.begin(), tmp.begin(), tmp.end());

    if (!result.empty()) {
        for (auto & tmp : all_clients) {
            server_.send_to_client(result, result.size(), tmp.first);
        }
    }

    if (real_entities == 0 && all_entities_spawned) {
        printf("Victory\n");
        win.close();
    }
}

/**
 * @brief This function handles the received players' inputs
 *
 * @param delta
 */
void GameInstance::receivePlayerInput(double delta) {
    static double shoot_timer = 0;

    shoot_timer += delta;
    size_t msg_size = 0;

    {
        std::lock_guard<std::mutex> lock(mtx);
        msg_size = messages.size();
    }

    while (msg_size != 0) {

        std::cout << "Message SIZE: " << msg_size << std::endl;

        std::pair<asio::ip::udp::endpoint, std::vector<int8_t>> msg;

        {
            std::lock_guard<std::mutex> lock(mtx);
            msg = messages.front();
            messages.erase(messages.begin());
            msg_size = messages.size();
        }

        std::vector<int8_t> tmp = msg.second;

        for (auto a : tmp) {
            std::cout << " " << std::to_string(a);
        }
        std::cout << std::endl;

        try {
            if (tmp[0] == 0x24) {

                velocity& vel = reg.get_components<component::velocity>(
                )[all_clients[msg.first]]
                                    .value();
                MoveResponse move_info = decodeMoveResponse(tmp);

                if (move_info.direction == LEFT) {
                    vel.vx = -PLAYER_SPEED;
                }
                if (move_info.direction == RIGHT) {
                    vel.vx = PLAYER_SPEED;
                }
                if (move_info.direction == UP) {
                    vel.vy = -PLAYER_SPEED;
                }
                if (move_info.direction == DOWN) {
                    vel.vy = PLAYER_SPEED;
                }
            }

            if (tmp[0] == 0x25) {

                ActionResponse action_info = decodeActionResponse(tmp);

                if (action_info.input == SPACE && shoot_timer > PLAYER_SHOOT_COOLDOWN) {
                    shoot_timer = 0;
                    position &pos = reg.get_components<component::position>()[all_clients[msg.first]].value();
                    if (diff_mode == PVP) {
                        name& name_ = reg.get_components<name>()[all_clients[msg.first]].value();
                        if (name_._name == "player1") {
                            entity missile = factory.make_player_missile();
                            position &missile_pos = reg.get_components<component::position>()[missile].value();
                            missile_pos.x = pos.x + 50;
                            missile_pos.y = pos.y + 6;
                        }
                        else {
                            std::cout << "EVIL MISSILE SPAWN" << std::endl;
                            entity missile = factory.make_evil_player_missile();
                            position &missile_pos = reg.get_components<component::position>()[missile].value();
                            missile_pos.x = pos.x - 10;
                            missile_pos.y = pos.y + 6;
                        }
                        
                    } else {
                        entity missile = factory.make_player_missile();
                        position &pos = reg.get_components<component::position>()[all_clients[msg.first]].value();
                        position &missile_pos = reg.get_components<component::position>()[missile].value();
                        missile_pos.x = pos.x + 8;
                        missile_pos.y = pos.y + 6;
                    }
                }
            }

        } catch (...) {}

    }
}

/**
 * @brief This function runs the current level
 * It makes sure to spawn new entities if their spawn time has come
 * And calls the receivePlayerInput function as well as the logGameEntities
 * function
 * @param delta The amount of time elapsed since the last frame
 */
void GameInstance::runLevel(double delta) {
    levelTimer += delta;
    int unspawned_entities = 0;

    // Spawn new ready entities
    for (auto it = entities.begin(); it != entities.end(); it++) {
        auto& en = *it;

        if (en.entity_id.getId() == -1) {
            unspawned_entities++;
            if (en.spawn_time <= levelTimer) {
                en.entity_id = factory.make_entity(en.type);
                if (en.entity_id.getId() == -1) {
                    throw std::runtime_error("Failed to spawn entity");
                }
                auto& pos =
                    reg.get_components<component::position>()[en.entity_id].value();
                pos.y = en.spawn_y;
                pos.x = 1000;
            }
        }
    }

    if (unspawned_entities == 0 && state == CUSTOM_LEVEL) {
        all_entities_spawned = true;
    }

    logGameEntities();
    receivePlayerInput(delta);
}

void GameInstance::handleWinOrLoss() {
    if (std::all_of(all_clients.begin(), all_clients.end(),
        [] (auto& elem) {
                return elem.second == -1;
            }
        )
    ) {
        printf("GAME OVER\n");
        win.close();
    }
    
    if (state == LEVEL1) {
        if (boss_dead) {
            printf("BOSS DEAD\n");
            state = LEVEL2;
            loadLevel();
        }
    }
    if (state == LEVEL2) {
        if (boss2_dead && boss1_dead) {
            printf("BOSS2 and BOSS1 DEAD\n");
            state = LEVEL3;
            loadLevel();
        }
    }
    if (state == LEVEL3) {
        if (final_boss_dead) {
            printf("FINAL BOSS DEAD\n");
            win.close();
            loadLevel();
        }
    }

    if (diff_mode == PVP) {
        if (all_clients.begin()->second == -1) {
            printf("PLAYER 2 WON\n");
        } else {
            printf("PLAYER 1 WON\n");
        }
    }
}

void GameInstance::updatePlayerHealth(void)
{
    for (auto& [endpoint, en] : all_clients) {
        if (en == -1) {
            continue;
        }
        hurtbox& hb = reg.get_components<hurtbox>()[en].value();
        if (hb.health <= 0) {
            entity explosion = factory.make_explosion();
            position& pos = reg.get_components<position>()[en].value();
            position& explosion_pos =
            reg.get_components<position>()[explosion].value();
            explosion_pos.x = pos.x;
            explosion_pos.y = pos.y;
            
            name& name_ = reg.get_components<name>()[en].value();

            int i = 0;
            
            reg.kill_entity((entity)en);
            for (auto& player : all_clients) {
                if (player.second == (size_t)en) {
                    player.second = -1;
                    printf("Player %d dead\n", i + 1);
                    break;
                }
                i++;
            }
        }
    }
}
