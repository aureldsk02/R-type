#ifndef EVENTHANDLER
    #define EVENTHANDLER
    #include "./librairies.hpp"

class EventHandler {
    public:
        std::vector<sf::Event> events;
    
        void pollEvents(sf::RenderWindow& window);
    
        const std::vector<sf::Event>& getEvents() const;
};
#endif