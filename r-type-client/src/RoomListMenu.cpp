/*
** EPITECH PROJECT, 2025
** G-CPP-500-COT-5-1-rtype-20
** File description:
** RoomListMenu implementation
*/

#include "RoomListMenu.hpp"
#include "FontManager.hpp"
#include <algorithm>

RoomListMenu::RoomListMenu() 
    : actionSelected(false), selectedAction(""), menuPosition{400, 300}, fontSize(20), 
      normalColor(WHITE), selectedColor(YELLOW), selectedRoomIndex(0), scrollOffset(0), 
      maxVisibleRooms(6), backgroundColor{20, 30, 50, 220}, borderColor{100, 150, 200, 255},
      joinableColor{100, 255, 100, 255}, fullColor{255, 100, 100, 255} {
}

RoomListMenu::~RoomListMenu() {
    Cleanup();
}

void RoomListMenu::Initialize() {
    actionSelected = false;
    selectedAction = "";
    selectedRoomIndex = 0;
    scrollOffset = 0;
    rooms.clear();
}

void RoomListMenu::Update(InputHandlerSystem& inputHandler) {
    int totalItems = static_cast<int>(rooms.size()) + 1; // rooms + "Créer une salle" en bas

    if (inputHandler.IsKeyPressed(InputKey::KEY_DOWN) || inputHandler.IsKeyPressed(InputKey::KEY_S)) {
        selectedRoomIndex = (selectedRoomIndex + 1) % totalItems;
        updateSelection();
        std::cout << "RoomListMenu: Item sélectionné: " << selectedRoomIndex << std::endl;
    } 
    else if (inputHandler.IsKeyPressed(InputKey::KEY_UP) || inputHandler.IsKeyPressed(InputKey::KEY_W)) {
        selectedRoomIndex = (selectedRoomIndex - 1 + totalItems) % totalItems;
        updateSelection();
        std::cout << "RoomListMenu: Item sélectionné: " << selectedRoomIndex << std::endl;
    } 
    else if (inputHandler.IsKeyPressed(InputKey::KEY_ENTER) || inputHandler.IsKeyPressed(InputKey::KEY_SPACE)) {
        if (selectedRoomIndex < (int)rooms.size()) {
            actionSelected = true;
            selectedAction = "joinRoom";
            std::cout << "RoomListMenu: Rejoindre room ID: " << rooms[selectedRoomIndex].roomId << std::endl;
        } else {
            actionSelected = true;
            selectedAction = "addRoom";
            std::cout << "RoomListMenu: Ajouter une salle" << std::endl;
        }
    }
    else if (inputHandler.IsKeyPressed(InputKey::KEY_ESCAPE)) {
        actionSelected = true;
        selectedAction = "back";
        std::cout << "RoomListMenu: Retour" << std::endl;
    }
}

