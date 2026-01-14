#ifndef USERMANAGER
    #define USERMANAGER
    #include "../Auth/include/librairies.hpp"
    #include "../Auth/include/User.hpp"
    #include "TokenManager.hpp"


class UserManager {
    private:
        std::unordered_map<size_t, User> _users;                  
        std::unordered_map<std::string, size_t> _username_index;
        std::unordered_map<std::string, size_t> _auth_index;
        size_t _next_id = 1;

        UserManager(){};
    public:
        ~UserManager(){};
        static UserManager& getInstance() {
            static UserManager instance;
            return instance;
        }
        UserManager(const UserManager&) = delete;
        UserManager& operator=(const UserManager&) = delete;

        void updateId() {
            _next_id = _users.size();
        }

        std::string generate_unique_username(const std::string& baseUsername)
        {
            std::string username = baseUsername;
            int suffix = 1;
        
            while (_username_index.count(username) != 0) {
                username = baseUsername + std::to_string(suffix);
                ++suffix;
            }
            return username;
        }
        
        size_t createUser(const std::string& username, const std::string& plain_password, sqlite3 *db)
        {
            std::string finalUsername = generate_unique_username(username);
            size_t id = _next_id++;
            User u(id, finalUsername, plain_password);
            _users.emplace(id, u);
            _username_index[finalUsername] = id;
            u.save(db);
            return id;
        }

        bool removeUser(size_t id) {
            auto it = _users.find(id);
            if (it == _users.end())
                return false;
            _username_index.erase(it->second.getUsername());
            if (!it->second.getAuthTokenHex().empty())
                _auth_index.erase(it->second.getAuthTokenHex());
            _users.erase(it);
            return true;
        }

        std::optional<User> getUser(size_t id) const {
            auto it = _users.find(id);
            if (it == _users.end())
                return std::nullopt;
            return it->second;
        }

        User* getUserRef(size_t id) {
            auto it = _users.find(id);
            if (it == _users.end())
                return nullptr;
            return &(it->second);
        }
    
        std::optional<std::reference_wrapper<User>> getUserByUsername(const std::string& username)
        {
            auto it = _username_index.find(username);
            if (it == _username_index.end())
                return std::nullopt;
        
            User* u = getUserRef(it->second);
            if (!u)
                return std::nullopt;
            return std::ref(*u);
        }

        std::optional<User> getUserByAuthTokenHex(const std::string& auth_hex) const {
            auto it = _auth_index.find(auth_hex);
            if (it == _auth_index.end())
                return std::nullopt;
            return getUser(it->second);
        }

        void assignAuthToken(size_t id, std::shared_ptr<IToken> token)
        {
            User* u = getUserRef(id);
            if (!u)
                throw std::runtime_error("user not found");      
            if (u->getAuthToken().lock() && !u->getAuthTokenHex().empty()) {
                _auth_index.erase(u->getAuthTokenHex());
            }
            u->setAuthToken(token);  
            _auth_index[u->getAuthTokenHex()] = id;
        }
        
        void assignSessionToken(size_t id, std::shared_ptr<IToken> token) {
            User* u = getUserRef(id);
            if (!u)
                throw std::runtime_error("user not found");
        
            u->setSessionToken(token);
        }

        bool updateUser(const User& user) {
            auto it = _users.find(user.getId());
            if (it == _users.end())
                return false;
            if (it->second.getUsername() != user.getUsername()) {
                _username_index.erase(it->second.getUsername());
                if (_username_index.count(user.getUsername()))
                    throw std::runtime_error("username already exists");
                _username_index[user.getUsername()] = user.getId();
            }
            if (it->second.getAuthTokenHex() != user.getAuthTokenHex()) {
                if (!it->second.getAuthTokenHex().empty())
                    _auth_index.erase(it->second.getAuthTokenHex());
                if (!user.getAuthTokenHex().empty())
                    _auth_index[user.getAuthTokenHex()] = user.getId();
            }
            it->second = user;
            return true;
        }

        std::vector<User> listUsers() const {
            std::vector<User> out;
            out.reserve(_users.size());
            for (const auto& kv : _users)
                out.push_back(kv.second);
            return out;
        }

        std::vector<std::string> getUsersList() const {
            std::vector<std::string> out;
            out.reserve(_users.size());
            for (const auto& kv : _users)
                out.push_back(kv.second.getUsername());
            return out;
        }

        void displayUsers() {
            for (const auto& kv : _users) {
                std::cout << "----------------------------------" << std::endl;
                std::cout << "User n° " << kv.second.getId() << std::endl;
                std::cout << "Username : " << kv.second.getUsername() << std::endl;
                std::cout << "Password : " << kv.second.getPasswordHash() << std::endl;
            }
        }

