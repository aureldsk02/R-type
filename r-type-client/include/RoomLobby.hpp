/*
** EPITECH PROJECT, 2025
** G-CPP-500-COT-5-1-rtype-20
** File description:
** RoomLobby
*/

#ifndef ROOM_LOBBY_HPP
#define ROOM_LOBBY_HPP

#include <iostream>
#include <string>
#include <vector>
#include "Display_Handler.hpp"
#include "Input_Handler.hpp"

struct LobbyPlayer {
    std::string name;
    int id;
    bool isReady;
    Color nameColor;
    
    LobbyPlayer(const std::string& playerName, int playerId, bool ready = false)
        : name(playerName), id(playerId), isReady(ready), nameColor(WHITE) {}
};

class RoomLobby {
    public:
        RoomLobby();
        ~RoomLobby();
        void Initialize();
        void Update(InputHandlerSystem& inputHandler);
        void Render(DisplayHandlerSystem& displayHandler);
        void Cleanup();
        std::string GetSelectedAction() const;
        bool IsActionSelected() const;
        void ResetSelection();
        
        void SetRoomInfo(const std::string& roomName, int roomId, int maxPlayers = 4);
        void AddPlayer(const std::string& playerName, int playerId);
        void RemovePlayer(int playerId);
        void SetPlayerReady(int playerId, bool ready);
        void SetLocalPlayerReady(bool ready) { isLocalPlayerReady = ready; }
        bool IsLocalPlayerReady() const { return isLocalPlayerReady; }
        void SetIsRoomOwner(bool owner) { isRoomOwner = owner; }
        bool IsRoomOwner() const { return isRoomOwner; }
        
        int GetPlayerCount() const { return players.size(); }
        int GetReadyPlayerCount() const;
        bool AllPlayersReady() const;

    private:
        bool actionSelected;
        std::string selectedAction;
        Vector2 menuPosition;
        int fontSize;
        Color normalColor;
        Color selectedColor;
        
        std::string roomName;
        int roomId;
        int maxPlayers;
        std::vector<LobbyPlayer> players;
        bool isLocalPlayerReady;
        bool isRoomOwner;
        
        int selectedOption;
        int maxOptions;
        
        Color backgroundColor;
        Color borderColor;
        Color readyColor;
        Color notReadyColor;
        Color ownerColor;
        
        void updateMaxOptions();
        std::string getOptionText(int option) const;
        bool isOptionAvailable(int option) const;
};

#endif // ROOM_LOBBY_HPP