void RoomListMenu::Render(DisplayHandlerSystem& displayHandler) {
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

    // Title
    Vector2 titlePos = {screenSize.x / 2 - 150, menuPosition.y - 250};
    
    TextComponent titleShadow = fontManager.CreateText("SALLES DISPONIBLES", {titlePos.x + 3, titlePos.y + 3}, 
                                                     FontManager::PIXEL_REGULAR, FontManager::HUGE_SIZE, 
                                                     Color{0, 0, 0, 180});
    displayHandler.DrawText(titleShadow);
    
    TextComponent titleText = fontManager.CreateText("SALLES DISPONIBLES", titlePos, 
                                                   FontManager::PIXEL_REGULAR, FontManager::HUGE_SIZE, 
                                                   Color{100, 200, 255, 255});
    displayHandler.DrawText(titleText);

    // Main panel
    int panelWidth = 700;
    int panelHeight = 400;
    Vector2 panelPos = {
        screenSize.x / 2 - panelWidth / 2,
        menuPosition.y - 150
    };
    
    displayHandler.DrawRectangle(panelPos, {(float)panelWidth, (float)panelHeight}, backgroundColor);

    if (rooms.empty()) {
        // No rooms available message
        TextComponent noRoomsText = fontManager.CreateText("Aucune salle disponible", 
                                                         {panelPos.x + panelWidth/2 - 120, panelPos.y + panelHeight/2 - 20}, 
                                                         FontManager::PIXEL_REGULAR, FontManager::XLARGE, 
                                                         Color{180, 180, 180, 255});
        displayHandler.DrawText(noRoomsText);
        
        TextComponent refreshText = fontManager.CreateText("Appuyez sur ECHAP pour retourner", 
                                                         {panelPos.x + panelWidth/2 - 140, panelPos.y + panelHeight/2 + 20}, 
                                                         FontManager::PIXEL_REGULAR, FontManager::MEDIUM, 
                                                         Color{150, 150, 150, 255});
        displayHandler.DrawText(refreshText);
    } else {
        // Room list header
        Vector2 headerPos = {panelPos.x + 20, panelPos.y + 20};
        
        TextComponent headerName = fontManager.CreateText("NOM DE LA SALLE", headerPos, 
                                                        FontManager::PIXEL_REGULAR, FontManager::MEDIUM, 
                                                        Color{200, 200, 200, 255});
        displayHandler.DrawText(headerName);
        
        TextComponent headerPlayers = fontManager.CreateText("JOUEURS", {headerPos.x + 300, headerPos.y}, 
                                                           FontManager::PIXEL_REGULAR, FontManager::MEDIUM, 
                                                           Color{200, 200, 200, 255});
        displayHandler.DrawText(headerPlayers);
        
        TextComponent headerAction = fontManager.CreateText("ACTION", {headerPos.x + 450, headerPos.y}, 
                                                          FontManager::PIXEL_REGULAR, FontManager::MEDIUM, 
                                                          Color{200, 200, 200, 255});
        displayHandler.DrawText(headerAction);

        // Room list
        int startIndex = scrollOffset;
        int endIndex = std::min(startIndex + maxVisibleRooms, (int)rooms.size());
        
        for (int i = startIndex; i < endIndex; i++) {
            const RoomInfo& room = rooms[i];
            bool isSelected = (i == selectedRoomIndex);
            
            Vector2 roomPos = {panelPos.x + 20, panelPos.y + 60 + (i - startIndex) * 50};
            
            // Room background
            Color roomBg = isSelected ? Color{50, 80, 120, 200} : Color{30, 40, 60, 150};
            displayHandler.DrawRectangle(roomPos, {(float)(panelWidth - 40), 45}, roomBg);
            
            // Room name
            TextComponent roomNameText = fontManager.CreateText(room.roomName, {roomPos.x + 10, roomPos.y + 12}, 
                                                              FontManager::PIXEL_REGULAR, FontManager::MEDIUM, 
                                                              Color{255, 255, 255, 255});
            displayHandler.DrawText(roomNameText);
            
            // Player count
            std::string playerCount = std::to_string(room.currentPlayers) + "/" + std::to_string(room.maxPlayers);
            Color playerColor = room.isJoinable ? joinableColor : fullColor;
            TextComponent playersText = fontManager.CreateText(playerCount, {roomPos.x + 300, roomPos.y + 12}, 
                                                             FontManager::PIXEL_REGULAR, FontManager::MEDIUM, 
                                                             playerColor);
            displayHandler.DrawText(playersText);
            
            // Join button
            std::string buttonText = room.isJoinable ? "REJOINDRE" : "PLEINE";
            Color buttonColor = room.isJoinable ? (isSelected ? Color{255, 255, 255, 255} : Color{100, 255, 100, 255}) 
                                                : Color{120, 120, 120, 255};
            
            if (isSelected && room.isJoinable) {
                displayHandler.DrawRectangle({roomPos.x + 450, roomPos.y + 5}, {120, 35}, Color{100, 255, 100, 100});
            }
            
            TextComponent buttonTextComp = fontManager.CreateText(buttonText, {roomPos.x + 460, roomPos.y + 12}, 
                                                                FontManager::PIXEL_REGULAR, FontManager::MEDIUM, 
                                                                buttonColor);
            displayHandler.DrawText(buttonTextComp);
        }
        
        // Extra action: create room button at bottom (last index)
        {
            bool isSelected = (selectedRoomIndex == (int)rooms.size());
            Vector2 btnPos = {panelPos.x + 20, panelPos.y + panelHeight - 60};
            Color btnBg = isSelected ? Color{50, 120, 80, 220} : Color{30, 60, 40, 180};
            displayHandler.DrawRectangle(btnPos, {(float)(panelWidth - 40), 45}, btnBg);
            TextComponent btnText = fontManager.CreateText("CREER UNE SALLE (ENTREE)", {btnPos.x + 10, btnPos.y + 12}, 
                                                          FontManager::PIXEL_REGULAR, FontManager::MEDIUM, 
                                                          Color{200, 255, 200, 255});
            displayHandler.DrawText(btnText);
        }

        // Scroll indicator
        if (rooms.size() > static_cast<size_t>(maxVisibleRooms)) {
            int scrollBarHeight = panelHeight - 100;
            int scrollBarY = panelPos.y + 60;
            int scrollBarX = panelPos.x + panelWidth - 15;
            
            displayHandler.DrawRectangle({(float)scrollBarX, (float)scrollBarY}, {10, (float)scrollBarHeight}, Color{50, 50, 50, 200});
            
            float scrollRatio = (float)scrollOffset / (rooms.size() - maxVisibleRooms);
            int cursorY = scrollBarY + (int)(scrollRatio * (scrollBarHeight - 30));
            displayHandler.DrawRectangle({(float)scrollBarX, (float)cursorY}, {10, 30}, Color{100, 200, 255, 255});
        }
    }

    // Instructions
    displayHandler.DrawRectangle({screenSize.x / 2 - 250, screenSize.y - 70}, {500, 30}, Color{0, 0, 0, 150});
    
    std::string instructions = rooms.empty() ? "ECHAP pour retourner" : "Haut/Bas pour naviguer > ENTREE pour rejoindre > ECHAP pour retourner";
    TextComponent instructionsText = fontManager.CreateText(instructions, 
                                                          {screenSize.x / 2 - 240, screenSize.y - 65}, 
                                                          FontManager::PIXEL_REGULAR, FontManager::MEDIUM, 
                                                          Color{180, 180, 180, 255});
    displayHandler.DrawText(instructionsText);
}

