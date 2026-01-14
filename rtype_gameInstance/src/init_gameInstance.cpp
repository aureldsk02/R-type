/*
** EPITECH PROJECT, 2025
** G-CPP-500-COT-5-1-rtype-8
** File description:
** init_server
*/

/**
 * @file init_server.cpp
 * @author Aurel DOSSOU KOHI
 * @brief This file contains the definition of the server initializing funcions
 * @version 0.1
 * @date 2025-10-12
 *
 * @copyright Copyright (c)
 *
 */

#include "../include/gameInstance.hpp"

/**
 * @brief This function uses the ResourceManager to pre-load textures and fonts
 * that will be used in the game
 *
 */
void load_textures(void) {
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

bool GameInstance::hasClient(const asio::ip::udp::endpoint& client)
{
    return all_clients.find(client) != all_clients.end();
}

void GameInstance::addMessage(const asio::ip::udp::endpoint& client, const std::vector<int8_t>& msg) {
    std::lock_guard<std::mutex> lock(mtx);
    messages.push_back(std::make_pair(client, msg));
}

void GameInstance::addClient(const asio::ip::udp::endpoint& client) {
    
    std::cout << "Adding client: " << client.address().to_string() << ":" << client.port() << std::endl;

    if (!hasClient(client)) {
        all_clients[client] = -1;
    }
}

void GameInstance::initializeGame(void) {
    factory.make_background();
    if (diff_mode != PVP) {
        factory.make_ceiling();
        factory.make_floor();
    }
    factory.make_menu_background_music();
}

/**
 * @brief This function initializes the players
 *
 */
void GameInstance::initializePlayers(void) {
    std::vector<std::string> tab = {"player1", "player2", "player3", "player4"};
    int i = 0;

    for (auto & player : all_clients) {
        player.second = factory.make_entity(tab[i]);

        auto& pos =
            reg.get_components<component::position>()[player.second].value();
        
        pos.x = 50;
        pos.y = 100 + (i*50);
        i++;
    }
}

/**
 * @brief This function initializes the players for pvp mode
 *
 */
void GameInstance::initializePlayersPVP(void) {

    std::vector<std::string> tab = {"player1", "player2_flipped"};

    int i = 0;

    for (auto & player : all_clients) {
        player.second = factory.make_entity(tab[i]);

        auto& pos =
            reg.get_components<component::position>()[player.second].value();
        
        if (i == 0)
            pos.x = 50;
        else
            pos.x = 250;
        pos.y = 250;
        i++;
    }
}

GameInstance::GameInstance(std::string _id, NetworkManager& server, const GameSettings& settings) :
    id(_id), reg(win), factory(reg), server_(server)
{
    reg.toggleLogic();
    reg.toggleMovement();
    reg.togglePhysics();
    reg.toggleRendering();
    counter = 0;

    load_textures();
    initializeGame();

    if (settings.getMode() == GameSettings::GameMode::PVP) {
        diff_mode = PVP;
        state = LEVEL1;
    } else if (settings.getMode() == GameSettings::GameMode::CUSTOM) {
        diff_mode = CUSTOM;
        custom_conf_path = settings.getFilePath();
        state = CUSTOM_LEVEL;
    } else {
        switch (settings.getDifficulty()) {
            case GameSettings::Difficulty::MEDIUM:
                diff_mode = MEDIUM;
                break;
            case GameSettings::Difficulty::DIFFICULT:
                diff_mode = HARD;
                break;
            default:
                diff_mode = EASY;
                break;
        }
        state = LEVEL1;
    }
    loadLevel();
}

/**
 * @brief Destroy the GameInstance:: Server object
 *
 */
GameInstance::~GameInstance() {}

void GameInstance::addSessionId(const std::string& id) {
    sessionIds.push_back(id);
}

const std::vector<std::string>& GameInstance::getSessionIds() const {
    return sessionIds;
}

void GameInstance::run()
{
    gameStarted = std::chrono::steady_clock::now();

    if (diff_mode == PVP) {
        initializePlayersPVP();
    } else {
        initializePlayers();
    }

    win.create(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "R-Type Server");

    while (win.isOpen()) {
        auto start = std::chrono::steady_clock::now();

        while (win.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                win.close();
            if (event.type == sf::Event::KeyPressed)
                if (event.key.code == sf::Keyboard::Escape)
                    win.close();
        }
        double dt = frameClock.restart().asSeconds();

        reg.run_systems(dt);

        logGameEntities();

        runLevel(dt);

        updatePlayerHealth();

        handleWinOrLoss();

        std::this_thread::sleep_until(start + tickDuration);
    }
}
