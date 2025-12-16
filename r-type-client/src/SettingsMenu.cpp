/*
** EPITECH PROJECT, 2025
** G-CPP-500-COT-5-1-rtype-20
** File description:
** SettingsMenu
*/

#include "SettingsMenu.hpp"
#include "FontManager.hpp"
#include <algorithm>
#include <sstream>
#include <iomanip>

SettingsMenu::SettingsMenu() 
    : currentField(FIELD_BRIGHTNESS), actionSelected(false), selectedAction(""),
      menuPosition{400, 300}, fontSize(24), normalColor(WHITE), selectedColor(YELLOW),
      brightness(0.8f), volume(0.7f) {
}

SettingsMenu::~SettingsMenu() {
}

void SettingsMenu::Initialize() {
    currentField = FIELD_BRIGHTNESS;
    actionSelected = false;
    selectedAction = "";
    brightness = 0.8f;
    volume = 0.7f;
}

void SettingsMenu::Update(InputHandlerSystem& inputHandler) {
    if (inputHandler.IsKeyPressed(InputKey::KEY_DOWN) || inputHandler.IsKeyPressed(InputKey::KEY_S)) {
        currentField = static_cast<SettingsField>((currentField + 1) % 4);
        std::cout << "SettingsMenu: Champ sélectionné: " << currentField << std::endl;
    } 
    else if (inputHandler.IsKeyPressed(InputKey::KEY_UP) || inputHandler.IsKeyPressed(InputKey::KEY_W)) {
        currentField = static_cast<SettingsField>((currentField - 1 + 4) % 4);
        std::cout << "SettingsMenu: Champ sélectionné: " << currentField << std::endl;
    }
    else if (inputHandler.IsKeyPressed(InputKey::KEY_LEFT) || inputHandler.IsKeyPressed(InputKey::KEY_A)) {
        if (currentField == FIELD_BRIGHTNESS) {
            adjustBrightness(false);
        } else if (currentField == FIELD_VOLUME) {
            adjustVolume(false);
        }
    }
    else if (inputHandler.IsKeyPressed(InputKey::KEY_RIGHT) || inputHandler.IsKeyPressed(InputKey::KEY_D)) {
        if (currentField == FIELD_BRIGHTNESS) {
            adjustBrightness(true);
        } else if (currentField == FIELD_VOLUME) {
            adjustVolume(true);
        }
    }
    else if (inputHandler.IsKeyPressed(InputKey::KEY_ENTER) || inputHandler.IsKeyPressed(InputKey::KEY_SPACE)) {
        actionSelected = true;
        
        if (currentField == FIELD_HELP) {
            selectedAction = "showHelp";
        } else if (currentField == FIELD_BACK_SETTINGS) {
            selectedAction = "backToMain";
        }
        
        std::cout << "SettingsMenu: Action sélectionnée: " << selectedAction << std::endl;
    }
}

void SettingsMenu::adjustBrightness(bool increase) {
    float step = 0.1f;
    if (increase) {
        brightness = std::min(1.0f, brightness + step);
    } else {
        brightness = std::max(0.1f, brightness - step);
    }
    std::cout << "Brightness adjusted to: " << brightness << std::endl;
}

void SettingsMenu::adjustVolume(bool increase) {
    float step = 0.1f;
    if (increase) {
        volume = std::min(1.0f, volume + step);
    } else {
        volume = std::max(0.0f, volume - step);
    }
    std::cout << "Volume adjusted to: " << volume << std::endl;
}

std::string SettingsMenu::getBrightnessBar() const {
    int bars = static_cast<int>(brightness * 10);
    std::string result = "[";
    for (int i = 0; i < 10; i++) {
        result += (i < bars) ? "=" : "-";
    }
    result += "]";
    
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(0) << (brightness * 100) << "%";
    result += " " + oss.str();
    
    return result;
}

std::string SettingsMenu::getVolumeBar() const {
    int bars = static_cast<int>(volume * 10);
    std::string result = "[";
    for (int i = 0; i < 10; i++) {
        result += (i < bars) ? "=" : "-";
    }
    result += "]";
    
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(0) << (volume * 100) << "%";
    result += " " + oss.str();
    
    return result;
}

void SettingsMenu::SetBrightness(float value) {
    brightness = std::clamp(value, 0.1f, 1.0f);
}

