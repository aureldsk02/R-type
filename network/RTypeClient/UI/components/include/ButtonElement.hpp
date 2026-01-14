#ifndef BUTTONELEMENT
    #define BUTTONELEMENT
    #include "../../include/librairies.hpp"
    #include "RectangleElement.hpp"
    #include "TextElement.hpp"


class ButtonElement : public UIElement {
    public:
        RectangleElement rect;
        TextElement text;
        std::function<void()> onHover;
        std::function<void()> onClick;
        ButtonState state = ButtonState::NORMAL;
    
        sf::Color normalColor;
        sf::Color hoverColor;
        sf::Color clickedColor;
    
        ButtonElement(const std::string& id, const RectangleElement& rectangle, const TextElement& textEl,
                      sf::Color normal, sf::Color hover, sf::Color clicked,
                      std::function<void()> hoverCb = nullptr,
                      std::function<void()> clickCb = nullptr);
    
        void display(sf::RenderWindow& window) override;

        void display(sf::RenderTexture& window) override;
    
        void updateState(sf::RenderWindow& window);

        void update(EventHandler& eventHandler, sf::RenderWindow& window) override;
    
    private:
        bool isMouseOver(sf::RenderWindow& window) {
            sf::Vector2f mousePos = (sf::Vector2f)sf::Mouse::getPosition(window);
            return rect.rect.getGlobalBounds().contains(mousePos);
        }
        bool wasPressed = false;
};
    
class ButtonBuilder {
    private:
        std::string id;
        std::string str = "Button";
        std::string fontPath;
        sf::Font font;
        sf::Vector2f pos = {0,0};
        sf::Vector2f textPos = {0,0};
        sf::Vector2f size = {100,50};
        sf::Color normalColor = sf::Color::Green;
        sf::Color hoverColor = sf::Color::Yellow;
        sf::Color clickedColor = sf::Color::Red;
        sf::Color textColor = sf::Color::Black;
        unsigned int textSize = 20;
        float outlineThickness = 2.0f;
        sf::Color outlineColor = sf::Color::White;
        std::function<void()> onHover = nullptr;
        std::function<void()> onClick = nullptr;
    
    public:
        ButtonBuilder(const std::string& id);
    
        ButtonBuilder& setText(const std::string& s, const std::string & fontpath);

        ButtonBuilder& setTextPos(const sf::Vector2f pos);
        ButtonBuilder& setTextColor(sf::Color c);
        ButtonBuilder& setTextSize(unsigned int size);
        ButtonBuilder& setPosition(sf::Vector2f p);
        ButtonBuilder& setSize(sf::Vector2f s);
        ButtonBuilder& setColors(sf::Color normal, sf::Color hover, sf::Color clicked);
        ButtonBuilder& setOutline(float thickness, sf::Color color);
        ButtonBuilder& setOnHover(std::function<void()> f);
        ButtonBuilder& setOnClick(std::function<void()> f);
    
        ButtonElement build();
};
#endif