void RoomListMenu::Cleanup() {
    rooms.clear();
}

std::string RoomListMenu::GetSelectedAction() const {
    return selectedAction;
}

bool RoomListMenu::IsActionSelected() const {
    return actionSelected;
}

void RoomListMenu::ResetSelection() {
    actionSelected = false;
    selectedAction = "";
}

void RoomListMenu::AddRoom(uint32_t roomId, const std::string& roomName, int currentPlayers, int maxPlayers) {
    // Check if room already exists
    for (auto& room : rooms) {
        if (room.roomId == roomId) {
            room.roomName = roomName;
            room.currentPlayers = currentPlayers;
            room.maxPlayers = maxPlayers;
            room.isJoinable = currentPlayers < maxPlayers;
            return;
        }
    }
    
    // Add new room
    rooms.emplace_back(roomId, roomName, currentPlayers, maxPlayers);
    std::cout << "Room added: " << roomName << " (ID: " << roomId << ")" << std::endl;
}

void RoomListMenu::UpdateRoom(uint32_t roomId, int currentPlayers) {
    for (auto& room : rooms) {
        if (room.roomId == roomId) {
            room.currentPlayers = currentPlayers;
            room.isJoinable = currentPlayers < room.maxPlayers;
            return;
        }
    }
}

void RoomListMenu::RemoveRoom(uint32_t roomId) {
    rooms.erase(std::remove_if(rooms.begin(), rooms.end(),
        [roomId](const RoomInfo& room) { return room.roomId == roomId; }), rooms.end());
    
    // Adjust selection if needed
    if (selectedRoomIndex >= (int)rooms.size() && !rooms.empty()) {
        selectedRoomIndex = rooms.size() - 1;
    }
    
    std::cout << "Room removed (ID: " << roomId << ")" << std::endl;
}

void RoomListMenu::ClearRooms() {
    rooms.clear();
    selectedRoomIndex = 0;
    scrollOffset = 0;
}

uint32_t RoomListMenu::GetSelectedRoomId() const {
    if (selectedRoomIndex >= 0 && selectedRoomIndex < (int)rooms.size()) {
        return rooms[selectedRoomIndex].roomId;
    }
    return 0;
}

void RoomListMenu::updateSelection() {
    // Update scroll offset for room items only
    int roomCount = static_cast<int>(rooms.size());
    if (selectedRoomIndex < scrollOffset) {
        scrollOffset = selectedRoomIndex;
    } else if (selectedRoomIndex >= scrollOffset + maxVisibleRooms) {
        scrollOffset = selectedRoomIndex - maxVisibleRooms + 1;
    }
}

bool RoomListMenu::isRoomSelectable(int index) const {
    return index >= 0 && index < (int)rooms.size() && rooms[index].isJoinable;
}