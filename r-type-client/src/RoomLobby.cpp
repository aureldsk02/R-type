/*
** EPITECH PROJECT, 2025
** G-CPP-500-COT-5-1-rtype-20
** File description:
** RoomLobby
*/

#include "RoomLobby.hpp"
#include "FontManager.hpp"

RoomLobby::RoomLobby() 
    : actionSelected(false), selectedAction(""), menuPosition{400, 300}, fontSize(24), 
      normalColor(WHITE), selectedColor(YELLOW), roomName(""), roomId(0), maxPlayers(4),
      isLocalPlayerReady(false), isRoomOwner(false), selectedOption(0), maxOptions(3),
      backgroundColor{20, 30, 50, 220}, borderColor{100, 150, 200, 255},
      readyColor{100, 255, 100, 255}, notReadyColor{255, 100, 100, 255}, ownerColor{255, 200, 100, 255} {
}

RoomLobby::~RoomLobby() {
    Cleanup();
}

void RoomLobby::Initialize() {
    actionSelected = false;
    selectedAction = "";
    selectedOption = 0;
    isLocalPlayerReady = false;
    players.clear();
    updateMaxOptions();
}



void RoomLobby::Update(InputHandlerSystem& inputHandler) {
    updateMaxOptions();
    if (maxOptions == 0) {
        return; // No selectable options for non-owners
    }

    if (inputHandler.IsKeyPressed(InputKey::KEY_DOWN) || inputHandler.IsKeyPressed(InputKey::KEY_S)) {
        do {
            selectedOption = (selectedOption + 1) % maxOptions;
        } while (!isOptionAvailable(selectedOption));
        std::cout << "RoomLobby: Option sélectionnée: " << selectedOption << std::endl;
    } 
    else if (inputHandler.IsKeyPressed(InputKey::KEY_UP) || inputHandler.IsKeyPressed(InputKey::KEY_W)) {
        do {
            selectedOption = (selectedOption - 1 + maxOptions) % maxOptions;
        } while (!isOptionAvailable(selectedOption));
        std::cout << "RoomLobby: Option sélectionnée: " << selectedOption << std::endl;
    } 
    else if (inputHandler.IsKeyPressed(InputKey::KEY_ENTER) || inputHandler.IsKeyPressed(InputKey::KEY_SPACE)) {
        actionSelected = true;
        // Only option is Start Game for room owner
        if (selectedOption == 0 && isRoomOwner) {
            selectedAction = "startGame";
            std::cout << "RoomLobby: Starting game..." << std::endl;
        }
    }
}

