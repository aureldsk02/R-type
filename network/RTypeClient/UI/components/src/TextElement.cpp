#include "../include/TextElement.hpp"
#include "../../../Managers/FontManager.hpp"

TextElement::TextElement(const std::string& id, const std::string& fontPath, const std::string& str,
    sf::Color color, sf::Vector2f pos, unsigned int size)
: UIElement(id, ElementTag::TEXT, pos)
{
    fontpath = fontPath;
    content = str;
    text.setFont(FontManager::getInstance().getFont(fontpath));
    text.setString(str);
    text.setFillColor(color);
    text.setCharacterSize(size);
    text.setPosition(pos);
}

void TextElement::display(sf::RenderWindow& window) {
    window.draw(text);
}

void TextElement::display(sf::RenderTexture& window) {
    window.draw(text);
}

void TextElement::update(EventHandler& eventHandler, sf::RenderWindow& window) {
    return;
}

void TextElement::displayFontPath() {std::cout << fontpath << std::endl;}
void TextElement::setText(const std::string &texte) { text.setString(texte); content = texte;}
const std::string TextElement::getContent() const {
    return content;
}