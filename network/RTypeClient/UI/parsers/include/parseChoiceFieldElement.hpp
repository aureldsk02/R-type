#pragma once
#include "../../include/librairies.hpp"
#include "../../components/include/UIElement.hpp"
#include "../../components/include/ChoiceFieldElement.hpp"

std::shared_ptr<UIElement> parseChoiceFieldElement(const libconfig::Setting& setting)
{
    try {
        std::string id;
        std::string fontPath;
        std::string callbackName;
        sf::Vector2f pos(0.f, 0.f);
        sf::Vector2f size(200.f, 50.f);
        sf::Color baseColor(60, 60, 60);
        sf::Color hoverColor(90, 90, 90);
        sf::Color activeColor(100, 170, 255);
        std::vector<std::string> options;
        bool can_display = true;

        setting.lookupValue("id", id);
        setting.lookupValue("font", fontPath);

        if (setting.exists("display")) {
            setting.lookupValue("display", can_display);
        }

        if (setting.exists("pos")) {
            const libconfig::Setting& posSetting = setting.lookup("pos");
            if (posSetting.getLength() == 2) {
                pos.x = static_cast<float>(posSetting[0]);
                pos.y = static_cast<float>(posSetting[1]);
            }
        }

        if (setting.exists("size")) {
            const libconfig::Setting& sizeSetting = setting.lookup("size");
            if (sizeSetting.getLength() == 2) {
                size.x = static_cast<float>(sizeSetting[0]);
                size.y = static_cast<float>(sizeSetting[1]);
            }
        }

        auto parseColor = [](const libconfig::Setting& colorSetting) -> sf::Color {
            int r = 255, g = 255, b = 255, a = 255;
            if (colorSetting.getLength() >= 3) {
                r = colorSetting[0];
                g = colorSetting[1];
                b = colorSetting[2];
                if (colorSetting.getLength() == 4)
                    a = colorSetting[3];
            }
            return sf::Color(
                static_cast<sf::Uint8>(r),
                static_cast<sf::Uint8>(g),
                static_cast<sf::Uint8>(b),
                static_cast<sf::Uint8>(a)
            );
        };

        if (setting.exists("baseColor"))
            baseColor = parseColor(setting.lookup("baseColor"));
        if (setting.exists("hoverColor"))
            hoverColor = parseColor(setting.lookup("hoverColor"));
        if (setting.exists("activeColor"))
            activeColor = parseColor(setting.lookup("activeColor"));

        if (setting.exists("options")) {
            const libconfig::Setting& optionsSetting = setting.lookup("options");
            for (int i = 0; i < optionsSetting.getLength(); ++i) {
                options.push_back(static_cast<const char*>(optionsSetting[i]));
            }
        }

        if (setting.lookupValue("callback", callbackName)) {
        }

        auto element = std::make_shared<ChoiceFieldElement>(
            id, pos, size, fontPath, options,
            baseColor, hoverColor, activeColor, can_display
        );
        return element;
    }
    catch (const libconfig::SettingNotFoundException& e) {
        std::cerr << "Erreur : champ manquant dans ChoiceFieldElement (" << e.getPath() << ")\n";
    }
    catch (const libconfig::SettingTypeException& e) {
        std::cerr << "Erreur : type invalide dans ChoiceFieldElement (" << e.getPath() << ")\n";
    }

    return nullptr;
}
