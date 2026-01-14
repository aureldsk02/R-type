/*
** EPITECH PROJECT, 2025
** G-CPP-500-COT-5-1-rtype-8
** File description:
** init_client
*/

/**
 * @file init_client.cpp
 * @author Aurel DOSSOU KOHI
 * @brief This file contains the definition of the client functions
 * @version 0.1
 * @date 2025-10-12
 *
 * @copyright Copyright (c) 2026 - Aurel DOSSOU KOHI
 *
 */

#include "../include/graphicsClient.hpp"
#include "components.hpp"

bool boss_dead = false;

bool boss1_dead = false;
bool boss2_dead = false;

bool final_boss_dead = false;

/**
 * @brief This function uses the ResourceManager to pre-load textures
 * and fonts that will be used in the game
 */
void load_client_textures(void) {
    ResourceManager::Instance().load(
        "assets/sprites/player/player1.gif", "player1", TEXTURE
    );
    ResourceManager::Instance().load(
        "assets/sprites/player/player2.gif", "player2", TEXTURE
    );
    ResourceManager::Instance().load(
        "assets/sprites/player/player3.gif", "player3", TEXTURE
    );
    ResourceManager::Instance().load(
        "assets/sprites/player/player4.gif", "player4", TEXTURE
    );
    ResourceManager::Instance().load(
        "assets/sprites/player/player2_flipped.gif", "player2_flipped", TEXTURE
    );
    ResourceManager::Instance().load(
        "assets/sprites/player/player_missile_flipped.gif", "player_missile_flipped", TEXTURE
    );
    ResourceManager::Instance().load(
        "assets/sprites/player/player_up.gif", "player_up", TEXTURE
    );
    ResourceManager::Instance().load(
        "assets/sprites/player/player_down.gif", "player_down", TEXTURE
    );
    ResourceManager::Instance().load(
        "assets/sprites/player/player_missile.gif", "player_missile", TEXTURE
    );
    ResourceManager::Instance().load(
        "assets/sprites/enemies/red_trooper.gif", "red_trooper", TEXTURE
    );
    ResourceManager::Instance().load(
        "assets/sprites/enemies/walker_walk.gif", "walker", TEXTURE
    );
    ResourceManager::Instance().load(
        "assets/sprites/enemies/enemy_missile.gif", "enemy_missile", TEXTURE
    );

    ResourceManager::Instance().load(
        "assets/sprites/enemies/plane.gif", "plane", TEXTURE
    );

    ResourceManager::Instance().load(
        "assets/sprites/effects/Explosion.png", "explosion", TEXTURE
    );
    ResourceManager::Instance().load(
        "assets/sprites/effects/hit_effect.gif", "hit_effect", TEXTURE
    );
    ResourceManager::Instance().load(
        "assets/sprites/background/background.jpg", "background", TEXTURE
    );
    ResourceManager::Instance().load(
        "assets/sprites/background/wall1_shadow.png", "ceiling", TEXTURE
    );
    ResourceManager::Instance().load(
        "assets/sprites/enemies/boss.gif", "boss", TEXTURE
    );
    ResourceManager::Instance().load(
        "assets/sprites/player/force.gif", "force", TEXTURE
    );

    ResourceManager::Instance().load(
        "assets/fonts/ARCADECLASSIC.TTF", "arcade", FONT
    );

    ResourceManager::Instance().load(
        "assets/sprites/enemies/enemy_jet.gif", "green_trooper", TEXTURE
    );

    ResourceManager::Instance().load(
        "assets/sprites/enemies/tourelles.gif", "tourelles", TEXTURE
    );
    ResourceManager::Instance().load(
        "assets/sprites/enemies/boss_big_shooter.gif", "big_shooter", TEXTURE
    );
    ResourceManager::Instance().load(
        "assets/sprites/enemies/boss_small_shooter.gif", "small_shooter", TEXTURE
    );
    ResourceManager::Instance().load(
        "assets/sprites/enemies/final_boss.gif", "final_boss", TEXTURE
    );
    ResourceManager::Instance().load(
        "assets/sprites/enemies/big_missile.gif", "big_missile", TEXTURE
    );

    ResourceManager::Instance().load(
        "assets/sprites/enemies/space_enemies.gif", "space_enemy", TEXTURE
    );
}

