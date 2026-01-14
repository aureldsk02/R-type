#include "../include/TextureElement.hpp"

TextureElement::TextureElement(const std::string& id, const std::string& texturePath, sf::Vector2f pos, sf::Vector2f scale = {1.f, 1.f})
    : UIElement(id, ElementTag::TEXTURE, pos), scale(scale)
{
    if (!texture.loadFromFile(texturePath)) {
        std::cerr << "Erreur: impossible de charger la texture " << texturePath << std::endl;
    }
    sprite.setTexture(texture);
    sprite.setPosition(pos);
    sprite.setScale(scale);
}
    
void TextureElement::display(sf::RenderWindow& window) {
    window.draw(sprite);
}

void TextureElement::display(sf::RenderTexture& window) {
    window.draw(sprite);
}

void TextureElement::setPosition(sf::Vector2f pos) {
    this->pos = pos;
    sprite.setPosition(pos);
}

void TextureElement::setScale(sf::Vector2f s) {
    scale = s;
    sprite.setScale(scale);
}

void TextureElement::setTexture(const std::string& texturePath) {
    if (!texture.loadFromFile(texturePath)) {
        std::cerr << "Erreur: impossible de charger la texture " << texturePath << std::endl;
        return;
    }
    sprite.setTexture(texture);
}

void TextureElement::update(EventHandler& eventHandler, sf::RenderWindow& window) {
    return;
}
