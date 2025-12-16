/*
** EPITECH PROJECT, 2025
** G-CPP-500-COT-5-1-rtype-20
** File description:
** RoomMenu
*/

#ifndef ROOM_MENU_HPP
#define ROOM_MENU_HPP

#include <iostream>
#include <string>
#include <vector>
#include "Display_Handler.hpp"
#include "Input_Handler.hpp"

class RoomMenu {
    public:
        RoomMenu();
        ~RoomMenu();
        void Initialize();
        void Update(InputHandlerSystem& inputHandler);
        void Render(DisplayHandlerSystem& displayHandler);
        void Cleanup();
        std::string GetSelectedAction() const;
        bool IsActionSelected() const;
        void ResetSelection();
        void SetServerInfo(const std::string& ip, int port);

    private:
        std::vector<std::string> options;
        int selectedIndex;
        bool actionSelected;
        std::string selectedAction;
        Vector2 menuPosition;
        int fontSize;
        Color normalColor;
        Color selectedColor;
        std::string serverIP;
        int serverPort;
};

#endif // ROOM_MENU_HPP