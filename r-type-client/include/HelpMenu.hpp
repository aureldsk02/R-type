/*
** EPITECH PROJECT, 2025
** G-CPP-500-COT-5-1-rtype-20
** File description:
** HelpMenu
*/

#ifndef HELP_MENU_HPP
#define HELP_MENU_HPP

#include <iostream>
#include <string>
#include <vector>
#include "Display_Handler.hpp"
#include "Input_Handler.hpp"

class HelpMenu {
    public:
        HelpMenu();
        ~HelpMenu();
        void Initialize();
        void Update(InputHandlerSystem& inputHandler);
        void Render(DisplayHandlerSystem& displayHandler);
        void Cleanup();
        std::string GetSelectedAction() const;
        bool IsActionSelected() const;
        void ResetSelection();

    private:
        bool actionSelected;
        std::string selectedAction;
        Vector2 menuPosition;
        int fontSize;
        Color normalColor;
        Color selectedColor;
        int scrollOffset;
        
        std::vector<std::string> helpContent;
        void initializeHelpContent();
};

#endif // HELP_MENU_HPP