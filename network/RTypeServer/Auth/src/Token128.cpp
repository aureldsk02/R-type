#include "../include/Token128.hpp"

Token128::Token128(TokenType type, uint64_t ttl_seconds)
        : _type(type),
        _created_at(Clock::now()),
        _ttl(std::chrono::seconds(ttl_seconds))
{
}    
TokenType Token128::getType() const  { return _type; }

bool Token128::isExpired() const  {
    return Clock::now() - _created_at > _ttl;
}

uint64_t Token128::getTTL() const  { return _ttl.count(); }

void Token128::createToken(const uint8_t *key, size_t key_len,
    const void* message, size_t message_len) 
{
    std::array<uint8_t, 32> full_hmac;
    crypto_auth_hmacsha256(full_hmac.data(), static_cast<const uint8_t*>(message),
        message_len, key);
    std::copy(full_hmac.begin(), full_hmac.begin() + 16, _data.begin());    
    _token_hex = IToken::toHex<16>(_data);
}
const Token128::Data& Token128::getData() const { return _data; }

void Token128::setData(const Data& data) {
    _data = data;
    _token_hex = IToken::toHex<16>(data);
}
const std::string& Token128::getHex() const { return _token_hex; }