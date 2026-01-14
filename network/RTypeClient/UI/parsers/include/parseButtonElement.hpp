#ifndef PARSEBUTTON
    #define PARSEBUTTON
    #include "../../include/librairies.hpp"
    #include "../../components/include/UIElement.hpp"
    #include "../../../Managers/ActionRegistry.hpp"
    #include "../../components/include/ButtonElement.hpp"

std::shared_ptr<UIElement> parseButtonElement(const libconfig::Setting& setting) {
    try {
        std::string id, text, fontPath;
        sf::Vector2f pos(0,0), size(100,50), textPos(0,0);
        sf::Color normalColor = sf::Color::Green;
        sf::Color hoverColor = sf::Color::Yellow;
        sf::Color clickedColor = sf::Color::Red;
        sf::Color outlineColor = sf::Color::White;
        float outlineThickness = 2.0f;

        unsigned int textSize = 20;
        sf::Color textColor = sf::Color::Black;
        int r = 255, g = 255, b = 255, a = 255;


        setting.lookupValue("id", id);
        setting.lookupValue("text", text);
        setting.lookupValue("font", fontPath);

        setting.lookupValue("textSize", textSize);
        if (setting.exists("textColor")) {
            const auto& c = setting.lookup("textColor");
            if (c.getLength() >= 3) {
                r = c[0];
                g = c[1];
                b = c[2];
                if (c.getLength() == 4) {
                    a = c[3];
                }
            }
            textColor = sf::Color(
                static_cast<sf::Uint8>(r),
                static_cast<sf::Uint8>(g),
                static_cast<sf::Uint8>(b),
                static_cast<sf::Uint8>(a)
            );
        }

        if (setting.exists("pos")) {
            const auto& p = setting.lookup("pos");
            pos = {static_cast<float>(p[0]), static_cast<float>(p[1])};
        }
        if (setting.exists("textPos")) {
            const auto& p = setting.lookup("textPos");
            textPos = {static_cast<float>(p[0]), static_cast<float>(p[1])};
        }
        if (setting.exists("size")) {
            const auto& s = setting.lookup("size");
            size = {static_cast<float>(s[0]), static_cast<float>(s[1])};
        }

        if (setting.exists("colors")) {
            const auto& colors = setting.lookup("colors");
            if (colors.exists("normal")) { 
                const auto& c = colors["normal"]; 
                if (c.getLength() >= 3) {
                    r = c[0];
                    g = c[1];
                    b = c[2];
                    if (c.getLength() == 4) {
                        a = c[3];
                    }
                }
                normalColor = sf::Color(
                    static_cast<sf::Uint8>(r),
                    static_cast<sf::Uint8>(g),
                    static_cast<sf::Uint8>(b),
                    static_cast<sf::Uint8>(a)
                ); 
            }
            if (colors.exists("hover")) { 
                const auto& c = colors["hover"]; 
                if (c.getLength() >= 3) {
                    r = c[0];
                    g = c[1];
                    b = c[2];
                    if (c.getLength() == 4) {
                        a = c[3];
                    }
                }
                hoverColor = sf::Color(
                    static_cast<sf::Uint8>(r),
                    static_cast<sf::Uint8>(g),
                    static_cast<sf::Uint8>(b),
                    static_cast<sf::Uint8>(a)
                ); 
            }
            if (colors.exists("clicked")) { 
                const auto& c = colors["clicked"]; 
                if (c.getLength() >= 3) {
                    r = c[0];
                    g = c[1];
                    b = c[2];
                    if (c.getLength() == 4) {
                        a = c[3];
                    }
                }
                clickedColor = sf::Color(
                    static_cast<sf::Uint8>(r),
                    static_cast<sf::Uint8>(g),
                    static_cast<sf::Uint8>(b),
                    static_cast<sf::Uint8>(a)
                ); 
            }
        }

        if (setting.exists("outline")) {
            const auto& outline = setting.lookup("outline");
            outline.lookupValue("thickness", outlineThickness);
            if (outline.exists("color")) {
                const auto& c = outline.lookup("color");
                if (c.getLength() >= 3) {
                    r = c[0];
                    g = c[1];
                    b = c[2];
                    if (c.getLength() == 4) {
                        a = c[3];
                    }
                }
                outlineColor = sf::Color(
                    static_cast<sf::Uint8>(r),
                    static_cast<sf::Uint8>(g),
                    static_cast<sf::Uint8>(b),
                    static_cast<sf::Uint8>(a)
                ); 
            }
        }

        std::function<void()> clickCb = nullptr;
        std::function<void()> hoverCb = nullptr;
        std::string clickId, hoverId;
        setting.lookupValue("onClick", clickId);
        setting.lookupValue("onHover", hoverId);
        if (ActionRegistry::getInstance().get(clickId)) {
            clickCb = ActionRegistry::getInstance().get(clickId);
        }
        if (ActionRegistry::getInstance().get(hoverId)) 
            hoverCb = ActionRegistry::getInstance().get(hoverId);

        ButtonBuilder builder(id);
        builder.setText(text, fontPath)
               .setTextPos(textPos)
               .setPosition(pos)
               .setSize(size)
               .setColors(normalColor, hoverColor, clickedColor)
               .setOutline(outlineThickness, outlineColor)
               .setTextSize(textSize)
               .setTextColor(textColor)
               .setOnClick(clickCb)
               .setOnHover(hoverCb);

        return std::make_shared<ButtonElement>(builder.build());
    }
    catch (const libconfig::SettingNotFoundException& e) {
        std::cerr << "ButtonElement: champ manquant (" << e.getPath() << ")\n";
    }
    catch (const libconfig::SettingTypeException& e) {
        std::cerr << "ButtonElement: type invalide (" << e.getPath() << ")\n";
    }

    return nullptr;
}
#endif