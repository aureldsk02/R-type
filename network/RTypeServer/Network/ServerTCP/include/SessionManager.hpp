#pragma once
#include "ServerTCP.hpp"

class SessionManager {
    public:
        static SessionManager& getInstance() {
            static SessionManager instance;
            return instance;
        }
    
        void addSession(const std::string& id, std::shared_ptr<Session> session) {
            std::lock_guard<std::mutex> lock(mutex_);
            sessions_[id] = session;
        }
    
        void removeSession(const std::string& id) {
            std::lock_guard<std::mutex> lock(mutex_);
            sessions_.erase(id);
        }
    
        std::shared_ptr<Session> getSession(const std::string& id) {
            std::lock_guard<std::mutex> lock(mutex_);
            auto it = sessions_.find(id);
            return (it != sessions_.end()) ? it->second : nullptr;
        }
    
        void broadcastTo(const std::vector<std::string>& ids, const std::string& msg) {
            std::lock_guard<std::mutex> lock(mutex_);
            for (auto& id : ids) {
                if (sessions_.count(id))
                    sessions_[id]->enqueue_write(msg);
            }
        }
    
    private:
        SessionManager() = default;
        std::unordered_map<std::string, std::shared_ptr<Session>> sessions_;
        std::mutex mutex_;
    };
    