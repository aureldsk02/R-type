/*
** EPITECH PROJECT, 2025
** G-CPP-500-COT-5-1-rtype-20
** File description:
** RoomMenu
*/

#include "RoomMenu.hpp"
#include "FontManager.hpp"

RoomMenu::RoomMenu() : selectedIndex(0), actionSelected(false), selectedAction(""), menuPosition{400, 300}, fontSize(30), normalColor(WHITE), selectedColor(YELLOW), serverIP(""), serverPort(0) {
    options = {"Create Room", "Join Room", "Disconnect"};
}

RoomMenu::~RoomMenu() {
}

void RoomMenu::Initialize() {
    selectedIndex = 0;
    actionSelected = false;
    selectedAction = "";
}

void RoomMenu::Update(InputHandlerSystem& inputHandler) {
    if (inputHandler.IsKeyPressed(InputKey::KEY_DOWN) || inputHandler.IsKeyPressed(InputKey::KEY_S)) {
        selectedIndex = (selectedIndex + 1) % options.size();
        std::cout << "RoomMenu: Option sélectionnée: " << options[selectedIndex] << std::endl;
    } 
    else if (inputHandler.IsKeyPressed(InputKey::KEY_UP) || inputHandler.IsKeyPressed(InputKey::KEY_W)) {
        selectedIndex = (selectedIndex - 1 + options.size()) % options.size();
        std::cout << "RoomMenu: Option sélectionnée: " << options[selectedIndex] << std::endl;
    } 
    else if (inputHandler.IsKeyPressed(InputKey::KEY_ENTER) || inputHandler.IsKeyPressed(InputKey::KEY_SPACE)) {
        actionSelected = true;
        std::cout << "RoomMenu: Action sélectionnée: " << options[selectedIndex] << std::endl;
        
        if (options[selectedIndex] == "Create Room") {
            selectedAction = "createRoom";
        } else if (options[selectedIndex] == "Join Room") {
            selectedAction = "joinRoom";
        } else if (options[selectedIndex] == "Disconnect") {
            selectedAction = "disconnect";
        }
    }
}

