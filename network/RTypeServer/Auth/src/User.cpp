#include "../include/User.hpp"


//--------------USER_STATS------------------------------------------------

int UserStats::getNbGamesPlayed() const { return _games_played; }
int UserStats::getNbGamesWon() const { return _games_won; }
int UserStats::getLevel() const { return _level; }

void UserStats::setNbGamesPlayed(int games_played) { _games_played = games_played; }
void UserStats::setNbGamesWon(int games_won) { _games_won = games_won; }
void UserStats::setLevel(int level) { _level = level; }

void UserStats::updateGamesPlayed() { _games_played++; }
void UserStats::updateGamesWon() { _games_won++; }
void UserStats::updateLevel() { _level++; }

UserStats::UserStats() = default;
UserStats::~UserStats(){};

//--------------USER------------------------------------------------------------

User::User() = default;
User::User(size_t id, std::string username, std::string plain_password)
: _id(id), _username(std::move(username)) 
{
    if (sodium_init() < 0) {
        throw std::runtime_error("Failed to initialize libsodium");
    }
    char hashed_password[crypto_pwhash_STRBYTES];
    sodium_mlock(const_cast<char*>(plain_password.data()), plain_password.size());

    if (crypto_pwhash_str(
            hashed_password,
            plain_password.c_str(),
            plain_password.size(),
            crypto_pwhash_OPSLIMIT_MODERATE,
            crypto_pwhash_MEMLIMIT_MODERATE) != 0)
    {
        sodium_munlock(const_cast<char*>(plain_password.data()), plain_password.size());
        throw std::runtime_error("Failed to hash password");
    }
    sodium_munlock(const_cast<char*>(plain_password.data()), plain_password.size());
    sodium_memzero(const_cast<char*>(plain_password.data()), plain_password.size());
    _password_hash = hashed_password;
}

const UserStats User::getUserStats() const {
    return _stats;
}

void User::setLevel(const int level) {
    _stats.setLevel(level);
}

void User::bind_keys(const std::vector<std::string> &keyMap)
{
    _control.bind_keys(keyMap);
    return;
}

UserControl &User::getUserControl() {
    return _control;
}

const std::string User::getUserRole() const {
    std::string role = (user_role == Role::PLAYER) ? "PLAYER" : "ADMIN";

    return role;
}

void User::setRole(const std::string &role) {
    Role my_role = (role == "PLAYER") ? Role::PLAYER : Role::ADMIN;

    user_role = my_role;
}

void User::setBanned(const bool banned){
    is_banned = banned;
}

const bool User::isUserBanned() const {
    return is_banned;
}
void User::setName(const std::string &name)
{
    _username = name;
}
void User::setAuthToken(const std::shared_ptr<IToken> token) {
    _auth_token = token;
}
void User::setSessionToken(const std::shared_ptr<IToken> token) {
    _session_token = token;
}
void User::setClientHash(const std::string &client_hash) {
    _client_hash = client_hash;
}
void User::setId(const size_t &id) {
    _id = id;
}
void User::setPasswordHash(const std::string &password_hash) {
    _password_hash = password_hash;
}
void User::setNbGamesPlayed(const int games_played) { _stats.setNbGamesPlayed(games_played);}
void User::setNbGamesWon(const int games_won) { _stats.setNbGamesWon(games_won); }
const std::string User::getClientHash() { return _client_hash; }
const std::weak_ptr<IToken> User::getAuthToken() { return _auth_token; }
const std::weak_ptr<IToken> User::getSessionToken() { return _session_token; }
const std::string User::getAuthTokenHex() const {
    if (auto token = _auth_token.lock())
        return token->getHex();
    else
        return "";
};
const std::string& User::getUsername() const { return _username; }
const std::string& User::getPasswordHash() const { return _password_hash; }
size_t User::getId() const { return _id; }
void User::setLastLogin(const std::time_t time) {_last_login_at = time;}
void User::save(sqlite3* db) const {
    const char* sql = R"(
        INSERT INTO users (
            id, username, password_hash, auth_token, session_token,
            last_login, client_hash, nb_games_played, nb_games_won, level, 
            up, down, left, right, shoot, role, is_banned
        )
        VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)
        ON CONFLICT(id) DO UPDATE SET
            username = excluded.username,
            password_hash = excluded.password_hash,
            auth_token = excluded.auth_token,
            session_token = excluded.session_token,
            last_login = excluded.last_login,
            client_hash = excluded.client_hash,
            nb_games_played = excluded.nb_games_played,
            nb_games_won = excluded.nb_games_won,
            level = excluded.level,
            up = excluded.up,
            down = excluded.down,
            left = excluded.left,
            right = excluded.right,
            shoot = excluded.shoot,
            role = excluded.role,
            is_banned = excluded.is_banned
    )";

    sqlite3_stmt* stmt = nullptr;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
        throw std::runtime_error("Failed to prepare statement: " + std::string(sqlite3_errmsg(db)));

    int nb_games_played = _stats.getNbGamesPlayed();
    int nb_games_won = _stats.getNbGamesWon();
    int level = _stats.getLevel();

    sqlite3_bind_int(stmt, 1, static_cast<int>(_id));
    sqlite3_bind_text(stmt, 2, _username.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, _password_hash.c_str(), -1, SQLITE_TRANSIENT);
    std::string authHex;
    if (auto auth = _auth_token.lock()) {
        authHex = auth->getHex();
    } else {
        authHex = "";
    }
    sqlite3_bind_text(stmt, 4, authHex.c_str(), -1, SQLITE_TRANSIENT);
    std::string sessionHex;
    if (auto session = _session_token.lock()) {
        sessionHex = session->getHex();
    } else {
        sessionHex = "";
    }
    sqlite3_bind_text(stmt, 5, sessionHex.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int64(stmt, 6, static_cast<sqlite3_int64>(_last_login_at));
    sqlite3_bind_text(stmt, 7, _client_hash.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 8, nb_games_played);
    sqlite3_bind_int(stmt, 9, nb_games_won);
    sqlite3_bind_int(stmt, 10, level);

    const std::vector<std::string> keyMap = _control.getKeys();
    for (auto a : keyMap) {
        std::cout << "A : " << a << std::endl;
    }

    sqlite3_bind_text(stmt, 11, keyMap[0].c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 12, keyMap[1].c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 13, keyMap[2].c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 14, keyMap[3].c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 15, keyMap[4].c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 16, getUserRole().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 17, isUserBanned() ? 1 : 0);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        sqlite3_finalize(stmt);
        throw std::runtime_error("Failed to execute statement: " + std::string(sqlite3_errmsg(db)));
    }
    sqlite3_finalize(stmt);
}