/*
** EPITECH PROJECT, 2025
** G-CPP-500-COT-5-1-rtype-20
** File description:
** GameModeMenu
*/

#ifndef GAMEMODE_MENU_HPP
#define GAMEMODE_MENU_HPP

#include <iostream>
#include <string>
#include <vector>
#include "Display_Handler.hpp"
#include "Input_Handler.hpp"

class GameModeMenu {
    public:
        GameModeMenu();
        ~GameModeMenu();
        void Initialize();
        void Update(InputHandlerSystem& inputHandler);
        void Render(DisplayHandlerSystem& displayHandler);
        void Cleanup();
        std::string GetSelectedAction() const;
        bool IsActionSelected() const;
        void ResetSelection();

    private:
        std::vector<std::string> options;
        int selectedIndex;
        bool actionSelected;
        std::string selectedAction;
        Vector2 menuPosition;
        int fontSize;
        Color normalColor;
        Color selectedColor;
};

#endif // GAMEMODE_MENU_HPP