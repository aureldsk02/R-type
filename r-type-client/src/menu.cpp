/*
** EPITECH PROJECT, 2025
** G-CPP-500-COT-5-1-rtype-20 [WSL: Ubuntu-24.04]
** File description:
** Menu
*/

#include "menu.hpp"
#include "FontManager.hpp"

Menu::Menu() : selectedIndex(0), actionSelected(false), selectedAction(""), menuPosition{400, 300}, fontSize(30), normalColor(WHITE), selectedColor(YELLOW) {
    options = {"Start Game", "Settings", "Quit Game"};
}

Menu::~Menu() {
}

void Menu::Initialize() {
    selectedIndex = 0;
    actionSelected = false;
    selectedAction = "";
}

void Menu::Update(InputHandlerSystem& inputHandler) {
    if (inputHandler.IsKeyPressed(InputKey::KEY_DOWN) || inputHandler.IsKeyPressed(InputKey::KEY_S)) {
        selectedIndex = (selectedIndex + 1) % options.size();
        std::cout << "Menu: Option sélectionnée: " << options[selectedIndex] << std::endl;
    } 
    else if (inputHandler.IsKeyPressed(InputKey::KEY_UP) || inputHandler.IsKeyPressed(InputKey::KEY_W)) {
        selectedIndex = (selectedIndex - 1 + options.size()) % options.size();
        std::cout << "Menu: Option sélectionnée: " << options[selectedIndex] << std::endl;
    } 
    else if (inputHandler.IsKeyPressed(InputKey::KEY_ENTER) || inputHandler.IsKeyPressed(InputKey::KEY_SPACE)) {
        actionSelected = true;
        std::cout << "Menu: Action sélectionnée: " << options[selectedIndex] << std::endl;
        
        if (options[selectedIndex] == "Start Game") {
            selectedAction = "startGame";
        } else if (options[selectedIndex] == "Settings") {
            selectedAction = "openSettings";
        } else if (options[selectedIndex] == "Quit Game") {
            selectedAction = "quitGame";
        }
    }
}

void Menu::Render(DisplayHandlerSystem& displayHandler) {
    Vector2 screenSize = displayHandler.GetScreenSize();
    FontManager& fontManager = FontManager::getInstance();
    fontManager.Initialize(displayHandler);
    
    // Background gradient
    for (int i = 0; i < screenSize.y; i++) {
        float alpha = (float)i / screenSize.y;
        Color gradientColor = {
            static_cast<unsigned char>(10 + alpha * 20),
            static_cast<unsigned char>(15 + alpha * 25),
            static_cast<unsigned char>(35 + alpha * 40),
            255
        };
        displayHandler.DrawRectangle({0, (float)i}, {screenSize.x, 1}, gradientColor);
    }
    
    menuPosition.x = screenSize.x / 2;
    menuPosition.y = screenSize.y / 2;

    // Title with shadow effect using pixel font - BIGGER SIZE AND CENTERED
    Vector2 titlePos = {screenSize.x / 2 - 140, menuPosition.y - 250};
    
    // Title shadow
    TextComponent titleShadow = fontManager.CreateText("R-TYPE", {titlePos.x + 6, titlePos.y + 6}, 
                                                      FontManager::PIXEL_REGULAR, FontManager::GIGANTIC, 
                                                      Color{0, 0, 0, 180});
    displayHandler.DrawText(titleShadow);
    
    // Main title
    TextComponent titleText = fontManager.CreateText("R-TYPE", titlePos, 
                                                   FontManager::PIXEL_REGULAR, FontManager::GIGANTIC, 
                                                   Color{255, 140, 0, 255});
    displayHandler.DrawText(titleText);
    
    // Subtitle
    TextComponent subtitleText = fontManager.CreateText("HORIZONTAL SHOOT'EM UP", 
                                                       {screenSize.x / 2 - 140, titlePos.y + 80}, 
                                                       FontManager::PIXEL_REGULAR, FontManager::LARGE, 
                                                       Color{180, 180, 180, 255});
    displayHandler.DrawText(subtitleText);

    // Menu box background
    int menuBoxWidth = 400;
    int menuBoxHeight = options.size() * 70 + 60;
    Vector2 menuBoxPos = {
        screenSize.x / 2 - menuBoxWidth / 2,
        menuPosition.y - 60
    };
    
    displayHandler.DrawRectangle({menuBoxPos.x - 20, menuBoxPos.y - 20}, {(float)(menuBoxWidth + 40), (float)(menuBoxHeight + 40)}, Color{20, 25, 40, 220});
    
    // Menu options with pixel font
    for (size_t i = 0; i < options.size(); ++i) {
        Vector2 textPos = {
            screenSize.x / 2 - 100,
            menuPosition.y + i * 70 - 10
        };
        
        bool isSelected = (i == static_cast<size_t>(selectedIndex));
        
        if (isSelected) {
            int animWidth = 350;
            displayHandler.DrawRectangle({textPos.x - 75, textPos.y - 15}, {(float)animWidth, 50}, Color{255, 140, 0, 80});
        }
        
        // Menu option text with pixel font
        TextComponent menuText = fontManager.CreateMenuText(options[i], textPos, isSelected, 
                                                          Color{200, 200, 200, 255}, Color{255, 255, 255, 255});
        displayHandler.DrawText(menuText);
    }
    
    // Instructions with pixel font
    displayHandler.DrawRectangle({screenSize.x / 2 - 250, screenSize.y - 70}, {500, 30}, Color{0, 0, 0, 150});
    
    TextComponent instructionsText = fontManager.CreateText("Haut/Bas ou WASD pour naviguer > ENTREE pour selectionner", 
                                                          {screenSize.x / 2 - 240, screenSize.y - 65}, 
                                                          FontManager::PIXEL_REGULAR, FontManager::MEDIUM, 
                                                          Color{180, 180, 180, 255});
    displayHandler.DrawText(instructionsText);
    
    // Version with pixel font
    TextComponent versionText = fontManager.CreateText("v1.0.0", {20, screenSize.y - 30}, 
                                                     FontManager::PIXEL_REGULAR, FontManager::SMALL, 
                                                     Color{120, 120, 120, 255});
    displayHandler.DrawText(versionText);
}

void Menu::Cleanup() {
    options.clear();
}

std::string Menu::GetSelectedAction() const {
    return selectedAction;
}

bool Menu::IsActionSelected() const {
    return actionSelected;
}

void Menu::ResetSelection() {
    actionSelected = false;
    selectedAction = "";
}