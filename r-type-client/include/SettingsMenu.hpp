/*
** EPITECH PROJECT, 2025
** G-CPP-500-COT-5-1-rtype-20
** File description:
** SettingsMenu
*/

#ifndef SETTINGS_MENU_HPP
#define SETTINGS_MENU_HPP

#include <iostream>
#include <string>
#include <vector>
#include "Display_Handler.hpp"
#include "Input_Handler.hpp"

enum SettingsField {
    FIELD_BRIGHTNESS,
    FIELD_VOLUME,
    FIELD_HELP,
    FIELD_BACK_SETTINGS
};

class SettingsMenu {
    public:
        SettingsMenu();
        ~SettingsMenu();
        void Initialize();
        void Update(InputHandlerSystem& inputHandler);
        void Render(DisplayHandlerSystem& displayHandler);
        void Cleanup();
        std::string GetSelectedAction() const;
        bool IsActionSelected() const;
        void ResetSelection();
        
        float GetBrightness() const { return brightness; }
        float GetVolume() const { return volume; }
        
        void SetBrightness(float value);
        void SetVolume(float value);

    private:
        SettingsField currentField;
        bool actionSelected;
        std::string selectedAction;
        Vector2 menuPosition;
        int fontSize;
        Color normalColor;
        Color selectedColor;
        
        float brightness;
        float volume;
        
        void adjustBrightness(bool increase);
        void adjustVolume(bool increase);
        std::string getBrightnessBar() const;
        std::string getVolumeBar() const;
};

#endif // SETTINGS_MENU_HPP