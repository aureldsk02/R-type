/*
** EPITECH PROJECT, 2025
** G-CPP-500-COT-5-1-rtype-20
** File description:
** Chat
*/

#ifndef CHAT_HPP
#define CHAT_HPP

#include <iostream>
#include <string>
#include <vector>
#include <deque>
#include <functional>
#include "Display_Handler.hpp"
#include "Input_Handler.hpp"

struct ChatMessage {
    std::string playerName;
    std::string message;
    float timestamp;
    Color color;
    
    ChatMessage(const std::string& name, const std::string& msg, float time = 0.0f, Color col = WHITE)
        : playerName(name), message(msg), timestamp(time), color(col) {}
};

class Chat {
    public:
        Chat();
        ~Chat();
        void Initialize();
        void Update(InputHandlerSystem& inputHandler);
        void Render(DisplayHandlerSystem& displayHandler);
        void Cleanup();
        
        // Chat functionality
        void AddMessage(const std::string& playerName, const std::string& message, Color color = WHITE);
        void SetVisible(bool visible) { isVisible = visible; }
        bool IsVisible() const { return isVisible; }
        void ToggleVisibility() { isVisible = !isVisible; if (!isVisible) { isInputActive = false; } }
        void SetInputActive(bool active) { isInputActive = active; if (active) justActivated = true; }
        bool IsInputActive() const { return isInputActive; }
        void SetJustActivated(bool activated) { justActivated = activated; }
        void SetPlayerName(const std::string& name) { currentPlayerName = name; }
        
        // Get current input message (for network sending)
        std::string GetCurrentInput() const { return currentInput; }
        void ClearCurrentInput() { currentInput = ""; }
        
        // Set callback for sending messages over network
        void SetSendMessageCallback(std::function<void(const std::string&)> callback) { 
            sendMessageCallback = callback; 
        }

    private:
        bool isVisible;
        bool isInputActive;
        bool justActivated; // Pour éviter de capturer le T d'activation
        std::string currentPlayerName;
        std::string currentInput;
        
        // Chat display settings
        Vector2 chatPosition;
        Vector2 chatSize;
        int maxMessages;
        int fontSize;
        int lineHeight;
        float messageLifetime; // How long messages stay visible (0 = forever)
        
        // Message storage
        std::deque<ChatMessage> messages;
        
        // Visual settings
        Color backgroundColor;
        Color borderColor;
        Color inputBackgroundColor;
        Color inputBorderColor;
        Color inputTextColor;
        
        // Network callback
        std::function<void(const std::string&)> sendMessageCallback;
        
        // Helper methods
        void handleTextInput(InputHandlerSystem& inputHandler);
        void removeOldMessages();
        std::vector<std::string> wrapText(const std::string& text, int maxWidth);
};

#endif // CHAT_HPP