#pragma once
#include "../../include/librairies.hpp"
#include "UIElement.hpp"

class ChoiceFieldElement : public UIElement {
public:
    std::vector<std::string> _choices;
    std::vector<sf::RectangleShape> _optionRects;
    std::vector<sf::Text> _optionTexts;

    sf::Color _baseColor;
    sf::Color _hoverColor;
    sf::Color _activeColor;

    int _selectedIndex;
    int _hoverIndex;
    bool can_display = true;

    std::function<void(int, const std::string&)> _onChoiceSelected;
    
    ChoiceFieldElement(
        const std::string& id,
        const sf::Vector2f& position,
        const sf::Vector2f& size,
        const std::string& fontpath,
        const std::vector<std::string>& choices,
        const sf::Color& baseColor,
        const sf::Color& hoverColor,
        const sf::Color& activeColor,
        const bool display = true
    );

    void setCallback(std::function<void(int, const std::string&)> callback);

    int getSelectedIndex() const;

    std::string getSelectedValue() const;

    void update(EventHandler& eventHandler, sf::RenderWindow& window) override;

    void display(sf::RenderWindow& window) override;

    void display(sf::RenderTexture& window) override;

    void updateColors();
};