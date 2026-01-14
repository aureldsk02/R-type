#include "../include/InputFieldElement.hpp"
#include "../../../Managers/FontManager.hpp"

InputFieldElement::InputFieldElement(const std::string& id, const std::string& fontPath, sf::Vector2f pos, sf::Vector2f sizes,
    bool password, sf::Color activeColor, bool remapMode, unsigned int characterSize, bool display)
    : UIElement(id, ElementTag::INPUTFIELD, pos),
        rect(id, pos, sizes, sf::Color::White, sf::Color::Black, 2.0f),
        isPassword(password), activeOutlineColor(activeColor), isRemapMode(remapMode), can_display(display)
{
    size = characterSize;
    text.setFont(FontManager::getInstance().getFont(fontPath));
    text.setFillColor(sf::Color::Black);
    text.setPosition(pos + sf::Vector2f(5, 5));
    text.setCharacterSize(size);
}

void InputFieldElement::addChar(char c) {
    if (isRemapMode) {
        content = std::string(1, static_cast<char>(std::toupper(c)));
    } else {
        content += c;
    }
    text.setString(isPassword ? std::string(content.size(), '*') : content);
}

void InputFieldElement::backspace() {
    if (!content.empty()) {
        content.pop_back();
        text.setString(isPassword ? std::string(content.size(), '*') : content);
    }
}

static std::string keyToReadable(sf::Keyboard::Key key) {
    static const std::unordered_map<sf::Keyboard::Key, std::string> keyNames = {
        {sf::Keyboard::Up, "UP"},
        {sf::Keyboard::Down, "DOWN"},
        {sf::Keyboard::Left, "LEFT"},
        {sf::Keyboard::Right, "RIGHT"},
        {sf::Keyboard::Space, "SPACE"},
        {sf::Keyboard::Enter, "ENTER"},
        {sf::Keyboard::Escape, "ESC"}
    };

    auto it = keyNames.find(key);
    if (it != keyNames.end())
        return it->second;

    if (key >= sf::Keyboard::A && key <= sf::Keyboard::Z)
        return std::string(1, static_cast<char>('A' + (key - sf::Keyboard::A)));

    if (key >= sf::Keyboard::Num0 && key <= sf::Keyboard::Num9)
        return std::string(1, static_cast<char>('0' + (key - sf::Keyboard::Num0)));

    return "?";
}

void InputFieldElement::updateActive(sf::RenderWindow& window) {
    sf::Vector2f mousePos = (sf::Vector2f)sf::Mouse::getPosition(window);
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
        if (rect.rect.getGlobalBounds().contains(mousePos)) {
            active = true;
        } else {
            active = false;
        }
    }
    rect.setInnerColor(rect.innerColor);
    rect.setOuterColor(active ? activeOutlineColor : sf::Color::Black);
}

void InputFieldElement::handleInput(const sf::Event& event) {
    if (!active)
        return;
    
    if (isRemapMode) {
        if (event.type == sf::Event::KeyPressed) {
            sf::Keyboard::Key key = event.key.code;
            content = keyToReadable(key);
            text.setString(content);
        }
        return;
    }
    if (event.type == sf::Event::TextEntered) {
        if (event.text.unicode >= 32 && event.text.unicode <= 126) {
            addChar(static_cast<char>(event.text.unicode));
        }
    } else if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::Backspace) {
            backspace();
        }
    }
}

void InputFieldElement::update(EventHandler& eventHandler, sf::RenderWindow& window) {
    if (can_display) {
        updateActive(window);
        for (const auto& event : eventHandler.getEvents()) {
            handleInput(event);
        }
    }
}

void InputFieldElement::display(sf::RenderWindow& window) {
    if (can_display) {
        rect.display(window);
        window.draw(text);
    }
}

void InputFieldElement::display(sf::RenderTexture& window) {
    if (can_display) {
        rect.display(window);
        window.draw(text);
    }
}

const std::string &InputFieldElement::getText() const {
    return content;
}

void InputFieldElement::setText(const std::string &texte) {
    content = texte;
    text.setString(texte);
}