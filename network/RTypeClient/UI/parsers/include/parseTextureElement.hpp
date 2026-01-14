#ifndef PARSETEXTURE
    #define PARSETEXTURE
    #include "../../include/librairies.hpp"
    #include "../../components/include/UIElement.hpp"
    #include "../../components/include/TextureElement.hpp"

std::shared_ptr<UIElement> parseTextureElement(const libconfig::Setting& setting) {
    try {
        std::string id;
        std::string texturePath;
        sf::Vector2f pos(0.f, 0.f);
        sf::Vector2f scale(1.f, 1.f);

        setting.lookupValue("id", id);
        setting.lookupValue("texturePath", texturePath);

        if (setting.exists("pos")) {
            const libconfig::Setting& posSetting = setting.lookup("pos");
            if (posSetting.getLength() == 2)
                pos = { static_cast<float>(posSetting[0]), static_cast<float>(posSetting[1]) };
        }

        if (setting.exists("scale")) {
            const libconfig::Setting& scaleSetting = setting.lookup("scale");
            if (scaleSetting.getLength() == 2)
                scale = { static_cast<float>(scaleSetting[0]), static_cast<float>(scaleSetting[1]) };
        }

        return std::make_shared<TextureElement>(id, texturePath, pos, scale);
    }
    catch (const libconfig::SettingNotFoundException& e) {
        std::cerr << "TextureElement: champ manquant (" << e.getPath() << ")\n";
    }
    catch (const libconfig::SettingTypeException& e) {
        std::cerr << "TextureElement: type invalide (" << e.getPath() << ")\n";
    }

    return nullptr;
}
#endif