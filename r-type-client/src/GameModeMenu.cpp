/*
** EPITECH PROJECT, 2025
** G-CPP-500-COT-5-1-rtype-20
** File description:
** GameModeMenu
*/

#include "GameModeMenu.hpp"
#include "FontManager.hpp"

GameModeMenu::GameModeMenu() : selectedIndex(0), actionSelected(false), selectedAction(""), menuPosition{400, 300}, fontSize(30), normalColor(WHITE), selectedColor(YELLOW) {
    options = {"Solo", "Multijoueur", "Retour"};
}

GameModeMenu::~GameModeMenu() {
}

void GameModeMenu::Initialize() {
    selectedIndex = 0;
    actionSelected = false;
    selectedAction = "";
}

void GameModeMenu::Update(InputHandlerSystem& inputHandler) {
    if (inputHandler.IsKeyPressed(InputKey::KEY_DOWN) || inputHandler.IsKeyPressed(InputKey::KEY_S)) {
        selectedIndex = (selectedIndex + 1) % options.size();
        std::cout << "GameModeMenu: Option sélectionnée: " << options[selectedIndex] << std::endl;
    } 
    else if (inputHandler.IsKeyPressed(InputKey::KEY_UP) || inputHandler.IsKeyPressed(InputKey::KEY_W)) {
        selectedIndex = (selectedIndex - 1 + options.size()) % options.size();
        std::cout << "GameModeMenu: Option sélectionnée: " << options[selectedIndex] << std::endl;
    } 
    else if (inputHandler.IsKeyPressed(InputKey::KEY_ENTER) || inputHandler.IsKeyPressed(InputKey::KEY_SPACE)) {
        actionSelected = true;
        std::cout << "GameModeMenu: Action sélectionnée: " << options[selectedIndex] << std::endl;
        
        if (options[selectedIndex] == "Solo") {
            selectedAction = "startSolo";
        } else if (options[selectedIndex] == "Multijoueur") {
            selectedAction = "startMultiplayer";
        } else if (options[selectedIndex] == "Retour") {
            selectedAction = "backToMain";
        }
    }
}