/**
 * @brief This function returns the name of an entity type
 *
 * @param value the entity type
 * @return std::string
 */
std::string getKey(int value) {
    for (const auto& pair : type_map) {
        if (pair.second == value)
            return pair.first;
    }
    return "";
}

/**
 * @brief Construct a new GraphicsClient:: Client object
 *
 * @param p The port to connect to
 * @param address The server address
 */
GraphicsClient::GraphicsClient (NetworkManager& client, std::vector<int8_t>& _lastmsg, std::mutex& _mtx, std::vector<sf::Keyboard::Key> keyTab):
    client_(client), lastmsg(_lastmsg), mtx(_mtx), reg(win), factory(reg)
{
    state = LEVEL1;
    substate = LEVEL1_START;
    isRunning_ = false;

    reg.toggleMovement();
    reg.toggleRendering();
    reg.toggleControl();

    std::vector<int8_t> msg(1, 0x5);
    client_.send_to_server(msg, msg.size());

    load_client_textures();
    // initMenu();
    initGame(keyTab);
}

bool GraphicsClient::isRunning() const {
    return isRunning_;
}

/**
 * @brief A utility function to check in an entity of a certain id is inside a
 * vector
 *
 * @param vec A vector of entities informations
 * @param id The id of the entity to search
 * @return true if the entity is in the vector
 * @return false if the entity is not in the vector
 */
bool isInside(std::vector<EnemyMovedResponse> vec, size_t id) {
    for (auto it = vec.begin(); it != vec.end(); it++) {
        if (it->enemy_id == id)
            return true;
    }
    return false;
}

/**
 * @brief This function checks if the player is entering inputs and sends info
 * to the server accordingly
 */
void GraphicsClient::sendPlayerInput() {
    component::controllable& con =
        reg.get_components<component::controllable>()[controllable_id].value(
        );

    if (!con.left && !con.right && !con.up && !con.down)
        return;

    MoveResponse pos;

    pos.type = 0x24;
    pos.player_id = static_cast<int16_t>(-1);

    if (con.left) {
        pos.direction = LEFT;
    } else if (con.right) {
        pos.direction = RIGHT;
    } else if (con.up) {
        pos.direction = UP;
    } else if (con.down) {
        pos.direction = DOWN;
    }

    std::vector<int8_t> buff = encodeMoveResponse(pos);

    client_.send_to_server(buff, buff.size());
}

/**
 * @brief This function checks if the player is shooting and sends info to the
 * server accordingly
 *
 */
void GraphicsClient::sendPlayerAction() {
    component::controllable& con =
        reg.get_components<component::controllable>()[controllable_id].value(
        );

    if (!con.space)
        return;

    ActionResponse pos;

    pos.type = 0x25;
    pos.player_id = static_cast<int16_t>(-1);

    if (con.space) {
        pos.input = SPACE;
    }

    std::vector<int8_t> buff = encodeActionResponse(pos);

    client_.send_to_server(buff, buff.size());
}

