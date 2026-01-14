#include "../include/RectangleElement.hpp"

RectangleElement::RectangleElement(const std::string& id, sf::Vector2f pos, sf::Vector2f size,
    sf::Color inner, sf::Color outer, float thickness = 2.0f)
: UIElement(id, ElementTag::RECTANGLE, pos), innerColor(inner), outerColor(outer), outlineThickness(thickness)
{
    rect.setPosition(pos);
    rect.setSize(size);
    rect.setFillColor(innerColor);
    rect.setOutlineColor(outerColor);
    rect.setOutlineThickness(outlineThickness);
}

void RectangleElement::display(sf::RenderWindow& window) {
    window.draw(rect);
}

void RectangleElement::display(sf::RenderTexture& window) {
    window.draw(rect);
}

void RectangleElement::setPosition(sf::Vector2f pos) {
    rect.setPosition(pos);
}

void RectangleElement::setSize(sf::Vector2f size) {
    rect.setSize(size);
}

void RectangleElement::setInnerColor(sf::Color color) {
    innerColor = color;
    rect.setFillColor(color);
}

void RectangleElement::setOuterColor(sf::Color color) {
    outerColor = color;
    rect.setOutlineColor(color);
}

void RectangleElement::setOutlineThickness(float thickness) {
    outlineThickness = thickness;
    rect.setOutlineThickness(thickness);
}

void RectangleElement::update(EventHandler& eventHandler, sf::RenderWindow& window) {
    return;
}