#ifndef PARSETEXT
    #define PARSETEXT
    #include "../../include/librairies.hpp"
    #include "../../components/include/UIElement.hpp"
    #include "../../components/include/TextElement.hpp"

std::shared_ptr<UIElement> parseTextElement(const libconfig::Setting& setting)
{
    try {
        std::string id;
        std::string fontPath;
        std::string str;
        unsigned int size = 20;
        sf::Vector2f pos(0.f, 0.f);
        sf::Color color = sf::Color::White;

        setting.lookupValue("id", id);
        setting.lookupValue("font", fontPath);
        setting.lookupValue("text", str);
        setting.lookupValue("textSize", size);

        const libconfig::Setting& posSetting = setting.lookup("pos");
        if (posSetting.getLength() == 2) {
            pos.x = static_cast<float>(posSetting[0]);
            pos.y = static_cast<float>(posSetting[1]);
        }

        const libconfig::Setting& colorSetting = setting.lookup("textColor");
        int r = 255, g = 255, b = 255, a = 255;

        if (colorSetting.getLength() >= 3) {
            r = colorSetting[0];
            g = colorSetting[1];
            b = colorSetting[2];
            if (colorSetting.getLength() == 4) {
                a = colorSetting[3];
            }
        }

        color = sf::Color(
            static_cast<sf::Uint8>(r),
            static_cast<sf::Uint8>(g),
            static_cast<sf::Uint8>(b),
            static_cast<sf::Uint8>(a)
        );
        return std::make_shared<TextElement>(id, fontPath, str, color, pos, size);
    }
    catch (const libconfig::SettingNotFoundException& e) {
        std::cerr << "Erreur : champ manquant dans TextElement (" << e.getPath() << ")" << std::endl;
    }
    catch (const libconfig::SettingTypeException& e) {
        std::cerr << "Erreur : type invalide dans TextElement (" << e.getPath() << ")" << std::endl;
    }

    return nullptr;
}
#endif