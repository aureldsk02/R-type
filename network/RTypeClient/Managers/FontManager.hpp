#ifndef FONTMANAGER
    #define FONTMANAGER
    #include "../UI/include/librairies.hpp"

class FontManager {
    public:
        static FontManager& getInstance() {
            static FontManager instance;
            return instance;
        }
    
        FontManager(const FontManager&) = delete;
        FontManager& operator=(const FontManager&) = delete;
        sf::Font& getFont(const std::string& path) {
            if (fonts.find(path) == fonts.end()) {
                if (!fonts[path].loadFromFile(path))
                    std::cerr << "Error: impossible to load the font " << path << std::endl;
            }
            return fonts[path];
        }
    
    private:
        FontManager() = default;
        ~FontManager() = default;
        std::unordered_map<std::string, sf::Font> fonts;
    };
#endif