void RoomLobby::Render(DisplayHandlerSystem& displayHandler) {
    Vector2 screenSize = displayHandler.GetScreenSize();
    FontManager& fontManager = FontManager::getInstance();
    fontManager.Initialize(displayHandler);
    
    menuPosition.x = screenSize.x / 2;
    menuPosition.y = screenSize.y / 2;

    // === BACKGROUND GRADIENT ===
    for (int i = 0; i < screenSize.y; i += 4) {
        float alpha = (float)i / screenSize.y;
        Color gradientColor = {
            static_cast<unsigned char>(15 + alpha * 25),
            static_cast<unsigned char>(20 + alpha * 35),
            static_cast<unsigned char>(45 + alpha * 55),
            255
        };
        displayHandler.DrawRectangle({0, (float)i}, {screenSize.x, 4}, gradientColor);
    }

    // === MAIN TITLE WITH SHADOW ===
    TextComponent titleShadow = fontManager.CreateTitleText("LOBBY", {menuPosition.x - 85, menuPosition.y - 245}, Color{0, 0, 0, 150});
    displayHandler.DrawText(titleShadow);
    
    TextComponent titleText = fontManager.CreateTitleText("LOBBY", {menuPosition.x - 90, menuPosition.y - 250}, Color{255, 255, 255, 255});
    displayHandler.DrawText(titleText);
    
    // === ROOM INFO PANEL ===
    Vector2 roomPanelPos = {menuPosition.x - 300, menuPosition.y - 180};
    Vector2 roomPanelSize = {600, 60};
    
    displayHandler.DrawRectangle({roomPanelPos.x + 5, roomPanelPos.y}, {roomPanelSize.x - 10, roomPanelSize.y}, Color{40, 50, 70, 200});
    displayHandler.DrawRectangle({roomPanelPos.x, roomPanelPos.y + 5}, {roomPanelSize.x, roomPanelSize.y - 10}, Color{40, 50, 70, 200});
    
    TextComponent roomNameText = fontManager.CreateText(roomName + " - ID: " + std::to_string(roomId), 
                                                      {roomPanelPos.x + 30, roomPanelPos.y + 15}, 
                                                      FontManager::PIXEL_REGULAR, FontManager::LARGE, 
                                                      Color{100, 200, 255, 255});
    displayHandler.DrawText(roomNameText);
    
    TextComponent playerCountText = fontManager.CreateText(std::to_string(players.size()) + "/" + std::to_string(maxPlayers) + " JOUEURS", 
                                                         {roomPanelPos.x + 450, roomPanelPos.y + 20}, 
                                                         FontManager::PIXEL_REGULAR, FontManager::MEDIUM, 
                                                         Color{150, 255, 150, 255});
    displayHandler.DrawText(playerCountText);

    // === PLAYERS GRID ===
    Vector2 playersAreaPos = {menuPosition.x - 350, menuPosition.y - 80};
    Vector2 playersAreaSize = {700, 240};
    
    displayHandler.DrawRectangle(playersAreaPos, playersAreaSize, Color{25, 35, 55, 180});
    
    int cardsPerRow = 2;
    float cardWidth = 300;
    float cardHeight = 100;
    float cardSpacing = 50;
    
    for (size_t i = 0; i < (size_t)maxPlayers; i++) {
        int row = i / cardsPerRow;
        int col = i % cardsPerRow;
        
        Vector2 cardPos = {
            playersAreaPos.x + 50 + col * (cardWidth + cardSpacing),
            playersAreaPos.y + 20 + row * (cardHeight + 20)
        };
        
        if (i < players.size()) {
            const LobbyPlayer& player = players[i];
            
            Color cardBg = player.isReady ? Color{50, 120, 50, 200} : Color{120, 50, 50, 200};
            if (isRoomOwner && i == 0) cardBg = Color{120, 80, 20, 200};
            
            displayHandler.DrawRectangle(cardPos, {cardWidth, cardHeight}, cardBg);
            
            Color borderColor = player.isReady ? Color{100, 255, 100, 255} : Color{255, 100, 100, 255};
            if (isRoomOwner && i == 0) borderColor = Color{255, 200, 100, 255};
            
            displayHandler.DrawRectangle({cardPos.x - 2, cardPos.y - 2}, {cardWidth + 4, 2}, borderColor);
            displayHandler.DrawRectangle({cardPos.x - 2, cardPos.y + cardHeight}, {cardWidth + 4, 2}, borderColor);
            displayHandler.DrawRectangle({cardPos.x - 2, cardPos.y}, {2, cardHeight}, borderColor);
            displayHandler.DrawRectangle({cardPos.x + cardWidth, cardPos.y}, {2, cardHeight}, borderColor);
            
            Vector2 avatarPos = {cardPos.x + 30, cardPos.y + 25};
            Color avatarColor = player.isReady ? Color{100, 255, 100, 255} : Color{255, 150, 150, 255};
            if (isRoomOwner && i == 0) avatarColor = Color{255, 200, 100, 255};
            
            displayHandler.DrawRectangle({avatarPos.x - 20, avatarPos.y - 20}, {40, 40}, avatarColor);
            
            TextComponent playerNameText = fontManager.CreateText(player.name, {cardPos.x + 80, cardPos.y + 15}, 
                                                                FontManager::PIXEL_REGULAR, FontManager::MEDIUM, 
                                                                Color{255, 255, 255, 255});
            displayHandler.DrawText(playerNameText);
            
            std::string statusText = player.isReady ? "PRET" : "EN ATTENTE";
            if (isRoomOwner && i == 0) statusText = "HOST " + statusText;
            
            TextComponent playerStatusText = fontManager.CreateText(statusText, {cardPos.x + 80, cardPos.y + 45}, 
                                                                  FontManager::PIXEL_REGULAR, FontManager::SMALL, 
                                                                  borderColor);
            displayHandler.DrawText(playerStatusText);
            
        } else {
            displayHandler.DrawRectangle(cardPos, {cardWidth, cardHeight}, Color{40, 40, 40, 100});
            
            for (int dash = 0; dash < cardWidth; dash += 20) {
                displayHandler.DrawRectangle({cardPos.x + dash, cardPos.y - 1}, {10, 2}, Color{100, 100, 100, 150});
                displayHandler.DrawRectangle({cardPos.x + dash, cardPos.y + cardHeight - 1}, {10, 2}, Color{100, 100, 100, 150});
            }
            
            TextComponent waitingText = fontManager.CreateText("En attente...", {cardPos.x + cardWidth/2 - 60, cardPos.y + cardHeight/2 - 10}, 
                                                              FontManager::PIXEL_REGULAR, FontManager::MEDIUM, 
                                                              Color{120, 120, 120, 255});
            displayHandler.DrawText(waitingText);
        }
    }

    // === ACTION BUTTONS ===
    Vector2 buttonsAreaPos = {menuPosition.x - 300, menuPosition.y + 200};
    
    for (int i = 0; i < maxOptions; i++) {
        if (!isOptionAvailable(i)) continue;
        
        bool isSelected = (i == selectedOption);
        std::string optionText = getOptionText(i);
        
        Vector2 buttonPos = {buttonsAreaPos.x + i * 200, buttonsAreaPos.y};
        Vector2 buttonSize = {180, 50};
        
        Color buttonBg;
        Color buttonBorder;
        
        // Only Start button for owner at index 0
        bool canStart = (players.size() >= 1);
        buttonBg = canStart ? Color{20, 100, 20, 200} : Color{60, 60, 60, 200};
        buttonBorder = canStart ? Color{50, 255, 50, 255} : Color{120, 120, 120, 255};
        
        if (isSelected) {
            displayHandler.DrawRectangle({buttonPos.x - 5, buttonPos.y - 5}, {buttonSize.x + 10, buttonSize.y + 10}, Color{buttonBorder.r, buttonBorder.g, buttonBorder.b, 100});
        }
        
        displayHandler.DrawRectangle(buttonPos, buttonSize, buttonBg);
        
        displayHandler.DrawRectangle({buttonPos.x - 2, buttonPos.y - 2}, {buttonSize.x + 4, 2}, buttonBorder);
        displayHandler.DrawRectangle({buttonPos.x - 2, buttonPos.y + buttonSize.y}, {buttonSize.x + 4, 2}, buttonBorder);
        displayHandler.DrawRectangle({buttonPos.x - 2, buttonPos.y}, {2, buttonSize.y}, buttonBorder);
        displayHandler.DrawRectangle({buttonPos.x + buttonSize.x, buttonPos.y}, {2, buttonSize.y}, buttonBorder);
        
        TextComponent buttonText = fontManager.CreateText(optionText, {buttonPos.x + 20, buttonPos.y + 15}, 
                                                         FontManager::PIXEL_REGULAR, FontManager::SMALL, 
                                                         isSelected ? Color{255, 255, 255, 255} : Color{220, 220, 220, 255});
        displayHandler.DrawText(buttonText);
    }

    Vector2 statusBarPos = {menuPosition.x - 200, menuPosition.y + 280};
    int readyCount = GetReadyPlayerCount();
    
    TextComponent statusText = fontManager.CreateText("Joueurs prets: " + std::to_string(readyCount) + "/" + std::to_string(players.size()), 
                                                    statusBarPos, FontManager::PIXEL_REGULAR, FontManager::MEDIUM, 
                                                    AllPlayersReady() ? Color{100, 255, 100, 255} : Color{255, 200, 100, 255});
    displayHandler.DrawText(statusText);

    TextComponent instructionsText = fontManager.CreateText("Haut/Bas Naviguer • ENTREE Selectionner", 
                                                          {menuPosition.x - 150, menuPosition.y + 320}, 
                                                          FontManager::PIXEL_REGULAR, FontManager::SMALL, 
                                                          Color{150, 150, 150, 255});
    displayHandler.DrawText(instructionsText);
}