void SettingsMenu::SetVolume(float value) {
    volume = std::clamp(value, 0.0f, 1.0f);
}

void SettingsMenu::Render(DisplayHandlerSystem& displayHandler) {
    Vector2 screenSize = displayHandler.GetScreenSize();
    FontManager& fontManager = FontManager::getInstance();
    fontManager.Initialize(displayHandler);
    
    // Background gradient
    for (int i = 0; i < screenSize.y; i++) {
        float alpha = (float)i / screenSize.y;
        Color gradientColor = {
            static_cast<unsigned char>((8 + alpha * 12) * brightness),
            static_cast<unsigned char>((12 + alpha * 18) * brightness),
            static_cast<unsigned char>((28 + alpha * 32) * brightness),
            255
        };
        displayHandler.DrawRectangle({0, (float)i}, {screenSize.x, 1}, gradientColor);
    }
    
    menuPosition.x = screenSize.x / 2;
    menuPosition.y = screenSize.y / 2;

    // Title with pixel font
    Vector2 titlePos = {screenSize.x / 2 - 150, menuPosition.y - 200};
    
    // Title shadow
    TextComponent titleShadow = fontManager.CreateText("PARAMETRES", {titlePos.x + 3, titlePos.y + 3}, 
                                                     FontManager::PIXEL_EXTRUDE, FontManager::HUGE_SIZE, 
                                                     Color{0, 0, 0, static_cast<unsigned char>(180 * brightness)});
    displayHandler.DrawText(titleShadow);
    
    // Main title
    TextComponent titleText = fontManager.CreateText("PARAMETRES", titlePos, 
                                                   FontManager::PIXEL_EXTRUDE, FontManager::HUGE_SIZE, 
                                                   Color{255, 180, 50, static_cast<unsigned char>(255 * brightness)});
    displayHandler.DrawText(titleText);

    // Settings panel
    int panelWidth = 600;
    int panelHeight = 400;
    Vector2 panelPos = {
        screenSize.x / 2 - panelWidth / 2,
        menuPosition.y - 120
    };
    
    displayHandler.DrawRectangle(panelPos, {(float)panelWidth, (float)panelHeight}, Color{20, 30, 50, static_cast<unsigned char>(220 * brightness)});

    // Brightness setting
    Vector2 brightnessPos = {panelPos.x + 50, panelPos.y + 50};
    bool brightnessSelected = (currentField == FIELD_BRIGHTNESS);
    
    if (brightnessSelected) {
        displayHandler.DrawRectangle({brightnessPos.x - 10, brightnessPos.y - 5}, {500, 70}, Color{255, 200, 0, static_cast<unsigned char>(30 * brightness)});
    }
    
    TextComponent brightnessLabel = fontManager.CreateText("> LUMINOSITE", brightnessPos, 
                                                         FontManager::PIXEL_REGULAR, FontManager::XLARGE, 
                                                         brightnessSelected ? Color{255, 255, 100, static_cast<unsigned char>(255 * brightness)} : Color{200, 200, 200, static_cast<unsigned char>(255 * brightness)});
    displayHandler.DrawText(brightnessLabel);
    
    std::string brightnessBar = getBrightnessBar();
    TextComponent brightnessValue = fontManager.CreateText(brightnessBar, {brightnessPos.x + 50, brightnessPos.y + 35}, 
                                                         FontManager::PIXEL_REGULAR, FontManager::LARGE, 
                                                         brightnessSelected ? Color{255, 200, 0, static_cast<unsigned char>(255 * brightness)} : Color{150, 150, 150, static_cast<unsigned char>(255 * brightness)});
    displayHandler.DrawText(brightnessValue);
    
    if (brightnessSelected) {
        TextComponent brightnessHelp = fontManager.CreateText("< Gauche/Droite pour ajuster >", {brightnessPos.x + 50, brightnessPos.y + 55}, 
                                                            FontManager::PIXEL_REGULAR, FontManager::MEDIUM, 
                                                            Color{180, 180, 180, static_cast<unsigned char>(255 * brightness)});
        displayHandler.DrawText(brightnessHelp);
    }

    // Volume setting
    Vector2 volumePos = {panelPos.x + 50, panelPos.y + 150};
    bool volumeSelected = (currentField == FIELD_VOLUME);
    
    if (volumeSelected) {
        displayHandler.DrawRectangle({volumePos.x - 10, volumePos.y - 5}, {500, 70}, Color{0, 255, 150, static_cast<unsigned char>(30 * brightness)});
    }
    
    TextComponent volumeLabel = fontManager.CreateText("> VOLUME", volumePos, 
                                                     FontManager::PIXEL_REGULAR, FontManager::XLARGE, 
                                                     volumeSelected ? Color{255, 255, 100, static_cast<unsigned char>(255 * brightness)} : Color{200, 200, 200, static_cast<unsigned char>(255 * brightness)});
    displayHandler.DrawText(volumeLabel);
    
    std::string volumeBar = getVolumeBar();
    TextComponent volumeValue = fontManager.CreateText(volumeBar, {volumePos.x + 50, volumePos.y + 35}, 
                                                     FontManager::PIXEL_REGULAR, FontManager::LARGE, 
                                                     volumeSelected ? Color{0, 255, 150, static_cast<unsigned char>(255 * brightness)} : Color{150, 150, 150, static_cast<unsigned char>(255 * brightness)});
    displayHandler.DrawText(volumeValue);
    
    if (volumeSelected) {
        TextComponent volumeHelp = fontManager.CreateText("< Gauche/Droite pour ajuster >", {volumePos.x + 50, volumePos.y + 55}, 
                                                        FontManager::PIXEL_REGULAR, FontManager::MEDIUM, 
                                                        Color{180, 180, 180, static_cast<unsigned char>(255 * brightness)});
        displayHandler.DrawText(volumeHelp);
    }

    // Help button
    Vector2 helpPos = {panelPos.x + 50, panelPos.y + 250};
    bool helpSelected = (currentField == FIELD_HELP);
    
    Color helpBgColor = helpSelected ? Color{100, 150, 255, static_cast<unsigned char>(200 * brightness)} : Color{50, 80, 120, static_cast<unsigned char>(150 * brightness)};
    displayHandler.DrawRectangle(helpPos, {200, 50}, helpBgColor);
    
    TextComponent helpText = fontManager.CreateText("? AIDE", {helpPos.x + 70, helpPos.y + 15}, 
                                                  FontManager::PIXEL_REGULAR, FontManager::XLARGE, 
                                                  helpSelected ? Color{255, 255, 255, static_cast<unsigned char>(255 * brightness)} : Color{200, 200, 200, static_cast<unsigned char>(255 * brightness)});
    displayHandler.DrawText(helpText);

    // Back button
    Vector2 backPos = {panelPos.x + 300, panelPos.y + 250};
    bool backSelected = (currentField == FIELD_BACK_SETTINGS);
    
    Color backBgColor = backSelected ? Color{255, 100, 100, static_cast<unsigned char>(200 * brightness)} : Color{120, 50, 50, static_cast<unsigned char>(150 * brightness)};
    displayHandler.DrawRectangle(backPos, {200, 50}, backBgColor);
    
    TextComponent backText = fontManager.CreateText("< RETOUR", {backPos.x + 60, backPos.y + 15}, 
                                                  FontManager::PIXEL_REGULAR, FontManager::XLARGE, 
                                                  backSelected ? Color{255, 255, 255, static_cast<unsigned char>(255 * brightness)} : Color{200, 200, 200, static_cast<unsigned char>(255 * brightness)});
    displayHandler.DrawText(backText);

    // Instructions with pixel font
    displayHandler.DrawRectangle({screenSize.x / 2 - 300, screenSize.y - 60}, {600, 25}, Color{0, 0, 0, static_cast<unsigned char>(150 * brightness)});
    
    TextComponent instructionsText = fontManager.CreateText("Haut/Bas pour naviguer > Gauche/Droite pour ajuster > ENTREE pour selectionner", 
                                                          {screenSize.x / 2 - 290, screenSize.y - 55}, 
                                                          FontManager::PIXEL_REGULAR, FontManager::MEDIUM, 
                                                          Color{180, 180, 180, static_cast<unsigned char>(255 * brightness)});
    displayHandler.DrawText(instructionsText);
}

void SettingsMenu::Cleanup() {
}

std::string SettingsMenu::GetSelectedAction() const {
    return selectedAction;
}

bool SettingsMenu::IsActionSelected() const {
    return actionSelected;
}

void SettingsMenu::ResetSelection() {
    actionSelected = false;
    selectedAction = "";
}