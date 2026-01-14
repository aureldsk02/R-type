#ifndef CIRCLEELEMENT
    #define CIRCLEELEMENT
    #include "../../include/librairies.hpp"
    #include "UIElement.hpp"

class CircleElement : public UIElement {
    public:
        sf::CircleShape circle;
        sf::Color innerColor;
        sf::Color outerColor;
        float outlineThickness;
    
        CircleElement(const std::string& id, sf::Vector2f pos, float radius,
            sf::Color inner, sf::Color outer, float thickness);
    
        void display(sf::RenderWindow& window) override;

        void display(sf::RenderTexture& window) override;
    
        void setPosition(sf::Vector2f pos);
    
        void setRadius(float radius);
    
        void setInnerColor(sf::Color color);
    
        void setOuterColor(sf::Color color);
    
        void setOutlineThickness(float thickness);

        void update(EventHandler& eventHandler, sf::RenderWindow& window) override;
};
#endif