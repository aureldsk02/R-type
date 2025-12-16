/*
** EPITECH PROJECT, 2025
** G-CPP-500-COT-5-1-rtype-20
** File description:
** RoomListMenu - Interface for listing and joining available rooms
*/

#ifndef ROOMLISTMENU_HPP_
#define ROOMLISTMENU_HPP_

#include <iostream>
#include <string>
#include <vector>
#include "Display_Handler.hpp"
#include "Input_Handler.hpp"

struct RoomInfo {
    uint32_t roomId;
    std::string roomName;
    int currentPlayers;
    int maxPlayers;
    bool isJoinable;
    
    RoomInfo(uint32_t id, const std::string& name, int current, int max)
        : roomId(id), roomName(name), currentPlayers(current), maxPlayers(max), 
          isJoinable(current < max) {}
};

class RoomListMenu {
    public:
        RoomListMenu();
        ~RoomListMenu();
        
        void Initialize();
        void Update(InputHandlerSystem& inputHandler);
        void Render(DisplayHandlerSystem& displayHandler);
        void Cleanup();
        
        std::string GetSelectedAction() const;
        bool IsActionSelected() const;
        void ResetSelection();
        
        // Room management
        void AddRoom(uint32_t roomId, const std::string& roomName, int currentPlayers, int maxPlayers);
        void UpdateRoom(uint32_t roomId, int currentPlayers);
        void RemoveRoom(uint32_t roomId);
        void ClearRooms();
        
        uint32_t GetSelectedRoomId() const;
        bool HasRooms() const { return !rooms.empty(); }

    private:
        bool actionSelected;
        std::string selectedAction;
        Vector2 menuPosition;
        int fontSize;
        Color normalColor;
        Color selectedColor;
        
        std::vector<RoomInfo> rooms;
        int selectedRoomIndex;
        int scrollOffset;
        int maxVisibleRooms;
        
        Color backgroundColor;
        Color borderColor;
        Color joinableColor;
        Color fullColor;
        
        void updateSelection();
        bool isRoomSelectable(int index) const;
};

#endif /* !ROOMLISTMENU_HPP_ */