        std::string getTopThreeUsersString() const {
            std::vector<User> users;
            for (const auto& [id, user] : _users) {
                if (user.getUserRole() == "PLAYER")
                    users.push_back(user);
            }
        
            std::sort(users.begin(), users.end(), [](const User& a, const User& b) {
                return a.getUserStats().getNbGamesWon() > b.getUserStats().getNbGamesWon();
            });
        
            std::ostringstream oss;
            for (size_t i = 0; i < 3; ++i) {
                if (i < users.size()) {
                    oss << users[i].getUsername() << " " << users[i].getUserStats().getNbGamesWon();
                } else {
                    oss << "Username 0";
                }
                if (i < 2)
                    oss << " ";
            }
            return oss.str();
        }

        void generateAuthTokenAssign(size_t id, const uint8_t server_key[32])
        {
            assignAuthToken(id, TokenManager::getInstance().generateAuthToken(id, server_key));
            return;
        }
        
        void generateSessionTokenAssign(size_t id, const uint8_t server_key[32])
        {
            assignAuthToken(id, TokenManager::getInstance().generateSessionToken(id, server_key));
            return;
        }

        bool authenticate(const std::string &username, const std::string &plain_password, const uint8_t server_key[32])
        {
            auto userOpt = getUserByUsername(username);
            if (!userOpt)
                return false;

            User& u = userOpt.value().get();

            const std::string& stored_hash = u.getPasswordHash();
            if (crypto_pwhash_str_verify(stored_hash.c_str(),
                                        plain_password.c_str(),
                                        plain_password.size()) != 0) {
                return false;
            }

            u.setLastLogin(std::time(nullptr));
            generateAuthTokenAssign(u.getId(), server_key);
            return true;
        }

        User load(sqlite3* db, size_t id) {
            const char* sql = R"(
                SELECT id, username, password_hash, auth_token, session_token,
                       last_login, client_hash, nb_games_played, nb_games_won
                FROM users
                WHERE id = ?;
            )";
        
            sqlite3_stmt* stmt = nullptr;
        
            if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
                throw std::runtime_error("Failed to prepare statement: " + std::string(sqlite3_errmsg(db)));
        
            sqlite3_bind_int(stmt, 1, static_cast<int>(id));
        
            User user;

            int rc = sqlite3_step(stmt);
            if (rc == SQLITE_ROW) {
                user.setId(static_cast<size_t>(sqlite3_column_int(stmt, 0)));
                user.setName(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)));
                user.setPasswordHash(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2)));
                user.setLastLogin(static_cast<std::time_t>(sqlite3_column_int64(stmt, 5)));
                user.setClientHash(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 6)));
        
                int nb_games_played = sqlite3_column_int(stmt, 7);
                int nb_games_won = sqlite3_column_int(stmt, 8);
        
                user.setNbGamesPlayed(nb_games_played);
                user.setNbGamesWon(nb_games_won);
            } else if (rc == SQLITE_DONE) {
                sqlite3_finalize(stmt);
                throw std::runtime_error("User not found in database.");
            } else {
                sqlite3_finalize(stmt);
                throw std::runtime_error("Error executing query: " + std::string(sqlite3_errmsg(db)));
            }
            sqlite3_finalize(stmt);
            return user;
        }

        void loadAllUsers(sqlite3 *db) {
            const char* sql = R"(
                SELECT id, username, password_hash, auth_token, session_token,
                       last_login, client_hash, nb_games_played, nb_games_won, level,
                       up, down, left, right, shoot, role, is_banned
                FROM users;
            )";
        
            sqlite3_stmt* stmt = nullptr;
        
            if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
                throw std::runtime_error("Failed to prepare statement: " + std::string(sqlite3_errmsg(db)));
            while (sqlite3_step(stmt) == SQLITE_ROW) {
                User user;

                user.setId(static_cast<size_t>(sqlite3_column_int(stmt, 0)));
                user.setName(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)));
                user.setPasswordHash(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2)));
                user.setLastLogin(static_cast<std::time_t>(sqlite3_column_int64(stmt, 5)));
                user.setClientHash(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 6)));
        
                int nb_games_played = sqlite3_column_int(stmt, 7);
                int nb_games_won = sqlite3_column_int(stmt, 8);
                int level = sqlite3_column_int(stmt, 9);
        
                user.setNbGamesPlayed(nb_games_played);
                user.setNbGamesWon(nb_games_won);
                user.setLevel(level);

                std::string up = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 10));
                std::string down = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 11));
                std::string left = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 12));
                std::string right = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 13));
                std::string space = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 14));

                std::vector<std::string> keyMap = {up, down, left, right, space};
                user.getUserControl().bind_keys(keyMap);

                std::string role = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 15));
                user.setRole(role);

                int is_banned = sqlite3_column_int(stmt, 16);
                bool value = (is_banned == 0) ? false : true;

                user.setBanned(value);

                _username_index[user.getUsername()] = user.getId();
                _users.emplace(user.getId(), std::move(user));
            }
            sqlite3_finalize(stmt);
            return;
        }
};
#endif