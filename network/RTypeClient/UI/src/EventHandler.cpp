#include "../include/EventHandler.hpp"

void EventHandler::pollEvents(sf::RenderWindow& window) {
    events.clear();
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            window.close();
        if (event.type == sf::Event::MouseButtonPressed) {
            if (event.mouseButton.button == sf::Mouse::Left) {                        
                int x = event.mouseButton.x;
                int y = event.mouseButton.y;
                
                std::cout << "Clic Gauche détecté en X: " << x 
                            << ", Y: " << y << std::endl;
            }
        }
        events.push_back(event);
    }
}

const std::vector<sf::Event>& EventHandler::getEvents() const {
    return events;
}