void GraphicsClient::receiveServerInfo() {
    bool isempty;

    {
        std::lock_guard<std::mutex> lock(mtx);
        isempty = lastmsg.empty();
    }

    if (isempty)
        return;
    //// ajouter la condition pour checker l'ordre de message les messages perdus et le reste
    MessageHeader e = decodeMessageHeader(lastmsg);
    std::vector<EnemyMovedResponse> s;

    for (int a = 0; a < e.nbr; a++) {
        s.push_back(decodeEnemyMovedResponse(lastmsg));
    }

    if (entity_states.empty()) {
        entity_states.push_back({e.timeElapsed, s});
        return;
    }


    if (std::find_if(entity_states.begin(), entity_states.end(), 
        [&e](std::pair<int64_t, std::vector<EnemyMovedResponse>>&tmp) {
            return e.timeElapsed == tmp.first;
        }) != entity_states.end())
        return;

    for (int i = 0; i < entity_states.size() - 1; i++) {
        if (entity_states[i].first < e.timeElapsed && entity_states[i + 1].first > e.timeElapsed){
            entity_states.insert(entity_states.begin() + i - 1, {e.timeElapsed, s});
            return;
        }
    }

    entity_states.push_back({e.timeElapsed, s});
}

void GraphicsClient::entityMoveInterpole(EnemyMovedResponse pastPos, int64_t pastTime, int64_t now, EnemyMovedResponse nextPos, int64_t nextTime) {

    static auto progLinear = [](
        auto duration_to_now, auto totalDuration,
        int start, int end) -> double
    {
        double f_start = static_cast<double>(start);
        double f_end = static_cast<double>(end);
        return ( ( (f_end - f_start) / totalDuration ) * duration_to_now ) + f_start;
    };

    auto totalDuration = nextTime - pastTime;
    auto nowDuration = now - pastTime;

    Vector2D newPos = {
        static_cast<int16_t>(progLinear(nowDuration, totalDuration, pastPos.position.x, nextPos.position.x)),
        static_cast<int16_t>(progLinear(nowDuration, totalDuration, pastPos.position.y, nextPos.position.y))
    };

    if (ids_assoc.find(pastPos.enemy_id) == ids_assoc.end()) {
        auto new_entity = factory.make_entity(getKey(pastPos.enemy_type));
        ids_assoc[pastPos.enemy_id] = new_entity;
    }

    std::cerr << "Entity ID: " << pastPos.enemy_id << " ENTITY: " << ids_assoc.at(pastPos.enemy_id) << std::endl;

    if (ids_assoc.find(pastPos.enemy_id) != ids_assoc.end()) {
        try {
            auto& pos = reg.get_components<component::position>(
                )[ids_assoc.at(pastPos.enemy_id)]
                                        .value();
            pos.x = newPos.x;
            pos.y = newPos.y;
        } catch (...) {}
    }
}

void GraphicsClient::entityMovDelete(EnemyMovedResponse pastpastPos, int64_t pastpastTime, int64_t now, EnemyMovedResponse pastPos, int64_t pastTime) {

    if (now - pastTime > 25) {
        if (ids_assoc.find(pastPos.enemy_id) != ids_assoc.end()) {
            try {
                reg.kill_entity((class entity)(ids_assoc.at(pastPos.enemy_id)));
            } catch (...) {}
            ids_assoc.erase(pastPos.enemy_id);
            if (getKey(pastPos.enemy_type) == "player1") {
                player1_dead = true;
            }
            if (getKey(pastPos.enemy_type) == "player2") {
                player2_dead = true;
            }
            if (getKey(pastPos.enemy_type) == "player3") {
                player3_dead = true;
            }
            if (getKey(pastPos.enemy_type) == "player4") {
                player4_dead = true;
            }
            if (getKey(pastPos.enemy_type) == "boss") {
                boss_dead = true;
            }
            if (getKey(pastPos.enemy_type) == "small_shooter") {
                boss2_dead = true;
            }
            if (getKey(pastPos.enemy_type) == "big_shooter") {
                boss1_dead = true;
            }
            if (getKey(pastPos.enemy_type) == "final_boss") {
                final_boss_dead = true;
            }
        }
        return;
    }

    static auto progLinear = [](
        auto duration_to_now, auto totalDuration,
        int start, int end) -> double
    {
        double f_start = static_cast<double>(start);
        double f_end = static_cast<double>(end);
        return ( ( (f_end - f_start) / totalDuration ) * duration_to_now ) + f_start;
    };

    auto totalDuration = pastTime - pastpastTime;
    auto nowDuration = now - pastpastTime;

    Vector2D newPos = {
        static_cast<int16_t>(progLinear(nowDuration, totalDuration, pastpastPos.position.x, pastPos.position.x)),
        static_cast<int16_t>(progLinear(nowDuration, totalDuration, pastpastPos.position.y, pastPos.position.y))
    };

    if (ids_assoc.find(pastPos.enemy_id) == ids_assoc.end()) {
        auto new_entity = factory.make_entity(getKey(pastPos.enemy_type));
        ids_assoc[pastPos.enemy_id] = new_entity;
    }

    try {
        auto& pos = reg.get_components<component::position>(
                )[ids_assoc.at(pastPos.enemy_id)]
                                    .value();
        pos.x = newPos.x;
        pos.y = newPos.y;
    }  catch(const std::exception& e) {}
}

