#ifndef DATABASEMANAGER
    #define DATABASEMANAGER
    #include "../Auth/include/librairies.hpp"

class DatabaseManager {
    public:
        static DatabaseManager& getInstance() {
            static DatabaseManager instance;
            return instance;
        }
        DatabaseManager(const DatabaseManager&) = delete;
        DatabaseManager& operator=(const DatabaseManager&) = delete;

        void open(const std::string& path = "Database/r-type.db") {
            if (_db) {
                std::cerr << "La base de données est déjà ouverte.\n";
                return;
            }
            if (sqlite3_open(path.c_str(), &_db) != SQLITE_OK) {
                throw std::runtime_error("Impossible d’ouvrir la base : " + std::string(sqlite3_errmsg(_db)));
            }    
            _isOpen = true;
        }

        void exec(const std::string& query) {
            char* errMsg = nullptr;
            if (sqlite3_exec(_db, query.c_str(), nullptr, nullptr, &errMsg) != SQLITE_OK) {
                std::string err = errMsg ? errMsg : "Erreur SQL inconnue";
                sqlite3_free(errMsg);
                throw std::runtime_error("Erreur SQL : " + err);
            }
        }

        void initialize() {
            const char* createTableSQL = R"(
                CREATE TABLE IF NOT EXISTS users (
                    id INTEGER PRIMARY KEY AUTOINCREMENT,
                    username TEXT UNIQUE NOT NULL,
                    password_hash TEXT NOT NULL,
                    auth_token TEXT,
                    session_token TEXT,
                    last_login INTEGER,
                    client_hash TEXT,
                    nb_games_played INTEGER DEFAULT 0,
                    nb_games_won INTEGER DEFAULT 0,
                    level INTEGER DEFAULT 1,
                    up TEXT NOT NULL DEFAULT 'UP',
                    down TEXT NOT NULL DEFAULT 'DOWN',
                    left TEXT NOT NULL DEFAULT 'LEFT',
                    right TEXT NOT NULL DEFAULT 'RIGHT',
                    shoot TEXT NOT NULL DEFAULT 'SPACE',
                    role TEXT NOT NULL DEFAULT 'PLAYER',
                    is_banned BOOL NOT NULL DEFAULT 0
                );
            )";
            exec(createTableSQL);
        }
    
        void clearDatabase() {
            exec("DELETE FROM users;");
        }
    
        void close() {    
            if (_db) {
                sqlite3_close(_db);
                _db = nullptr;
                _isOpen = false;
            }
            return;
        }

        sqlite3* getDB() const {
            return _db;
        }    
        const uint8_t* getServerKey() const {
            return _serverKey;
        }
        void generateServerKey() {
            char *secret_key = getenv("SERVER_SECRET_KEY");

            if (secret_key == nullptr)
                throw std::runtime_error("Impossible to find the secret_key");
            _serverKey = reinterpret_cast<uint8_t*>(secret_key);
            return;
        }
    
    private:
        sqlite3* _db = nullptr;
        bool _isOpen = false;
        uint8_t *_serverKey;

        DatabaseManager(){};    
        ~DatabaseManager() {
            close();
        }
};
#endif