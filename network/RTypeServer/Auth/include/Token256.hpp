#ifndef TOKEN256
    #define TOKEN256
    #include "./IToken.hpp"

class Token256 : public IToken {
    using Data = std::array<uint8_t, 32>;

    TokenType _type;
    Clock::time_point _created_at;
    std::chrono::seconds _ttl;
    Data _data;
    std::string _token_hex;

public:
    Token256(TokenType type = TokenType::AUTH, uint64_t ttl_seconds = 86400);
    
    TokenType getType() const override;

    bool isExpired() const override;
    uint64_t getTTL() const override;

    void createToken(const uint8_t *key, size_t key_len,
        const void* message, size_t message_len) override;
    const Data& getData() const;
    void setData(const Data& data);
    const std::string& getHex() const;
};
#endif