#ifndef TOKEN128
    #define TOKEN128
    #include "./IToken.hpp"

class Token128 : public IToken {    
    public:
        using Data = std::array<uint8_t, 16>;
        Token128(TokenType type = TokenType::SESSION, uint64_t ttl_seconds = 86400);
        TokenType getType() const override;
        bool isExpired() const override;
        uint64_t getTTL() const override;
        void createToken(const uint8_t *key, size_t key_len,
            const void* message, size_t message_len) override;
        const Data& getData() const;
        void setData(const Data& data);
        const std::string& getHex() const;
    private:
        TokenType _type;
        Clock::time_point _created_at;
        std::chrono::seconds _ttl;
        Data _data;
        std::string _token_hex;
    
};
#endif