void GraphicsClient::entityMovCreate(EnemyMovedResponse nextPos, int64_t nextTime, int64_t now, EnemyMovedResponse nextnextPos, int64_t nextnextTime) {
    if (nextTime - now > 25)
        return;

    static auto progLinear = [](
        auto duration_to_now, auto totalDuration,
        int start, int end) -> double
    {
        double f_start = static_cast<double>(start);
        double f_end = static_cast<double>(end);
        return ( ( (f_end - f_start) / totalDuration ) * duration_to_now ) + f_start;
    };

    auto totalDuration = nextnextTime - nextTime;
    auto nowDuration = now - nextTime;

    Vector2D newPos = {
        static_cast<int16_t>(progLinear(nowDuration, totalDuration, nextPos.position.x, nextnextPos.position.x)),
        static_cast<int16_t>(progLinear(nowDuration, totalDuration, nextPos.position.y, nextnextPos.position.y))
    };

    if (ids_assoc.find(nextPos.enemy_id) == ids_assoc.end()) {
        auto new_entity = factory.make_entity(getKey(nextPos.enemy_type));
        ids_assoc[nextPos.enemy_id] = new_entity;
    }

    try {
        auto& pos = reg.get_components<component::position>(
                )[ids_assoc.at(nextPos.enemy_id)]
                                    .value();
        pos.x = newPos.x;
        pos.y = newPos.y;
    } catch(const std::exception& e) {}

}

/**
 * @brief This function updates the game state. It gets all the entities sent by
 * the server and checks for changes. If an entity is new, it creates a new
 * entity in the registry. If an entity is updated, it updates the entity in the
 * registry. If an entity is removed, it removes the entity from the registry.
 *
 * @param delta the time since the last update
 */
