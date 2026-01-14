#ifndef ITOKEN
    #define ITOKEN
    #include "./librairies.hpp"

class IToken {
    public:
        virtual ~IToken() = default;
        virtual TokenType getType() const = 0;
        virtual bool isExpired() const = 0;
        virtual void createToken(const uint8_t* key, size_t key_len,
            const void* message, size_t message_len) = 0;
        virtual const std::string& getHex() const = 0;
        virtual uint64_t getTTL() const = 0;
        
        template <size_t N>
        static std::array<uint8_t, N> fromHex(const std::string& hex) {
            if (hex.size() != N * 2) {
                throw std::runtime_error("Invalid hex string length for token");
            }
            std::array<uint8_t, N> out{};
            for (size_t i = 0; i < N; i++) {
                std::string byteString = hex.substr(i * 2, 2);
                out[i] = static_cast<uint8_t>(std::stoul(byteString, nullptr, 16));
            }
            return out;
        }
        template <size_t N>
        static std::string toHex(const std::array<uint8_t, N> &token) {
            std::stringstream ss;

            for (size_t i = 0; i < N; i++) {
                ss << std::hex << std::setfill('0') << std::setw(2) << static_cast<int>(token[i]);
            }
            return ss.str();
        }
};
#endif