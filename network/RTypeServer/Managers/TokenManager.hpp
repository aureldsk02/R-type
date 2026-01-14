#ifndef TOKENMANAGER
    #define TOKENMANAGER
    #include "../Auth/include/librairies.hpp"
    #include "../Auth/include/Token256.hpp"
    #include "../Auth/include/Token128.hpp"

class TokenManager {
    private:
        std::unordered_map<size_t, std::shared_ptr<IToken>> _tokens;
        std::unordered_map<std::string, size_t> _hex_tokens;
        size_t _next_id = 1;
    
        TokenManager() = default;
    
    public:
        ~TokenManager() = default;
    
        static TokenManager& getInstance() {
            static TokenManager instance;
            return instance;
        }
    
        TokenManager(const TokenManager&) = delete;
        TokenManager& operator=(const TokenManager&) = delete;
    
        std::shared_ptr<IToken> createToken(TokenType type,
            const uint8_t* key, size_t key_len,
            const void* message, size_t message_len)
        {
            std::shared_ptr<IToken> token;
    
            if (type == TokenType::AUTH) {
                token = std::make_shared<Token256>(type);
            } else {
                token = std::make_shared<Token128>(type);
            }
            token->createToken(key, key_len, message, message_len);
    
            _tokens[_next_id] = token;
            _hex_tokens[token->getHex()] = _next_id;
            
            _next_id++;
            return token;
        }

        void deleteToken(size_t id)
        {
            auto it = _tokens.find(id);
            if (it != _tokens.end()) {
                _hex_tokens.erase(it->second->getHex());
                _tokens.erase(it);
            }
        }
    
        bool verifyToken(const std::string& hex) const {
            return _hex_tokens.count(hex) > 0;
        }
    
        void update() {
            std::vector<size_t> expired_ids;
            for (auto& [id, token] : _tokens) {
                if (token->isExpired())
                    expired_ids.push_back(id);
            }
            for (size_t id : expired_ids) {
                deleteToken(id);
            }
        }

        std::shared_ptr<Token256> generateAuthToken(size_t player_id, const uint8_t server_key[32])
        {
            auto token = std::make_shared<Token256>(TokenType::AUTH);

            uint8_t message[sizeof(player_id) + 24];
            std::memcpy(message, &player_id, sizeof(player_id));

            std::random_device rd;
            for (size_t i = 0; i < 24; i++) {
                message[sizeof(player_id) + i] = static_cast<uint8_t>(rd() & 0xFF);
            }
            token->createToken(server_key, 32, message, sizeof(message));

            size_t id = _next_id++;
            _tokens[id] = token;
            _hex_tokens[token->getHex()] = id;

            return token;
        }

        std::shared_ptr<Token128> generateSessionToken(size_t player_id, const uint8_t server_key[32])
        {
            auto auth_token = generateAuthToken(player_id, server_key);
            auto session_token = std::make_shared<Token128>(TokenType::SESSION);

            Token128::Data session_data{};
            std::copy(auth_token->getData().begin(),
                    auth_token->getData().begin() + 16,
                    session_data.begin());

            session_token->setData(session_data);

            size_t id = _next_id++;
            _tokens[id] = session_token;
            _hex_tokens[session_token->getHex()] = id;

            return session_token;
        }
};
#endif