void GraphicsClient::runLevel(double delta) {

    using namespace std::chrono;

    int64_t nowDuration = duration_cast<milliseconds>(steady_clock::now() - clientStarted).count();

    size_t pastIdx = 0;
    size_t nextIdx = 0;

    std::cout << "Now: " << nowDuration << std::endl;

    static int64_t latence = 0;

    if (latence == 0 && entity_states.size() == 1) {
        latence = entity_states.front().first - nowDuration - 100;
    }
    std::cout << "SIZE: " << entity_states.size() << std::endl;
    if (entity_states.size() < 2)
        return;

    //////////////////////////

    std::cout << "Latence: " << latence << std::endl;
    std::cout << "Now + Latence: " << nowDuration + latence << std::endl;
    std::cout << "First Time: " << entity_states.front().first << std::endl;
    std::cout << "Last Time: " << entity_states.back().first << std::endl;

    if (entity_states.front().first > nowDuration + latence)
        return;
    else if (nowDuration + latence > entity_states.back().first) {
        if (nowDuration + latence > entity_states.back().first + 2500)
            win.close();
        else
            return;
    }

    //////////////////////////

    for (size_t i = 0; i < entity_states.size() - 1; i++) {
        if (entity_states[i].first <= nowDuration + latence && nowDuration + latence <= entity_states[i + 1].first) {
            std::cout << "PastTime: " << entity_states[i].first << " NextTime: " << entity_states[i + 1].first << std::endl;
            if (i == 0 || i == 1) {
                pastIdx = i;
                nextIdx = i + 1;
            } else if (i > 1) {
                entity_states.erase(entity_states.begin(), entity_states.begin() + i - 1);
                pastIdx = 1;
                nextIdx = 2;
            }
            break;
        }
    }

    std::pair<
            int64_t,
            std::vector<EnemyMovedResponse>
            >& pastInfo = entity_states.at(pastIdx);
    std::pair<
            int64_t,
            std::vector<EnemyMovedResponse>
            >& nextInfo = entity_states.at(nextIdx);

    for (auto pastEntity : pastInfo.second) {
        // Vérification de la présence de l'entité de pastInfo dans nextInfo
        auto nextEntity = find_if(nextInfo.second.begin(), nextInfo.second.end(),
            [&pastEntity](EnemyMovedResponse& ent) {
                return ent.enemy_id == pastEntity.enemy_id;
            });
        if (nextEntity != nextInfo.second.end()) {
            // interpolation entre pastEntity et nextEntity
            std::cerr << "INTERPOLATION " << nowDuration + latence << "\n";
            entityMoveInterpole(pastEntity, pastInfo.first,
                nowDuration + latence, *nextEntity, nextInfo.first);
        }
        if (nextEntity == nextInfo.second.end()) {
            /// Supression
            /// Interpolation legere puis supression

            if (pastIdx >= 1) {
                std::pair<
                    int64_t,
                    std::vector<EnemyMovedResponse>
                >& pastpastInfo = entity_states.at(pastIdx - 1);

                auto pastpastEntity = std::find_if(pastpastInfo.second.begin(), pastpastInfo.second.end(),
                    [&pastEntity](EnemyMovedResponse& ent) {
                            return ent.enemy_id == pastEntity.enemy_id;
                        });
                if (pastpastEntity == pastInfo.second.end())
                    return;
                std::cerr << "DELETE " << nowDuration + latence << "\n";
                entityMovDelete(*pastpastEntity, pastpastInfo.first, nowDuration + latence, pastEntity, pastInfo.first);
            }

        }
    }

    for (auto nextEntity : nextInfo.second) {
        // Vérification de la présence de l'entité de pastInfo dans nextInfo
        auto pastEntity = find_if(pastInfo.second.begin(), pastInfo.second.end(),
            [&nextEntity](EnemyMovedResponse& ent) {
                return ent.enemy_id == nextEntity.enemy_id;
            });
        if (pastEntity == pastInfo.second.end()) {
            /// Creation
            /// Création puis extrapolation légère

            if (nextIdx < entity_states.size() - 1) {
                std::pair<
                    int64_t,
                    std::vector<EnemyMovedResponse>
                >& nextnextInfo = entity_states.at(nextIdx + 1);

                auto nextnextEntity = std::find_if(nextnextInfo.second.begin(), nextnextInfo.second.end(),
                [&nextEntity](EnemyMovedResponse& ent) {
                    return ent.enemy_id == nextEntity.enemy_id;
                });
                if (nextnextEntity == nextnextInfo.second.end())
                    continue;
                std::cerr << "CREATE " << nowDuration + latence << "\n";
                entityMovCreate(nextEntity, nextInfo.first, nowDuration + latence, *nextnextEntity, nextnextInfo.first);
            }
        }
    }
    printf("STATE: %d\n", state);
}

/**
 * @brief Destroy the GraphicsClient:: Client object
 *
 */
GraphicsClient::~GraphicsClient() = default;

/**
 * @brief This function initializes the game
 *
 */
void GraphicsClient::initGame(std::vector<sf::Keyboard::Key> keyTab) {
    factory.make_background();
    factory.make_game_background_music();

    controllable_id = reg.spawn_entity();
    reg.add_component<component::controllable>((entity)controllable_id, component::controllable(keyTab[0], keyTab[1], keyTab[2], keyTab[3], keyTab[4]));
}

