#ifndef PARSECIRCLE
    #define PARSECIRCLE
    #include "../../include/librairies.hpp"
    #include "../../components/include/UIElement.hpp"
    #include "../../components/include/CircleElement.hpp"

std::shared_ptr<UIElement> parseCircleElement(const libconfig::Setting& setting) {
    try {
        std::string id;
        sf::Vector2f pos(0.f, 0.f);
        float radius = 50.f;
        sf::Color innerColor = sf::Color::White;
        sf::Color outerColor = sf::Color::Black;
        float thickness = 2.0f;
        int r = 255, g = 255, b = 255, a = 255;

        setting.lookupValue("id", id);

        if (setting.exists("pos")) {
            const libconfig::Setting& posSetting = setting.lookup("pos");
            if (posSetting.getLength() == 2)
                pos = { posSetting[0], posSetting[1] };
        }

        setting.lookupValue("radius", radius);

        if (setting.exists("innerColor")) {
            const libconfig::Setting& color = setting.lookup("innerColor");
            if (color.getLength() >= 3) {
                r = color[0];
                g = color[1];
                b = color[2];
                if (color.getLength() == 4) {
                    a = color[3];
                }
            }
            innerColor = sf::Color(
                static_cast<sf::Uint8>(r),
                static_cast<sf::Uint8>(g),
                static_cast<sf::Uint8>(b),
                static_cast<sf::Uint8>(a)
            );
        }

        if (setting.exists("outerColor")) {
            const libconfig::Setting& color = setting.lookup("outerColor");
            if (color.getLength() >= 3) {
                r = color[0];
                g = color[1];
                b = color[2];
                if (color.getLength() == 4) {
                    a = color[3];
                }
            }
            outerColor = sf::Color(
                static_cast<sf::Uint8>(r),
                static_cast<sf::Uint8>(g),
                static_cast<sf::Uint8>(b),
                static_cast<sf::Uint8>(a)
            );
        }

        setting.lookupValue("outlineThickness", thickness);
        return std::make_shared<CircleElement>(id, pos, radius, innerColor, outerColor, thickness);
    }
    catch (const libconfig::SettingNotFoundException& e) {
        std::cerr << "CircleElement: champ manquant (" << e.getPath() << ")\n";
    }
    catch (const libconfig::SettingTypeException& e) {
        std::cerr << "CircleElement: type invalide (" << e.getPath() << ")\n";
    }

    return nullptr;
}
#endif