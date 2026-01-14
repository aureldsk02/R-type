#ifndef TEXTUREELEMENT
    #define TEXTUREELEMENT
    #include "../../include/librairies.hpp"
    #include "UIElement.hpp"

class TextureElement : public UIElement {
    public:
        sf::Sprite sprite;
        sf::Texture texture;
        sf::Vector2f scale;
    
        TextureElement(const std::string& id, const std::string& texturePath, sf::Vector2f pos, sf::Vector2f scale);
        void display(sf::RenderWindow& window) override;

        void display(sf::RenderTexture& window) override;
        void setPosition(sf::Vector2f pos);
    
        void setScale(sf::Vector2f s);
        void setTexture(const std::string& texturePath);

        void update(EventHandler& eventHandler, sf::RenderWindow& window) override;
};
#endif