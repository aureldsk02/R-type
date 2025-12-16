/*
** EPITECH PROJECT, 2025
** G-CPP-500-COT-5-1-rtype-20
** File description:
** MultiplayerForm
*/

#ifndef MULTIPLAYER_FORM_HPP
#define MULTIPLAYER_FORM_HPP

#include <iostream>
#include <string>
#include <vector>
#include "Display_Handler.hpp"
#include "Input_Handler.hpp"

enum FormField {
    FIELD_PLAYER_NAME,
    FIELD_IP,
    FIELD_PORT,
    FIELD_CONNECT,
    FIELD_BACK
};

enum ConnectionStatus {
    STATUS_READY,
    STATUS_CONNECTING,
    STATUS_CONNECTED,
    STATUS_ERROR
};

class MultiplayerForm {
    public:
        MultiplayerForm();
        ~MultiplayerForm();
        void Initialize();
        void Update(InputHandlerSystem& inputHandler);
        void Render(DisplayHandlerSystem& displayHandler);
        void Cleanup();
        std::string GetSelectedAction() const;
        bool IsActionSelected() const;
        void ResetSelection();
        std::string GetPlayerName() const { return playerName; }
        std::string GetServerIP() const { return serverIP; }
        int GetServerPort() const { return serverPort; }
        void SetConnectionStatus(ConnectionStatus status, const std::string& message = "");
        ConnectionStatus GetConnectionStatus() const { return connectionStatus; }

    private:
        FormField currentField;
        bool actionSelected;
        std::string selectedAction;
        std::string playerName;
        std::string serverIP;
        std::string serverPortStr;
        int serverPort;
        Vector2 formPosition;
        int fontSize;
        Color normalColor;
        Color selectedColor;
        Color inputColor;
        bool isTyping;
        ConnectionStatus connectionStatus;
        std::string statusMessage;
        
        void handleTextInput(InputHandlerSystem& inputHandler);
        bool isValidIP(const std::string& ip);
        bool isValidPort(const std::string& port);
};

#endif // MULTIPLAYER_FORM_HPP