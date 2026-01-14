#ifndef UIELEMENT
    #define UIELEMENT
    #include "../../include/librairies.hpp"
    #include "../../include/EventHandler.hpp"

class UIElement {
    public:
        std::string id;
        ElementTag tag;
        sf::Vector2f pos;
    
        UIElement(const std::string& id, ElementTag tag, sf::Vector2f pos)
            : id(id), tag(tag), pos(pos) {}
        virtual ~UIElement() {}
        virtual void display(sf::RenderWindow& window) = 0;
        virtual void display(sf::RenderTexture& window) = 0;
        virtual void update(EventHandler& eventHandler, sf::RenderWindow& window) = 0;
        virtual ElementTag getTag() {return tag;}
};
#endif