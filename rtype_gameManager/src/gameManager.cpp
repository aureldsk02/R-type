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

#include "../include/gameManager.hpp"

GameManager& GameManager::getInstance() {
    static GameManager instance; 
    return instance;
}

void GameManager::init(
    std::vector<std::pair<asio::ip::udp::endpoint, std::vector<int8_t>>>& messages,
    std::mutex& mtx,
    NetworkManager& _udpServer
) {
    GameManager& instance = getInstance();
    
    if (instance.messages_ != nullptr) {
        return; 
    }
    
    instance.messages_ = &messages;
    instance.mtx_ = &mtx;
    instance.udpServer_ = &_udpServer;
}

// --- ACCESSEURS ---

static void check_init(const void* ptr, const std::string& componentName) {
    if (ptr == nullptr) {
        throw std::runtime_error("GameManager not initialized. Call GameManager::init() before accessing " + componentName + ".");
    }
}

std::vector<std::pair<asio::ip::udp::endpoint, std::vector<int8_t>>>& GameManager::getMessages() {
    check_init(messages_, "messages");
    return *messages_;
}

std::mutex& GameManager::getMutex() {
    check_init(mtx_, "mutex");
    return *mtx_;
}

NetworkManager& GameManager::getUdpServer() {
    check_init(udpServer_, "UDP server");
    return *udpServer_;
}

GameManager::GameManager() {
    isRunning.store(true);
    std::thread messageThread([this]() {
        while (isRunning.load()) {
            process_messages();
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    });
    gameThreads_.emplace_back(std::move(messageThread));
}

GameManager::~GameManager() {
    isRunning.store(false);
    for (auto& thread : gameThreads_) {
        if (thread.joinable()) {
            thread.join();
        }
    }
}

// --- LOGIQUE DE JEU ---

std::shared_ptr<GameInstance> GameManager::create_game(const std::string& id, const GameSettings& settings) {
    // std::lock_guard<std::mutex> lock(getMutex()); 
    
    if (active_games_.count(id)) {
        return nullptr;
    }
    auto new_game = std::make_shared<GameInstance>(id, getUdpServer(), settings);
    std::cout << "Game created with ID: " << id << std::endl;
    active_games_[id] = new_game;
    
    return new_game;
}

std::shared_ptr<GameInstance> GameManager::get_game(const std::string& id) {
    // std::lock_guard<std::mutex> lock(getMutex());
    if (active_games_.count(id)) {
        return active_games_[id];
    }
    return nullptr;
}

void GameManager::remove_game(const std::string& id) {
    // std::lock_guard<std::mutex> lock(getMutex());
    active_games_.erase(id);
}

void GameManager::list_games() {
    // std::lock_guard<std::mutex> lock(getMutex());
    std::cout << "Active Games: ";
    for (const auto& pair : active_games_) {
        std::cout << pair.first << " ";
    }
    std::cout << std::endl;
}

void GameManager::addClientToGame(
    const std::string& game_id,
    asio::ip::udp::endpoint& client,
    const std::string& session_id
) {
    // std::lock_guard<std::mutex> lock(getMutex());
    if (active_games_.count(game_id)) {
        active_games_[game_id]->addClient(client);
        active_games_[game_id]->addSessionId(session_id);
    }
}

void GameManager::process_messages() {
    std::pair<asio::ip::udp::endpoint, std::vector<int8_t>> msg_pair;

    size_t len = 0;
    {
        std::lock_guard<std::mutex> lock(getMutex());
        len = getMessages().size();
    }

    while (len > 0) {
        {
            std::lock_guard<std::mutex> lock(getMutex());
            if (getMessages().empty()) {
                return;
            }
            msg_pair = getMessages().front();
            getMessages().erase(getMessages().begin());
            len = getMessages().size();
        }
        const auto& endpoint = msg_pair.first;
        const auto& msg = msg_pair.second;

        for (auto& game_pair : active_games_) {
            auto& gameInstance = game_pair.second;
            if (gameInstance->hasClient(endpoint)) {
                std::cout << "Processing message for game: " << game_pair.first << std::endl;
                gameInstance->addMessage(endpoint, msg);
                break;
            }
        }
    }
    getMessages().clear();
}

void GameManager::start_game(const std::string& id) {
    std::lock_guard<std::mutex> lock(getMutex());
    if (active_games_.count(id)) {
        std::cout << "Starting game with ID: " << id << std::endl;

        std::thread gameThread([this, id]() {
            for (const auto& sid : active_games_[id]->getSessionIds()) {
                auto session = SessionManager::getInstance().getSession(sid);
                if (session)
                    session->enqueue_write("LAUNCH_OK\n");
            }
            active_games_[id]->run();
        });
        gameThreads_.emplace_back(std::move(gameThread));
    }
}