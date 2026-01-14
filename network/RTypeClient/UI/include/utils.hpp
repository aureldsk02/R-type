#ifndef UTILS
    #define UTILS
    #include "./librairies.hpp"

inline std::string getDate()
{
    std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
    std::time_t now_time_t = std::chrono::system_clock::to_time_t(now);
    std::tm* local_time = std::localtime(&now_time_t);
    std::stringstream ss;

    ss << std::put_time(local_time, "%d/%m/%Y");
    return ss.str();
}

inline std::string getHour()
{
    std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
    std::time_t now_time_t = std::chrono::system_clock::to_time_t(now);
    std::tm* local_time = std::localtime(&now_time_t);
    std::stringstream ss;

    ss << std::put_time(local_time, "%H:%M:%S");
    return ss.str();
}

inline std::string keyToReadable(sf::Keyboard::Key key) {
    static const std::unordered_map<sf::Keyboard::Key, std::string> keyNames = {
        {sf::Keyboard::Up, "UP"},
        {sf::Keyboard::Down, "DOWN"},
        {sf::Keyboard::Left, "LEFT"},
        {sf::Keyboard::Right, "RIGHT"},
        {sf::Keyboard::Space, "SPACE"},
        {sf::Keyboard::Enter, "ENTER"},
        {sf::Keyboard::Escape, "ESC"}
    };

    auto it = keyNames.find(key);
    if (it != keyNames.end())
        return it->second;

    if (key >= sf::Keyboard::A && key <= sf::Keyboard::Z)
        return std::string(1, static_cast<char>('A' + (key - sf::Keyboard::A)));

    if (key >= sf::Keyboard::Num0 && key <= sf::Keyboard::Num9)
        return std::string(1, static_cast<char>('0' + (key - sf::Keyboard::Num0)));

    return "?";
}
#endif