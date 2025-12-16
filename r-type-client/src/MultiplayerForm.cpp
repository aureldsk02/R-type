/*
** EPITECH PROJECT, 2025
** G-CPP-500-COT-5-1-rtype-20
** File description:
** MultiplayerForm
*/

#include "MultiplayerForm.hpp"
#include "FontManager.hpp"
#include <regex>
#include <sstream>

MultiplayerForm::MultiplayerForm() 
    : currentField(FIELD_PLAYER_NAME), actionSelected(false), selectedAction(""), 
      playerName("Player1"), serverIP("127.0.0.1"), serverPortStr("12345"), serverPort(12345),
      formPosition{400, 300}, fontSize(24), normalColor(WHITE), 
      selectedColor(YELLOW), inputColor(LIGHTGRAY), isTyping(false),
      connectionStatus(STATUS_READY), statusMessage("") {
}

MultiplayerForm::~MultiplayerForm() {
}

void MultiplayerForm::Initialize() {
    currentField = FIELD_PLAYER_NAME;
    actionSelected = false;
    selectedAction = "";
    playerName = "Player1";
    serverIP = "127.0.0.1";
    serverPortStr = "12345";
    serverPort = 12345;
    isTyping = false;
    connectionStatus = STATUS_READY;
    statusMessage = "";
}

void MultiplayerForm::Update(InputHandlerSystem& inputHandler) {
    if (isTyping) {
        handleTextInput(inputHandler);
        
        if (inputHandler.IsKeyPressed(InputKey::KEY_ENTER)) {
            isTyping = false;
            if (currentField == FIELD_PORT) {
                try {
                    serverPort = std::stoi(serverPortStr);
                    if (serverPort <= 0 || serverPort > 65535) {
                        serverPort = 12345;
                        serverPortStr = "12345";
                    }
                } catch (const std::exception&) {
                    serverPort = 12345;
                    serverPortStr = "12345";
                }
            }
        }
        
        if (inputHandler.IsKeyPressed(InputKey::KEY_ESCAPE)) {
            isTyping = false;
        }
        
        return;
    }
    
    if (inputHandler.IsKeyPressed(InputKey::KEY_DOWN) || inputHandler.IsKeyPressed(InputKey::KEY_S)) {
        currentField = static_cast<FormField>((currentField + 1) % 5);
        std::cout << "MultiplayerForm: Champ sélectionné: " << currentField << std::endl;
    } 
    else if (inputHandler.IsKeyPressed(InputKey::KEY_UP) || inputHandler.IsKeyPressed(InputKey::KEY_W)) {
        currentField = static_cast<FormField>((currentField - 1 + 5) % 5);
        std::cout << "MultiplayerForm: Champ sélectionné: " << currentField << std::endl;
    } 
    else if (inputHandler.IsKeyPressed(InputKey::KEY_ENTER) || inputHandler.IsKeyPressed(InputKey::KEY_SPACE)) {
        if (currentField == FIELD_PLAYER_NAME || currentField == FIELD_IP || currentField == FIELD_PORT) {
            isTyping = true;
            std::cout << "MultiplayerForm: Mode saisie activé pour le champ: " << currentField << std::endl;
        } else if (currentField == FIELD_CONNECT) {
            if (!playerName.empty() && isValidIP(serverIP) && isValidPort(serverPortStr)) {
                actionSelected = true;
                selectedAction = "connect";
                std::cout << "MultiplayerForm: Connexion de " << playerName << " à " << serverIP << ":" << serverPort << std::endl;
            } else {
                std::cout << "MultiplayerForm: Nom de joueur, IP ou port invalide!" << std::endl;
            }
        } else if (currentField == FIELD_BACK) {
            actionSelected = true;
            selectedAction = "back";
            std::cout << "MultiplayerForm: Retour au menu précédent" << std::endl;
        }
    }
}

void MultiplayerForm::handleTextInput(InputHandlerSystem&) {
    std::string* targetField;
    if (currentField == FIELD_PLAYER_NAME) {
        targetField = &playerName;
    } else if (currentField == FIELD_IP) {
        targetField = &serverIP;
    } else {
        targetField = &serverPortStr;
    }
    
    int key = GetKeyPressed();
    if (key == KEY_BACKSPACE) {
        if (!targetField->empty()) {
            targetField->pop_back();
        }
    }
    
    while ((key = GetCharPressed()) > 0) {
        char c = static_cast<char>(key);
        
        if (currentField == FIELD_PLAYER_NAME) {
            if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || 
                (c >= '0' && c <= '9') || c == '_' || c == '-') {
                if (playerName.length() < 16) {
                    *targetField += c;
                }
            }
        } else if (currentField == FIELD_IP) {
            if ((c >= '0' && c <= '9') || c == '.') {
                if (serverIP.length() < 15) { 
                    *targetField += c;
                }
            }
        } else if (currentField == FIELD_PORT) {
            if (c >= '0' && c <= '9') {
                if (serverPortStr.length() < 5) {
                    *targetField += c;
                }
            }
        }
    }
}

