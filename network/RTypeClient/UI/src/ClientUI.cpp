#include "../include/ClientUI.hpp"
#include "../../Managers/ActionRegistry.hpp"
#include "../include/loader.hpp"

void ClientUI::loadPages()
{
    loadDynamicUI();
}

void ClientUI::init(void) {
    ActionRegistry::getInstance().setDefaultActions();
    loadAllUI();
    UIManager::getInstance().setUI("Homepage");
    window.setFramerateLimit(60);
}
ClientUI::~ClientUI(){};
ClientUI::ClientUI() : window(sf::VideoMode(738, 432), "R-Type") {};