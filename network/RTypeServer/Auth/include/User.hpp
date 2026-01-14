#ifndef USER
    #define USER
    #include "./IToken.hpp"

class UserControl {
public:
    enum class Action {
        MOVE_UP,
        MOVE_DOWN,
        MOVE_LEFT,
        MOVE_RIGHT,
        SHOOT
    };
    UserControl() {
        _keyBindings[Action::MOVE_UP] = "UP";
        _keyBindings[Action::MOVE_DOWN] = "DOWN";
        _keyBindings[Action::MOVE_LEFT] = "LEFT";
        _keyBindings[Action::MOVE_RIGHT] = "RIGHT";
        _keyBindings[Action::SHOOT] = "SPACE";
    }

    void setKey(Action action, std::string &newKey) {
        _keyBindings[action] = newKey;
    }

    const std::string getKey(Action action) const {
        auto it = _keyBindings.find(action);
        if (it != _keyBindings.end())
            return it->second;
        return "UNKNOWN";
    }

    const std::vector<std::string> getKeys() const {
        std::vector<std::string> vector = {
            _keyBindings.at(Action::MOVE_UP),
            _keyBindings.at(Action::MOVE_DOWN),
            _keyBindings.at(Action::MOVE_LEFT),
            _keyBindings.at(Action::MOVE_RIGHT),
            _keyBindings.at(Action::SHOOT)
        };
        return vector;
    }

    void bind_keys(const std::vector<std::string> &_keyMap) {
        _keyBindings[Action::MOVE_UP] = _keyMap[0];
        _keyBindings[Action::MOVE_DOWN] = _keyMap[1];
        _keyBindings[Action::MOVE_LEFT] = _keyMap[2];
        _keyBindings[Action::MOVE_RIGHT] = _keyMap[3];
        _keyBindings[Action::SHOOT] = _keyMap[4];
        return;
    }

private:
    std::unordered_map<Action, std::string> _keyBindings;
};

class UserStats {
    private:
        int _games_played = 0;
        int _games_won = 0;
        int _level = 0;

    public:
        UserStats();
        ~UserStats();

        int getNbGamesPlayed() const;
        int getNbGamesWon() const;
        int getLevel() const;

        void setNbGamesPlayed(int games_played);
        void setNbGamesWon(int games_won);
        void setLevel(int level);

        void updateGamesPlayed();
        void updateGamesWon();
        void updateLevel();

};

class User {
    public:
        User();
        User(size_t id, std::string username, std::string plain_password);
        void setName(const std::string &name);
        void setAuthToken(const std::shared_ptr<IToken> token);
        void setSessionToken(const std::shared_ptr<IToken> token);
        void setClientHash(const std::string &client_hash);
        void setId(const size_t &id);
        void setPasswordHash(const std::string &password_hash);
        void setNbGamesPlayed(const int games_played);
        void setNbGamesWon(const int games_won);
        void setLevel(const int level);
        const UserStats getUserStats() const;
        UserControl &getUserControl();
        const std::string getUserRole() const;
        void setRole(const std::string &role);
        void setBanned(const bool banned);
        const bool isUserBanned() const;
        const std::string getClientHash();
        const std::weak_ptr<IToken> getAuthToken();
        const std::weak_ptr<IToken> getSessionToken();
        const std::string getAuthTokenHex() const;
        const std::string& getUsername() const;
        const std::string& getPasswordHash() const;
        size_t getId() const;
        void setLastLogin(const std::time_t time);
        void save(sqlite3* db) const;
        void bind_keys(const std::vector<std::string> &keyMap);

    private:
        size_t _id = 0;
        std::string _username;
        std::string _password_hash;
        std::weak_ptr<IToken> _auth_token;
        std::weak_ptr<IToken> _session_token;
        std::time_t _last_login_at = 0;
        std::string _client_hash = "";
        UserStats _stats;
        UserControl _control;
        Role user_role = Role::PLAYER;
        bool is_banned = false;
};
#endif