#include "../include/ButtonElement.hpp"

ButtonElement::ButtonElement(const std::string& id, const RectangleElement& rectangle, const TextElement& textEl,
                sf::Color normal, sf::Color hover, sf::Color clicked,
                std::function<void()> hoverCb,
                std::function<void()> clickCb)
    :   UIElement(id, ElementTag::BUTTON, rectangle.pos),
        rect(rectangle),
        text(textEl),
        onHover(hoverCb),
        onClick(clickCb),
        normalColor(normal),
        hoverColor(hover),
        clickedColor(clicked)
{
    rect.setInnerColor(normalColor);
}

void ButtonElement::display(sf::RenderWindow& window) {
    rect.display(window);
    text.display(window);
    return;
}

void ButtonElement::display(sf::RenderTexture& window) {
    rect.display(window);
    text.display(window);
}

void ButtonElement::updateState(sf::RenderWindow& window) 
{
    bool over = isMouseOver(window);
    bool isPressed = sf::Mouse::isButtonPressed(sf::Mouse::Left);

    if (over && isPressed && !wasPressed) {
        state = ButtonState::CLICKED;
        wasPressed = true;
    }
    else if (wasPressed && !isPressed) {
        if (over && onClick) {
            onClick();
        }
        wasPressed = false;
        state = over ? ButtonState::HOVER : ButtonState::NORMAL;
    }
    else if (over && !isPressed && state != ButtonState::HOVER) {
        state = ButtonState::HOVER;
        if (onHover) onHover();
    }
    else if (!over && !isPressed && state != ButtonState::NORMAL) {
        state = ButtonState::NORMAL;
    }

    switch (state) {
        case ButtonState::NORMAL: rect.setInnerColor(normalColor); break;
        case ButtonState::HOVER: rect.setInnerColor(hoverColor); break;
        case ButtonState::CLICKED: rect.setInnerColor(clickedColor); break;
    }
}


void ButtonElement::update(EventHandler& eventHandler, sf::RenderWindow& window) {
    updateState(window);
}

ButtonBuilder::ButtonBuilder(const std::string& id) : id(id) {}
    
ButtonBuilder& ButtonBuilder::setText(const std::string& s, const std::string & fontpath) { 
    str = s;
    fontPath = fontpath;
    return *this; 
}
ButtonBuilder& ButtonBuilder::setTextPos(const sf::Vector2f pos) { 
    textPos = pos;
    return *this; 
}
ButtonBuilder& ButtonBuilder::setTextColor(sf::Color c) { textColor = c; return *this; }
ButtonBuilder& ButtonBuilder::setTextSize(unsigned int size) { textSize = size; return *this; }
ButtonBuilder& ButtonBuilder::setPosition(sf::Vector2f p) { pos = p; return *this; }
ButtonBuilder& ButtonBuilder::setSize(sf::Vector2f s) { size = s; return *this; }
ButtonBuilder& ButtonBuilder::setColors(sf::Color normal, sf::Color hover, sf::Color clicked) { 
    normalColor = normal; hoverColor = hover; clickedColor = clicked; return *this; 
}
ButtonBuilder& ButtonBuilder::setOutline(float thickness, sf::Color color) { outlineThickness = thickness; outlineColor = color; return *this; }
ButtonBuilder& ButtonBuilder::setOnHover(std::function<void()> f) { onHover = f; return *this; }
ButtonBuilder& ButtonBuilder::setOnClick(std::function<void()> f) { onClick = f; return *this; }
ButtonElement ButtonBuilder::build() {
    RectangleElement rect(id, pos, size, normalColor, outlineColor, outlineThickness);
    TextElement textEl(id, fontPath, str, textColor, textPos, textSize);
    return ButtonElement(id, rect, textEl, normalColor, hoverColor, clickedColor, onHover, onClick);
}