#ifndef TEXTELEMENT
    #define TEXTELEMENT
    #include "../../include/librairies.hpp"
    #include "UIElement.hpp"

class TextElement : public UIElement {
    public:
        sf::Text text;
        std::string fontpath;
        std::string content;
    
        TextElement(const std::string& id, const std::string& fontPath, const std::string& str,
            sf::Color color, sf::Vector2f pos, unsigned int size);
    
        void display(sf::RenderWindow& window) override;
    
        void display(sf::RenderTexture& window) override;

        void update(EventHandler& eventHandler, sf::RenderWindow& window) override;
    
        void displayFontPath();

        void setText(const std::string &texte);

        const std::string getContent() const;
};
#endif