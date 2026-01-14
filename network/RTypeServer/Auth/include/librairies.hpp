#ifndef LIBRAIRIES
    #define LIBRAIRIES
    #include <iostream>
    #include <string>
    #include <array>
    #include <ctime>
    #include <sodium.h>
    #include <chrono>
    #include <cstdint>
    #include <iomanip>
    #include <unordered_map>
    #include <memory>
    #include <optional>
    #include <vector>
    #include <cstring>
    #include <random>
    #include <fstream>
    #include <sqlite3.h>
    #include <asio.hpp>
    #include <deque>
    #include <atomic>
    #include <thread>

using Clock = std::chrono::steady_clock;

enum class TokenType {
    AUTH,
    SESSION
};

enum class Role { PLAYER, ADMIN };

//adding nothing

#endif /* defined(_Game_) */