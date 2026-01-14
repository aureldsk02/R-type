#include "../include/Token256.hpp"


Token256::Token256(TokenType type, uint64_t ttl_seconds)
        : _type(type),
          _created_at(Clock::now()),
          _ttl(std::chrono::seconds(ttl_seconds))
{
}

TokenType Token256::getType() const { return _type; }

bool Token256::isExpired() const {
    return Clock::now() - _created_at > _ttl;
}

uint64_t Token256::getTTL() const { return _ttl.count(); }

void Token256::createToken(const uint8_t *key, size_t key_len,
    const void* message, size_t message_len)
{
    crypto_auth_hmacsha256(_data.data(), static_cast<const uint8_t*>(message), message_len, key);
    _token_hex = IToken::toHex<32>(_data);
    return;
}

const Token256::Data& Token256::getData() const { return _data; }

void Token256::setData(const Data& data) {
    _data = data;
    _token_hex = IToken::toHex<32>(data);
}
const std::string& Token256::getHex() const { return _token_hex; }