bool MultiplayerForm::isValidIP(const std::string& ip) {
    std::regex ipPattern(R"(^((25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.){3}(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)$)");
    return std::regex_match(ip, ipPattern);
}

bool MultiplayerForm::isValidPort(const std::string& port) {
    if (port.empty()) return false;
    try {
        int p = std::stoi(port);
        return p > 0 && p <= 65535;
    } catch (const std::exception&) {
        return false;
    }
}
void MultiplayerForm::Render(DisplayHandlerSystem& displayHandler) {
    Vector2 screenSize = displayHandler.GetScreenSize();
    FontManager& fontManager = FontManager::getInstance();
    fontManager.Initialize(displayHandler);
    
    // Background gradient
    for (int i = 0; i < screenSize.y; i++) {
        float alpha = (float)i / screenSize.y;
        Color gradientColor = {
            static_cast<unsigned char>(5 + alpha * 10),
            static_cast<unsigned char>(15 + alpha * 15),
            static_cast<unsigned char>(30 + alpha * 25),
            255
        };
        displayHandler.DrawRectangle({0, (float)i}, {screenSize.x, 1}, gradientColor);
    }
    
    formPosition.x = screenSize.x / 2;
    formPosition.y = screenSize.y / 2;

    // Title with pixel font
    Vector2 titlePos = {screenSize.x / 2 - 200, formPosition.y - 220};
    
    // Title shadow
    TextComponent titleShadow = fontManager.CreateText("CONNEXION RESEAU", {titlePos.x + 3, titlePos.y + 3}, 
                                                     FontManager::PIXEL_EXTRUDE, FontManager::HUGE_SIZE, 
                                                     Color{0, 0, 0, 180});
    displayHandler.DrawText(titleShadow);
    
    // Main title
    TextComponent titleText = fontManager.CreateText("CONNEXION RESEAU", titlePos, 
                                                   FontManager::PIXEL_EXTRUDE, FontManager::HUGE_SIZE, 
                                                   Color{0, 255, 255, 255});
    displayHandler.DrawText(titleText);

    // Form box
    int formBoxWidth = 600;
    int formBoxHeight = 420;
    Vector2 formBoxPos = {
        screenSize.x / 2 - formBoxWidth / 2,
        formPosition.y - 120
    };
    
    displayHandler.DrawRectangle({formBoxPos.x + 8, formBoxPos.y + 8}, {(float)formBoxWidth, (float)formBoxHeight}, Color{0, 0, 0, 100});
    displayHandler.DrawRectangle(formBoxPos, {(float)formBoxWidth, (float)formBoxHeight}, Color{15, 25, 45, 240});

    // Player name field
    Vector2 namePos = {formBoxPos.x + 40, formBoxPos.y + 40};
    bool nameSelected = (currentField == FIELD_PLAYER_NAME);
    bool nameActive = (currentField == FIELD_PLAYER_NAME && isTyping);
    
    TextComponent nameLabel = fontManager.CreateText("> NOM DU JOUEUR", namePos, 
                                                   FontManager::PIXEL_REGULAR, FontManager::LARGE, 
                                                   nameSelected ? Color{0, 255, 255, 255} : Color{180, 180, 180, 255});
    displayHandler.DrawText(nameLabel);
    
    Vector2 nameInputPos = {formBoxPos.x + 40, namePos.y + 40};
    
    Color nameBgColor = nameActive ? Color{40, 60, 80, 255} : (nameSelected ? Color{25, 40, 60, 255} : Color{20, 30, 45, 255});
    displayHandler.DrawRectangle(nameInputPos, {400, 40}, nameBgColor);
    
    std::string nameDisplayText = playerName;
    if (nameActive) nameDisplayText += "|";
    
    TextComponent nameInput = fontManager.CreateText(nameDisplayText, {nameInputPos.x + 15, nameInputPos.y + 10}, 
                                                   FontManager::PIXEL_REGULAR, FontManager::LARGE, 
                                                   Color{255, 255, 255, 255});
    displayHandler.DrawText(nameInput);

    // IP address field
    Vector2 ipLabelPos = {formBoxPos.x + 40, nameInputPos.y + 70};
    bool ipSelected = (currentField == FIELD_IP);
    bool ipActive = (currentField == FIELD_IP && isTyping);
    
    TextComponent ipLabel = fontManager.CreateText("> ADRESSE IP DU SERVEUR", ipLabelPos, 
                                                 FontManager::PIXEL_REGULAR, FontManager::LARGE, 
                                                 ipSelected ? Color{0, 255, 255, 255} : Color{180, 180, 180, 255});
    displayHandler.DrawText(ipLabel);
    
    Vector2 ipInputPos = {formBoxPos.x + 40, ipLabelPos.y + 40};
    
    Color ipBgColor = ipActive ? Color{40, 60, 80, 255} : (ipSelected ? Color{25, 40, 60, 255} : Color{20, 30, 45, 255});
    displayHandler.DrawRectangle(ipInputPos, {400, 40}, ipBgColor);
    
    std::string ipDisplayText = serverIP;
    if (ipActive) ipDisplayText += "|";
    
    TextComponent ipInput = fontManager.CreateText(ipDisplayText, {ipInputPos.x + 15, ipInputPos.y + 10}, 
                                                 FontManager::PIXEL_REGULAR, FontManager::LARGE, 
                                                 Color{255, 255, 255, 255});
    displayHandler.DrawText(ipInput);

    // Port field
    Vector2 portLabelPos = {formBoxPos.x + 40, ipInputPos.y + 70};
    bool portSelected = (currentField == FIELD_PORT);
    bool portActive = (currentField == FIELD_PORT && isTyping);
    
    TextComponent portLabel = fontManager.CreateText("> PORT DU SERVEUR", portLabelPos, 
                                                   FontManager::PIXEL_REGULAR, FontManager::LARGE, 
                                                   portSelected ? Color{0, 255, 255, 255} : Color{180, 180, 180, 255});
    displayHandler.DrawText(portLabel);
    
    Vector2 portInputPos = {formBoxPos.x + 40, portLabelPos.y + 40};
    
    Color portBgColor = portActive ? Color{40, 60, 80, 255} : (portSelected ? Color{25, 40, 60, 255} : Color{20, 30, 45, 255});
    displayHandler.DrawRectangle(portInputPos, {200, 40}, portBgColor);
    
    std::string portDisplayText = serverPortStr;
    if (portActive) portDisplayText += "|";
    
    TextComponent portInput = fontManager.CreateText(portDisplayText, {portInputPos.x + 15, portInputPos.y + 10}, 
                                                   FontManager::PIXEL_REGULAR, FontManager::LARGE, 
                                                   Color{255, 255, 255, 255});
    displayHandler.DrawText(portInput);

    // Validation indicators
    if (playerName.empty()) {
        TextComponent nameError = fontManager.CreateText("X Nom requis", {nameInputPos.x + 420, nameInputPos.y + 10}, 
                                                       FontManager::PIXEL_REGULAR, FontManager::MEDIUM, 
                                                       Color{255, 100, 100, 255});
        displayHandler.DrawText(nameError);
    } else if (playerName.length() >= 3) {
        TextComponent nameOk = fontManager.CreateText("OK", {nameInputPos.x + 420, nameInputPos.y + 10}, 
                                                    FontManager::PIXEL_REGULAR, FontManager::MEDIUM, 
                                                    Color{100, 255, 100, 255});
        displayHandler.DrawText(nameOk);
    }

    // Connect button
    Vector2 connectBtnPos = {formBoxPos.x + 80, portInputPos.y + 80};
    bool connectSelected = (currentField == FIELD_CONNECT);
    
    Color connectBgColor = connectSelected ? Color{0, 150, 100, 200} : Color{0, 100, 80, 150};
    displayHandler.DrawRectangle(connectBtnPos, {180, 50}, connectBgColor);
    
    TextComponent connectText = fontManager.CreateText("> SE CONNECTER", {connectBtnPos.x + 20, connectBtnPos.y + 15}, 
                                                     FontManager::PIXEL_REGULAR, FontManager::LARGE, 
                                                     connectSelected ? Color{255, 255, 255, 255} : Color{200, 200, 200, 255});
    displayHandler.DrawText(connectText);

    // Back button
    Vector2 backBtnPos = {formBoxPos.x + 320, portInputPos.y + 80};
    bool backSelected = (currentField == FIELD_BACK);
    
    Color backBgColor = backSelected ? Color{150, 50, 0, 200} : Color{100, 40, 0, 150};
    displayHandler.DrawRectangle(backBtnPos, {140, 50}, backBgColor);
    
    TextComponent backText = fontManager.CreateText("< RETOUR", {backBtnPos.x + 30, backBtnPos.y + 15}, 
                                                  FontManager::PIXEL_REGULAR, FontManager::LARGE, 
                                                  backSelected ? Color{255, 255, 255, 255} : Color{200, 200, 200, 255});
    displayHandler.DrawText(backText);

    // IP validation
    if (!isValidIP(serverIP) && !serverIP.empty()) {
        TextComponent ipError = fontManager.CreateText("X Format IP invalide", {ipInputPos.x + 420, ipInputPos.y + 10}, 
                                                     FontManager::PIXEL_REGULAR, FontManager::MEDIUM, 
                                                     Color{255, 100, 100, 255});
        displayHandler.DrawText(ipError);
    } else if (!serverIP.empty() && isValidIP(serverIP)) {
        TextComponent ipOk = fontManager.CreateText("OK", {ipInputPos.x + 420, ipInputPos.y + 10}, 
                                                  FontManager::PIXEL_REGULAR, FontManager::MEDIUM, 
                                                  Color{100, 255, 100, 255});
        displayHandler.DrawText(ipOk);
    }
    
    // Port validation
    if (!isValidPort(serverPortStr) && !serverPortStr.empty()) {
        TextComponent portError = fontManager.CreateText("X Port invalide", {portInputPos.x + 220, portInputPos.y + 10}, 
                                                       FontManager::PIXEL_REGULAR, FontManager::MEDIUM, 
                                                       Color{255, 100, 100, 255});
        displayHandler.DrawText(portError);
    } else if (!serverPortStr.empty() && isValidPort(serverPortStr)) {
        TextComponent portOk = fontManager.CreateText("OK", {portInputPos.x + 220, portInputPos.y + 10}, 
                                                    FontManager::PIXEL_REGULAR, FontManager::MEDIUM, 
                                                    Color{100, 255, 100, 255});
        displayHandler.DrawText(portOk);
    }

    // Instructions
    std::string instructions;
    if (isTyping) {
        instructions = "> Saisissez les donnees > ENTREE pour valider > ECHAP pour annuler";
    } else {
        instructions = "> TAB/Fleches pour naviguer > ENTREE pour selectionner/editer";
    }
    
    displayHandler.DrawRectangle({screenSize.x / 2 - 350, screenSize.y - 70}, {700, 30}, Color{0, 0, 0, 180});
    
    TextComponent instructionsText = fontManager.CreateText(instructions, {screenSize.x / 2 - 340, screenSize.y - 65}, 
                                                          FontManager::PIXEL_REGULAR, FontManager::MEDIUM, 
                                                          Color{180, 180, 180, 255});
    displayHandler.DrawText(instructionsText);
             
    std::string networkStatus;
    Color statusColor;
    
    switch (connectionStatus) {
        case STATUS_READY:
            networkStatus = "> Pret a se connecter";
            statusColor = Color{100, 255, 100, 255};
            break;
        case STATUS_CONNECTING:
            networkStatus = "> Connexion en cours...";
            statusColor = Color{255, 255, 100, 255};
            break;
        case STATUS_CONNECTED:
            networkStatus = "> Connecte avec succes!";
            statusColor = Color{100, 255, 100, 255};
            break;
        case STATUS_ERROR:
            networkStatus = "> Erreur: " + statusMessage;
            statusColor = Color{255, 100, 100, 255};
            break;
    }
    
    int statusWidth = MeasureText(networkStatus.c_str(), 14);
    
    DrawRectangle(screenSize.x / 2 - statusWidth/2 - 10, formBoxPos.y + formBoxHeight + 15, statusWidth + 20, 20, Color{0, 0, 0, 150});
    
    DrawText(networkStatus.c_str(), 
             screenSize.x / 2 - statusWidth / 2, 
             formBoxPos.y + formBoxHeight + 20, 
             14, 
             statusColor);
}

void MultiplayerForm::Cleanup() {
}

std::string MultiplayerForm::GetSelectedAction() const {
    return selectedAction;
}

bool MultiplayerForm::IsActionSelected() const {
    return actionSelected;
}

void MultiplayerForm::ResetSelection() {
    actionSelected = false;
    selectedAction = "";
}

void MultiplayerForm::SetConnectionStatus(ConnectionStatus status, const std::string& message) {
    connectionStatus = status;
    statusMessage = message;
}