/*
** EPITECH PROJECT, 2025
** G-CPP-500-COT-5-1-rtype-20 [WSL: Ubuntu-24.04]
** File description:
** Menu
*/

#ifndef MENU_HPP
#define MENU_HPP

#include <iostream>
#include <string>
#include <vector>
#include "Display_Handler.hpp"
#include "Input_Handler.hpp"

class Menu {
    public:
        Menu();
        ~Menu();
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

#endif // MENU_HPP