void RoomLobby::Cleanup() {
    players.clear();
}

std::string RoomLobby::GetSelectedAction() const {
    return selectedAction;
}

bool RoomLobby::IsActionSelected() const {
    return actionSelected;
}

void RoomLobby::ResetSelection() {
    actionSelected = false;
    selectedAction = "";
}

void RoomLobby::SetRoomInfo(const std::string& name, int id, int maxPlayers) {
    roomName = name;
    roomId = id;
    this->maxPlayers = maxPlayers;
}

void RoomLobby::AddPlayer(const std::string& playerName, int playerId) {
    for (auto& player : players) {
        if (player.id == playerId) {
            player.name = playerName;
            return;
        }
    }
    
    players.emplace_back(playerName, playerId, false);
    std::cout << "Player added to lobby: " << playerName << " (ID: " << playerId << ")" << std::endl;
}

void RoomLobby::RemovePlayer(int playerId) {
    players.erase(std::remove_if(players.begin(), players.end(),
        [playerId](const LobbyPlayer& player) { return player.id == playerId; }), players.end());
    std::cout << "Player removed from lobby (ID: " << playerId << ")" << std::endl;
}

void RoomLobby::SetPlayerReady(int playerId, bool ready) {
    for (auto& player : players) {
        if (player.id == playerId) {
            player.isReady = ready;
            std::cout << "Player " << player.name << " is now " << (ready ? "ready" : "not ready") << std::endl;
            return;
        }
    }
}

int RoomLobby::GetReadyPlayerCount() const {
    int count = 0;
    for (const auto& player : players) {
        if (player.isReady) count++;
    }
    return count;
}

bool RoomLobby::AllPlayersReady() const {
    if (players.empty()) return false;
    
    for (const auto& player : players) {
        if (!player.isReady) return false;
    }
    return true;
}

void RoomLobby::updateMaxOptions() {
    maxOptions = isRoomOwner ? 1 : 0;
}

std::string RoomLobby::getOptionText(int option) const {
    switch (option) {
        case 0:
            if (isRoomOwner) return "DEMARRER";
            return "";
        default:
            return "";
    }
}

bool RoomLobby::isOptionAvailable(int option) const {
    if (option == 0 && isRoomOwner) {
        return players.size() >= 1;
    }
    return false;
}