void RoomMenu::Render(DisplayHandlerSystem& displayHandler) {
    Vector2 screenSize = displayHandler.GetScreenSize();
    FontManager& fontManager = FontManager::getInstance();
    fontManager.Initialize(displayHandler);
    
    // Background gradient
    for (int i = 0; i < screenSize.y; i++) {
        float alpha = (float)i / screenSize.y;
        Color gradientColor = {
            static_cast<unsigned char>(8 + alpha * 12),
            static_cast<unsigned char>(15 + alpha * 20),
            static_cast<unsigned char>(30 + alpha * 35),
            255
        };
        displayHandler.DrawRectangle({0, (float)i}, {screenSize.x, 1}, gradientColor);
    }
    
    menuPosition.x = screenSize.x / 2;
    menuPosition.y = screenSize.y / 2;

    // Title with pixel font
    Vector2 titlePos = {screenSize.x / 2 - 200, menuPosition.y - 200};
    
    // Title shadow
    TextComponent titleShadow = fontManager.CreateTitleText("GESTION DES SALLES", {titlePos.x + 3, titlePos.y + 3}, Color{0, 0, 0, 180});
    displayHandler.DrawText(titleShadow);
    
    // Main title
    TextComponent titleText = fontManager.CreateTitleText("GESTION DES SALLES", titlePos, Color{100, 255, 100, 255});
    displayHandler.DrawText(titleText);

    // Connection info with pixel font
    std::string connectionInfo = "Connecte a: " + serverIP + ":" + std::to_string(serverPort);
    TextComponent connectionText = fontManager.CreateText(connectionInfo, {screenSize.x / 2 - 150, titlePos.y + 80}, 
                                                        FontManager::PIXEL_REGULAR, FontManager::LARGE, 
                                                        Color{150, 255, 150, 255});
    displayHandler.DrawText(connectionText);

    for (size_t i = 0; i < options.size(); ++i) {
        bool isSelected = (i == static_cast<size_t>(selectedIndex));
        
        Vector2 cardPos = {
            screenSize.x / 2 - 200,
            menuPosition.y - 40 + i * 80
        };
        
        if (isSelected) {
            cardPos.x -= 10;
        }
        
        displayHandler.DrawRectangle({cardPos.x + 5, cardPos.y + 5}, {400, 60}, Color{0, 0, 0, 100});
        
        Color cardColor = isSelected ? Color{50, 80, 50, 240} : Color{30, 40, 30, 220};
        displayHandler.DrawRectangle(cardPos, {400, 60}, cardColor);
        
        // Icon with pixel font
        Vector2 iconPos = {cardPos.x + 20, cardPos.y + 15};
        if (i == 0) {
            TextComponent iconText = fontManager.CreateText("+", iconPos, FontManager::PIXEL_REGULAR, FontManager::TITLE, Color{100, 255, 100, 255});
            displayHandler.DrawText(iconText);
        } else if (i == 1) {
            TextComponent iconText = fontManager.CreateText(">", iconPos, FontManager::PIXEL_REGULAR, FontManager::TITLE, Color{150, 200, 255, 255});
            displayHandler.DrawText(iconText);
        } else {
            TextComponent iconText = fontManager.CreateText("X", iconPos, FontManager::PIXEL_REGULAR, FontManager::TITLE, Color{255, 150, 150, 255});
            displayHandler.DrawText(iconText);
        }
        
        // Option text with pixel font
        Color textColor = isSelected ? Color{255, 255, 255, 255} : Color{200, 200, 200, 255};
        
        if (isSelected) {
            TextComponent shadowText = fontManager.CreateText(options[i], {cardPos.x + 71, cardPos.y + 21}, 
                                                            FontManager::PIXEL_REGULAR, FontManager::XLARGE, 
                                                            Color{100, 255, 100, 100});
            displayHandler.DrawText(shadowText);
        }
        
        TextComponent optionText = fontManager.CreateText(options[i], {cardPos.x + 70, cardPos.y + 20}, 
                                                        FontManager::PIXEL_REGULAR, FontManager::XLARGE, 
                                                        textColor);
        displayHandler.DrawText(optionText);
        
        if (isSelected) {
            TextComponent arrowText = fontManager.CreateText(">>", {cardPos.x + 350, cardPos.y + 20}, 
                                                           FontManager::PIXEL_REGULAR, FontManager::XLARGE, 
                                                           Color{100, 255, 100, 255});
            displayHandler.DrawText(arrowText);
        }
    }
    
    Vector2 descPos = {screenSize.x / 2, menuPosition.y + 120};
    std::string description = "";
    std::string details = "";
    
    if (selectedIndex == 0) {
        description = "CREER UNE SALLE";
        details = "Creez une nouvelle salle de jeu et invitez d'autres joueurs";
    } else if (selectedIndex == 1) {
        description = "REJOINDRE UNE SALLE";
        details = "Rejoignez une salle existante pour jouer avec d'autres";
    } else if (selectedIndex == 2) {
        description = "SE DECONNECTER";
        details = "Fermer la connexion et retourner au menu principal";
    }
    
    // Description panel with pixel font
    if (!description.empty()) {
        displayHandler.DrawRectangle({descPos.x - 250, descPos.y - 10}, {500, 60}, Color{20, 30, 20, 200});
        
        TextComponent descText = fontManager.CreateText(description, {descPos.x - 120, descPos.y}, 
                                                      FontManager::PIXEL_REGULAR, FontManager::LARGE, 
                                                      Color{255, 255, 255, 255});
        displayHandler.DrawText(descText);
                 
        TextComponent detailsText = fontManager.CreateText(details, {descPos.x - 240, descPos.y + 25}, 
                                                         FontManager::PIXEL_REGULAR, FontManager::MEDIUM, 
                                                         Color{180, 180, 180, 255});
        displayHandler.DrawText(detailsText);
    }
    
    // Instructions with pixel font
    displayHandler.DrawRectangle({screenSize.x / 2 - 200, screenSize.y - 60}, {400, 25}, Color{0, 0, 0, 150});
    
    TextComponent instructionsText = fontManager.CreateText("Haut/Bas pour naviguer > ENTREE pour selectionner", 
                                                          {screenSize.x / 2 - 190, screenSize.y - 55}, 
                                                          FontManager::PIXEL_REGULAR, FontManager::MEDIUM, 
                                                          Color{180, 180, 180, 255});
    displayHandler.DrawText(instructionsText);

    // Status with pixel font
    TextComponent statusText = fontManager.CreateText("Status: Connecte et pret", {screenSize.x / 2 - 100, screenSize.y - 30}, 
                                                    FontManager::PIXEL_REGULAR, FontManager::SMALL, 
                                                    Color{100, 255, 100, 255});
    displayHandler.DrawText(statusText);
}

void RoomMenu::Cleanup() {
    options.clear();
}

std::string RoomMenu::GetSelectedAction() const {
    return selectedAction;
}

bool RoomMenu::IsActionSelected() const {
    return actionSelected;
}

void RoomMenu::ResetSelection() {
    actionSelected = false;
    selectedAction = "";
}

void RoomMenu::SetServerInfo(const std::string& ip, int port) {
    serverIP = ip;
    serverPort = port;
}