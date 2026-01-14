#ifndef RECTANGLEELEMENT
    #define RECTANGLEELEMENT
    #include "../../include/librairies.hpp"
    #include "UIElement.hpp"

class RectangleElement : public UIElement {
    public:
        sf::RectangleShape rect;
        sf::Color innerColor;
        sf::Color outerColor;
        float outlineThickness;
    
        RectangleElement(const std::string& id, sf::Vector2f pos, sf::Vector2f size,
            sf::Color inner, sf::Color outer, float thickness);
    
        void display(sf::RenderWindow& window) override;

        void display(sf::RenderTexture& window) override;
    
        void setPosition(sf::Vector2f pos);
    
        void setSize(sf::Vector2f size);
    
        void setInnerColor(sf::Color color);
    
        void setOuterColor(sf::Color color);
    
        void setOutlineThickness(float thickness);

        void update(EventHandler& eventHandler, sf::RenderWindow& window) override;
};
#endif