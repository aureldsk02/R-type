#ifndef PARSEINPUT
    #define PARSEINPUT
    #include "../../include/librairies.hpp"
    #include "../../components/include/UIElement.hpp"
    #include "../../components/include/InputFieldElement.hpp"

std::shared_ptr<UIElement> parseInputFieldElement(const libconfig::Setting& setting) {
    try {
        std::string id;
        std::string fontPath;
        sf::Vector2f pos(0.f, 0.f), size(100.f, 30.f);
        bool isPassword = false;
        bool remapMode = false;
        sf::Color activeColor = sf::Color::Blue;
        int r = 255, g = 255, b = 255, a = 255;
        unsigned int textSize = 17;
        bool can_display = true;

        setting.lookupValue("id", id);
        setting.lookupValue("font", fontPath);

        if (setting.exists("pos")) {
            const libconfig::Setting& posSetting = setting.lookup("pos");
            if (posSetting.getLength() == 2)
                pos = { static_cast<float>(posSetting[0]), static_cast<float>(posSetting[1]) };
        }

        if (setting.exists("display")) {
            std::cout << "Display existe oh" << std::endl;
            setting.lookupValue("display", can_display);
        }

        if (setting.exists("size")) {
            const libconfig::Setting& sizeSetting = setting.lookup("size");
            if (sizeSetting.getLength() == 2)
                size = { static_cast<float>(sizeSetting[0]), static_cast<float>(sizeSetting[1]) };
        }

        setting.lookupValue("isPassword", isPassword);
        setting.lookupValue("remapMode", remapMode);
        setting.lookupValue("textSize", textSize);

        if (setting.exists("activeOutlineColor")) {
            const libconfig::Setting& color = setting.lookup("activeOutlineColor");
            if (color.getLength() >= 3) {
                r = color[0];
                g = color[1];
                b = color[2];
                if (color.getLength() == 4) {
                    a = color[3];
                }
            }
            activeColor = sf::Color(
                static_cast<sf::Uint8>(r),
                static_cast<sf::Uint8>(g),
                static_cast<sf::Uint8>(b),
                static_cast<sf::Uint8>(a)
            );
        }

        if (!can_display)
            std::cout << "J'ai bien vu que c'est false unh" << std::endl;

        return std::make_shared<InputFieldElement>(id, fontPath, pos, size, isPassword, activeColor, remapMode, textSize, can_display);
    }
    catch (const libconfig::SettingNotFoundException& e) {
        std::cerr << "InputFieldElement: champ manquant (" << e.getPath() << ")\n";
    }
    catch (const libconfig::SettingTypeException& e) {
        std::cerr << "InputFieldElement: type invalide (" << e.getPath() << ")\n";
    }

    return nullptr;
}
#endif