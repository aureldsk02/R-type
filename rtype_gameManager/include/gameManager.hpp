#ifndef GAME_MANAGER_HPP
#define GAME_MANAGER_HPP

#include <mutex>
#include <vector>
#include <map>
#include <asio.hpp>
#include "../../rtype_gameInstance/include/gameInstance.hpp"
#include "../../../RTypeServer/Network/ServerTCP/include/SessionManager.hpp"

class GameManager {
    private:
        std::vector<std::pair<asio::ip::udp::endpoint, std::vector<int8_t>>>* messages_ = nullptr;
        std::mutex* mtx_ = nullptr;
        NetworkManager* udpServer_ = nullptr;
    
        std::map<std::string, std::shared_ptr<GameInstance>> active_games_;
    
        void process_messages();
        
        GameManager();
    
        GameManager(const GameManager&) = delete;
        GameManager& operator=(const GameManager&) = delete;

        std::vector<std::thread> gameThreads_;

        std::atomic<bool> isRunning;

    public:
        static GameManager& getInstance();
        
        static void init(
            std::vector<std::pair<asio::ip::udp::endpoint, std::vector<int8_t>>>& messages,
            std::mutex& mtx,
            NetworkManager& _udpServer
        );

        void addClientToGame(
            const std::string& game_id,
            asio::ip::udp::endpoint& client,
            const std::string& session_id
        );
    
        std::vector<std::pair<asio::ip::udp::endpoint, std::vector<int8_t>>>& getMessages();
        std::mutex& getMutex();
        NetworkManager& getUdpServer();
    
        ~GameManager();
    
        std::shared_ptr<GameInstance> create_game(const std::string& id, const GameSettings& settings);
        std::shared_ptr<GameInstance> get_game(const std::string& id);
        void start_game(const std::string& id);
        void remove_game(const std::string& id);
        void list_games();
    };


#endif /* !GAME_MANAGER_HPP_ */