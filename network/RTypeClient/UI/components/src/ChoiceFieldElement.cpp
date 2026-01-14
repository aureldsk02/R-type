#include "../include/ChoiceFieldElement.hpp"
#include "../../../Managers/FontManager.hpp"

ChoiceFieldElement::ChoiceFieldElement(
    const std::string& id,
    const sf::Vector2f& position,
    const sf::Vector2f& size,
    const std::string& fontpath,
    const std::vector<std::string>& choices,
    const sf::Color& baseColor,
    const sf::Color& hoverColor,
    const sf::Color& activeColor,
    const bool display
) : UIElement(id, ElementTag::CHOICEFIELD, size),
      _choices(choices),
      _baseColor(baseColor),
      _hoverColor(hoverColor),
      _activeColor(activeColor),
      _selectedIndex(0),
      _hoverIndex(-1),
      can_display(display)
{
    float optionWidth = size.x / static_cast<float>(choices.size());
    float optionHeight = size.y;

    for (size_t i = 0; i < choices.size(); ++i) {
        sf::RectangleShape rect;
        rect.setPosition(position.x + i * optionWidth, position.y);
        rect.setSize({ optionWidth - 2.f, optionHeight });
        rect.setFillColor(_baseColor);
        _optionRects.push_back(rect);

        sf::Text text;
        text.setFont(FontManager::getInstance().getFont(fontpath));
        text.setString(choices[i]);
        text.setCharacterSize(18);
        text.setFillColor(sf::Color::White);

        sf::FloatRect textBounds = text.getLocalBounds();
        text.setPosition(
            rect.getPosition().x + (optionWidth - textBounds.width) / 2.f - 2.f,
            rect.getPosition().y + (optionHeight - textBounds.height) / 2.f - 8.f
        );
        _optionTexts.push_back(text);
    }
}

void ChoiceFieldElement::setCallback(std::function<void(int, const std::string&)> callback) {
    _onChoiceSelected = std::move(callback);
}

int ChoiceFieldElement::getSelectedIndex() const { return _selectedIndex; }

std::string ChoiceFieldElement::getSelectedValue() const {
    if (_selectedIndex >= 0 && _selectedIndex < (int)_choices.size())
        return _choices[_selectedIndex];
    return "";
}

void ChoiceFieldElement::update(EventHandler& eventHandler, sf::RenderWindow& window)
{
    if (can_display) {
        for (const auto& event : eventHandler.getEvents()) {
            if (event.type == sf::Event::MouseMoved) {
                sf::Vector2f mouse(event.mouseMove.x, event.mouseMove.y);
                _hoverIndex = -1;
                for (size_t i = 0; i < _optionRects.size(); ++i) {
                    if (_optionRects[i].getGlobalBounds().contains(mouse)) {
                        _hoverIndex = static_cast<int>(i);
                        break;
                    }
                }
                updateColors();
            }

            if (event.type == sf::Event::MouseButtonPressed &&
                event.mouseButton.button == sf::Mouse::Left)
            {
                sf::Vector2f mouse(event.mouseButton.x, event.mouseButton.y);
                for (size_t i = 0; i < _optionRects.size(); ++i) {
                    if (_optionRects[i].getGlobalBounds().contains(mouse)) {
                        _selectedIndex = static_cast<int>(i);
                        updateColors();
                        if (_onChoiceSelected)
                            _onChoiceSelected(_selectedIndex, _choices[_selectedIndex]);
                        break;
                    }
                }
            }
        }
    }
}

void ChoiceFieldElement::display(sf::RenderWindow& window)
{
    if (can_display) {
        for (size_t i = 0; i < _optionRects.size(); ++i) {
            window.draw(_optionRects[i]);
            window.draw(_optionTexts[i]);
        }
    }
    return;
}

void ChoiceFieldElement::display(sf::RenderTexture& window)
{
    if (can_display) {
        for (size_t i = 0; i < _optionRects.size(); ++i) {
            window.draw(_optionRects[i]);
            window.draw(_optionTexts[i]);
        }
    }
    return;
}

void ChoiceFieldElement::updateColors()
{
    for (size_t i = 0; i < _optionRects.size(); ++i) {
        if ((int)i == _selectedIndex)
            _optionRects[i].setFillColor(_activeColor);
        else if ((int)i == _hoverIndex)
            _optionRects[i].setFillColor(_hoverColor);
        else
            _optionRects[i].setFillColor(_baseColor);
    }
}
