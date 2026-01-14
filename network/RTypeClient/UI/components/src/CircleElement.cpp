#include "../include/CircleElement.hpp"

CircleElement::CircleElement(const std::string& id, sf::Vector2f pos, float radius,
    sf::Color inner, sf::Color outer, float thickness = 2.0f)
: UIElement(id, ElementTag::CIRCLE, pos), innerColor(inner), outerColor(outer), outlineThickness(thickness)
{
    circle.setRadius(radius);
    circle.setPosition(pos);
    circle.setFillColor(innerColor);
    circle.setOutlineColor(outerColor);
    circle.setOutlineThickness(outlineThickness);
    circle.setOrigin(radius, radius);
}

void CircleElement::display(sf::RenderWindow& window) {
    window.draw(circle);
}

void CircleElement::display(sf::RenderTexture& window) {
    window.draw(circle);
}

void CircleElement::setPosition(sf::Vector2f pos) {
    this->pos = pos;
    circle.setPosition(pos);
}

void CircleElement::setRadius(float radius) {
    circle.setRadius(radius);
    circle.setOrigin(radius, radius);
}

void CircleElement::setInnerColor(sf::Color color) {
    innerColor = color;
    circle.setFillColor(color);
}

void CircleElement::setOuterColor(sf::Color color) {
    outerColor = color;
    circle.setOutlineColor(color);
}

void CircleElement::setOutlineThickness(float thickness) {
    outlineThickness = thickness;
    circle.setOutlineThickness(thickness);
}

void CircleElement::update(EventHandler& eventHandler, sf::RenderWindow& window) {
    return;
}