void GraphicsClient::handleSubStates(double delta, sf::RenderWindow& win)
{
    if (state == LEVEL1) {
        if (boss_dead) {
            substate = VICTORY;
        }
    }
    if (state == LEVEL2) {
        if (boss1_dead && boss2_dead) {
            substate = VICTORY;
        }
    }
    if (state == LEVEL3) {
        if (final_boss_dead) {
            substate = VICTORY;
        }
    }

    if (substate == VICTORY) {
        if (ui_handler.fade_started == false) {
            ui_handler = UIHandler("LEVEL CLEARED", true);
            ui_handler.fade_in(win, delta);
        }
        if (ui_handler.fade_started) {
            ui_handler.fade_in(win, delta);
        }
        if (ui_handler.fade_ended) {
            player1_dead = false;
            player2_dead = false;
            if (state == LEVEL1) {
                state = LEVEL2;
                substate = LEVEL2_START;
            } else if (state == LEVEL2) {
                state = LEVEL3;
                substate = LEVEL3_START;
            } else if (state == LEVEL3) {
                substate = SUB_NONE;
                exit(0);
            }
        }
    }

    if (player1_dead && player2_dead && player3_dead && player4_dead && substate != GAME_OVER) {
        substate = GAME_OVER;
        ui_handler = UIHandler("GAME OVER", true);
    }
    if (substate == GAME_OVER) {
        if (ui_handler.fade_started == false) {
            ui_handler.fade_in(win, delta);
        }
        if (ui_handler.fade_started) {
            ui_handler.fade_in(win, delta);
        }
        if (ui_handler.fade_ended) {
            substate = SUB_NONE;
            exit(0);
        }
    }
    if (substate == LEVEL1_START) {
        if (ui_handler.fade_started == false) {
            ui_handler = UIHandler("LEVEL 1", false);
            ui_handler.fade_out(win, delta);
        }
        if (ui_handler.fade_started) {
            ui_handler.fade_out(win, delta);
        }
        if (ui_handler.fade_ended) {
            substate = SUB_NONE;
        }
    }
    if (substate == LEVEL2_START) {
        if (ui_handler.fade_started == false) {
            ui_handler = UIHandler("LEVEL 2", false);
            ui_handler.fade_out(win, delta);
        }
        if (ui_handler.fade_started) {
            ui_handler.fade_out(win, delta);
        }
        if (ui_handler.fade_ended) {
            substate = SUB_NONE;
        }
    }
    if (substate == LEVEL3_START) {
        if (ui_handler.fade_started == false) {
            ui_handler = UIHandler("LEVEL 3", false);
            ui_handler.fade_out(win, delta);
        }
        if (ui_handler.fade_started) {
            ui_handler.fade_out(win, delta);
        }
        if (ui_handler.fade_ended) {
            substate = SUB_NONE;
        }
    }
}

void GraphicsClient::run()
{
    isRunning_ = true;

    win.create(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "R-Type");

    clientStarted = std::chrono::steady_clock::now();

    while (win.isOpen()) {
        double dt = frameClock.restart().asSeconds();
        auto now = std::chrono::steady_clock::now();

        while (win.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                win.close();
            if (event.type == sf::Event::KeyPressed)
                if (event.key.code == sf::Keyboard::Escape)
                    win.close();
        }
        if (state == LEVEL1 || state == LEVEL2 || state == LEVEL3) {
            sendPlayerInput();
            sendPlayerAction();
            receiveServerInfo();
            runLevel(dt);
            handleSubStates(dt, win);
        }

        reg.run_systems(dt);

        // FIXME: The engine should not clear
        // nor display, so the main loop will do that and possibly add more draws
        ui_handler.draw(win);

        std::this_thread::sleep_until(now + tickDuration);
    }
}