void GameModeMenu::Render(DisplayHandlerSystem& displayHandler) {
    Vector2 screenSize = displayHandler.GetScreenSize();
    FontManager& fontManager = FontManager::getInstance();
    fontManager.Initialize(displayHandler);
    
    // Background gradient
    for (int i = 0; i < screenSize.y; i++) {
        float alpha = (float)i / screenSize.y;
        Color gradientColor = {
            static_cast<unsigned char>(5 + alpha * 15),
            static_cast<unsigned char>(10 + alpha * 20),
            static_cast<unsigned char>(25 + alpha * 35),
            255
        };
        displayHandler.DrawRectangle({0, (float)i}, {screenSize.x, 1}, gradientColor);
    }
    
    menuPosition.x = screenSize.x / 2;
    menuPosition.y = screenSize.y / 2;

    // Title with pixel font
    Vector2 titlePos = {screenSize.x / 2 - 200, menuPosition.y - 180};
    
    // Title shadow
    TextComponent titleShadow = fontManager.CreateTitleText("SELECTION DE MODE", {titlePos.x + 3, titlePos.y + 3}, Color{0, 0, 0, 180});
    displayHandler.DrawText(titleShadow);
    
    // Main title
    TextComponent titleText = fontManager.CreateTitleText("SELECTION DE MODE", titlePos, Color{100, 200, 255, 255});
    displayHandler.DrawText(titleText);

    for (size_t i = 0; i < options.size(); ++i) {
        bool isSelected = (i == static_cast<size_t>(selectedIndex));
        
        Vector2 cardPos = {
            screenSize.x / 2 - 150,
            menuPosition.y - 40 + i * 80
        };
        
        if (isSelected) {
            cardPos.x -= 10;
        }
        
        displayHandler.DrawRectangle({cardPos.x + 5, cardPos.y + 5}, {300, 60}, Color{0, 0, 0, 100});
        
        Color cardColor = isSelected ? Color{40, 60, 100, 240} : Color{30, 35, 50, 220};
        displayHandler.DrawRectangle(cardPos, {300, 60}, cardColor);
        
        // Icon for each mode with pixel font
        Vector2 iconPos = {cardPos.x + 20, cardPos.y + 15};
        if (i == 0) {
            TextComponent iconText = fontManager.CreateText("1P", iconPos, FontManager::PIXEL_REGULAR, FontManager::TITLE, Color{255, 200, 100, 255});
            displayHandler.DrawText(iconText);
        } else if (i == 1) {
            TextComponent iconText = fontManager.CreateText("MP", iconPos, FontManager::PIXEL_REGULAR, FontManager::TITLE, Color{100, 255, 150, 255});
            displayHandler.DrawText(iconText);
        } else {
            TextComponent iconText = fontManager.CreateText("<", iconPos, FontManager::PIXEL_REGULAR, FontManager::TITLE, Color{200, 200, 200, 255});
            displayHandler.DrawText(iconText);
        }
        
        // Option text with pixel font
        Color textColor = isSelected ? Color{255, 255, 255, 255} : Color{200, 200, 200, 255};
        
        if (isSelected) {
            TextComponent shadowText = fontManager.CreateText(options[i], {cardPos.x + 71, cardPos.y + 21}, 
                                                            FontManager::PIXEL_REGULAR, FontManager::XLARGE, 
                                                            Color{100, 200, 255, 100});
            displayHandler.DrawText(shadowText);
        }
        
        TextComponent optionText = fontManager.CreateText(options[i], {cardPos.x + 70, cardPos.y + 20}, 
                                                        FontManager::PIXEL_REGULAR, FontManager::XLARGE, 
                                                        textColor);
        displayHandler.DrawText(optionText);
        
        if (isSelected) {
            TextComponent arrowText = fontManager.CreateText("►", {cardPos.x + 260, cardPos.y + 20}, 
                                                           FontManager::PIXEL_REGULAR, FontManager::XLARGE, 
                                                           Color{100, 200, 255, 255});
            displayHandler.DrawText(arrowText);
        }
    }
    
    Vector2 descPos = {screenSize.x / 2, menuPosition.y + 120};
    std::string description = "";
    std::string details = "";
    
    if (selectedIndex == 0) {
        description = "MODE SOLO";
        details = "Affrontez des vagues d'ennemis contrôlés par l'IA";
    } else if (selectedIndex == 1) {
        description = "MODE MULTIJOUEUR";
        details = "Jusqu'à 4 joueurs en coopération via réseau";
    } else if (selectedIndex == 2) {
        description = "RETOUR";
        details = "Retourner au menu principal";
    }
    
    // Description panel with pixel font
    if (!description.empty()) {
        displayHandler.DrawRectangle({descPos.x - 200, descPos.y - 10}, {400, 60}, Color{20, 25, 40, 200});
        
        TextComponent descText = fontManager.CreateText(description, {descPos.x - 100, descPos.y}, 
                                                      FontManager::PIXEL_REGULAR, FontManager::LARGE, 
                                                      Color{255, 255, 255, 255});
        displayHandler.DrawText(descText);
                 
        TextComponent detailsText = fontManager.CreateText(details, {descPos.x - 180, descPos.y + 25}, 
                                                         FontManager::PIXEL_REGULAR, FontManager::MEDIUM, 
                                                         Color{180, 180, 180, 255});
        displayHandler.DrawText(detailsText);
    }
    
    // Instructions with pixel font
    displayHandler.DrawRectangle({screenSize.x / 2 - 250, screenSize.y - 60}, {500, 25}, Color{0, 0, 0, 150});
    
    TextComponent instructionsText = fontManager.CreateText("Haut/Bas ou W/S pour naviguer > ENTREE pour selectionner", 
                                                          {screenSize.x / 2 - 240, screenSize.y - 55}, 
                                                          FontManager::PIXEL_REGULAR, FontManager::MEDIUM, 
                                                          Color{180, 180, 180, 255});
    displayHandler.DrawText(instructionsText);
}

void GameModeMenu::Cleanup() {
    options.clear();
}

std::string GameModeMenu::GetSelectedAction() const {
    return selectedAction;
}

bool GameModeMenu::IsActionSelected() const {
    return actionSelected;
}

void GameModeMenu::ResetSelection() {
    actionSelected = false;
    selectedAction = "";
}