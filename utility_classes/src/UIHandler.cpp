/* ------------------------------------------------------------------------------------ *
 *                                                                                      *
 * EPITECH PROJECT - Fri, Oct, 2025                                                     *
 * Title           - G-CPP-500-COT-5-1-rtype-8                                          *
 * Description     -                                                                    *
 *     UIHandler                                                                        *
 *                                                                                      *
 * ------------------------------------------------------------------------------------ *
 *                                                                                      *
 *             ███████╗██████╗ ██╗████████╗███████╗ ██████╗██╗  ██╗                     *
 *             ██╔════╝██╔══██╗██║╚══██╔══╝██╔════╝██╔════╝██║  ██║                     *
 *             █████╗  ██████╔╝██║   ██║   █████╗  ██║     ███████║                     *
 *             ██╔══╝  ██╔═══╝ ██║   ██║   ██╔══╝  ██║     ██╔══██║                     *
 *             ███████╗██║     ██║   ██║   ███████╗╚██████╗██║  ██║                     *
 *             ╚══════╝╚═╝     ╚═╝   ╚═╝   ╚══════╝ ╚═════╝╚═╝  ╚═╝                     *
 *                                                                                      *
 * ------------------------------------------------------------------------------------ */

#include "UIHandler.hpp"
#include "../../rtype_gameInstance/include/gameInstance.hpp"


UIHandler::UIHandler(const std::string &text, bool fade_in)
{
    fade_ended = false;
    fade_started = false;
    t = 0.0;
    this->text.setString(text);
    this->text.setFont(ResourceManager::Instance().getFont("arcade"));
    this->text.setFillColor(sf::Color::White);
    this->text.setCharacterSize(30);
    this->text.setPosition(((WINDOW_WIDTH - (text.length() * 25)) / 2), (WINDOW_HEIGHT / 2) - 30);

    this->fade_rect.setSize(sf::Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT));
    if (fade_in)
        this->fade_rect.setFillColor(sf::Color(0, 0, 0, 0));
    else
        this->fade_rect.setFillColor(sf::Color(0, 0, 0, 255));
    this->fade_rect.setPosition(0, 0);
}

void UIHandler::fade_in(sf::RenderWindow &window, double delta)
{
    fade_started = true;
    t += delta;
    this->fade_rect.setFillColor(sf::Color(0, 0, 0, (t * 255) / 3));
    this->text.setFillColor(sf::Color(255, 255, 255, (t * 255) / 3));
    
    if (fade_rect.getFillColor().a >= 255) {
        fade_ended = true;
        fade_started = false;
    }
}

void UIHandler::fade_out(sf::RenderWindow &window, double delta)
{
    fade_started = true;
    t += delta;
    this->fade_rect.setFillColor(sf::Color(0, 0, 0, 255 - ((t * 255) / 3)));
    this->text.setFillColor(sf::Color(255, 255, 255, 255 - ((t * 255) / 3)));

    if (fade_rect.getFillColor().a <= 0) {
        fade_ended = true;
        fade_started = false;
    }
}

void UIHandler::draw(sf::RenderWindow &window)
{
    window.draw(this->fade_rect);
    window.draw(this->text);
    window.display();
}
