/*
** EPITECH PROJECT, 2025
** G-CPP-500-COT-5-1-rtype-20
** File description:
** Chat
*/

#include "Chat.hpp"
#include <raylib.h>
#include <algorithm>

Chat::Chat() 
    : isVisible(false), isInputActive(false), justActivated(false), currentPlayerName("Player"), currentInput(""),
      chatPosition{0, 0}, chatSize{280, 300}, maxMessages(12), fontSize(14), lineHeight(18),
      messageLifetime(0.0f), // 0 = messages never expire
      backgroundColor{0, 0, 0, 200}, borderColor{150, 150, 150, 255},
      inputBackgroundColor{20, 20, 20, 200}, inputBorderColor{150, 150, 150, 255},
      inputTextColor{255, 255, 255, 255} {
}

Chat::~Chat() {
    Cleanup();
}

void Chat::Initialize() {
    // Position chat on the left side
    Vector2 screenSize = {static_cast<float>(GetScreenWidth()), static_cast<float>(GetScreenHeight())};
    chatPosition.x = 20; // 20px margin from left
    chatPosition.y = 150; // Start below top UI elements
    chatSize.x = 280; // Smaller width for side panel
    chatSize.y = 300; // Fixed smaller height
    

}

void Chat::Update(InputHandlerSystem& inputHandler) {
    if (!isVisible) return;
    
    if (isInputActive) {
        // Si on vient d'activer, ignorer la saisie cette frame
        if (justActivated) {
            justActivated = false;
            return;
        }
        
        handleTextInput(inputHandler);
        
        // Send message with ENTER
        if (inputHandler.IsKeyPressed(InputKey::KEY_ENTER)) {
            if (!currentInput.empty()) {
                // Envoyer via le réseau si le callback est défini
                if (sendMessageCallback) {
                    sendMessageCallback(currentInput);
                } else {
                    // Si pas de callback réseau, ajouter localement (mode solo)
                    AddMessage(currentPlayerName, currentInput, Color{255, 255, 255, 255});
                }
                
                currentInput = "";
            }
            isInputActive = false;
            justActivated = false;
        }
        
        // Note: ESC key is now handled in GameClient.cpp to close chat
    }
    
    // Remove old messages if lifetime is set
    if (messageLifetime > 0.0f) {
        removeOldMessages();
    }
}

void Chat::Render(DisplayHandlerSystem& displayHandler) {
    if (!isVisible) return;
    
    // Update position and size based on current screen size
    Vector2 screenSize = {static_cast<float>(GetScreenWidth()), static_cast<float>(GetScreenHeight())};
    chatPosition.x = 20; // Keep on left side
    chatPosition.y = 150;
    chatSize.x = 280; // Smaller width
    chatSize.y = 300; // Fixed smaller height
    
    // Draw chat border
    displayHandler.DrawRectangle({chatPosition.x - 2, chatPosition.y - 2}, 
                                {chatSize.x + 4, chatSize.y + 4}, borderColor);
    
    // Draw chat background using DisplayHandler
    displayHandler.DrawRectangle(chatPosition, chatSize, backgroundColor);
    
    // Draw chat title using DisplayHandler
    TextComponent titleText;
    titleText.content = "CHAT";
    titleText.position = {chatPosition.x + 10, chatPosition.y + 5};
    titleText.fontSize = 16;
    titleText.color = Color{200, 200, 200, 255};
    titleText.visible = true;
    displayHandler.DrawText(titleText);
    
    // Draw messages
    int yOffset = 30; // Start below title
    int maxVisibleMessages = (chatSize.y - 60) / lineHeight; // Reserve space for input
    
    // Draw messages from newest to oldest, but display oldest to newest
    for (int i = std::max(0, (int)messages.size() - maxVisibleMessages); i < (int)messages.size(); i++) {
        const ChatMessage& msg = messages[i];
        
        // Format message: [PlayerName]: Message
        std::string fullMessage = "[" + msg.playerName + "]: " + msg.message;
        
        // Wrap text if too long
        std::vector<std::string> wrappedLines = wrapText(fullMessage, chatSize.x - 20);
        
        for (const std::string& line : wrappedLines) {
            if (yOffset + lineHeight > chatSize.y - 40) break; // Don't overflow into input area
            
            int drawX = chatPosition.x + 10;
            int drawY = chatPosition.y + yOffset;
            
            // Utiliser DisplayHandler pour dessiner le texte
            TextComponent textComp;
            textComp.content = line;
            textComp.position = {static_cast<float>(drawX), static_cast<float>(drawY)};
            textComp.fontSize = 14;
            textComp.color = msg.color;
            textComp.visible = true;
            displayHandler.DrawText(textComp);
            
            yOffset += lineHeight;
        }
    }
    
    // Draw input area
    Vector2 inputPos = {chatPosition.x, chatPosition.y + chatSize.y + 5};
    Vector2 inputSize = {chatSize.x, 30};
    
    Color inputBg = isInputActive ? inputBackgroundColor : Color{inputBackgroundColor.r, inputBackgroundColor.g, inputBackgroundColor.b, 100};
    displayHandler.DrawRectangle(inputPos, inputSize, inputBg);
    
    // Draw input text using DisplayHandler
    std::string displayText = isInputActive ? (currentInput + "|") : "Tapez votre message... (ESC pour fermer)";
    Color textColor = isInputActive ? inputTextColor : Color{150, 150, 150, 255};
    
    TextComponent inputTextComp;
    inputTextComp.content = displayText;
    inputTextComp.position = {inputPos.x + 5, inputPos.y + 7};
    inputTextComp.fontSize = 12;
    inputTextComp.color = textColor;
    inputTextComp.visible = true;
    displayHandler.DrawText(inputTextComp);
    
}

void Chat::Cleanup() {
    messages.clear();
}

void Chat::AddMessage(const std::string& playerName, const std::string& message, Color color) {
    messages.emplace_back(playerName, message, GetTime(), color);
    
    // Limit number of messages
    while ((int)messages.size() > maxMessages) {
        messages.pop_front();
    }
}

void Chat::handleTextInput(InputHandlerSystem& /* inputHandler */) {
    // Si on vient d'activer le chat, ignorer la première saisie pour éviter le T d'activation
    if (justActivated) {
        // Vider la queue des caractères pour ignorer le T d'activation
        while (GetCharPressed() > 0) {
            // Ignorer tous les caractères en attente
        }
        justActivated = false;
        return;
    }
    
    // Handle backspace
    int key = GetKeyPressed();
    if (key == KEY_BACKSPACE) {
        if (!currentInput.empty()) {
            currentInput.pop_back();
        }
    }
    
    // Handle character input
    while ((key = GetCharPressed()) > 0) {
        char c = static_cast<char>(key);
        
        // Allow letters, numbers, spaces, and basic punctuation
        if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || 
            (c >= '0' && c <= '9') || c == ' ' || c == '!' || c == '?' || 
            c == '.' || c == ',' || c == ':' || c == ';' || c == '-' || c == '_') {
            if (currentInput.length() < 100) { // Limit message length
                currentInput += c;
            }
        }
    }
}

void Chat::removeOldMessages() {
    float currentTime = GetTime();
    while (!messages.empty() && (currentTime - messages.front().timestamp) > messageLifetime) {
        messages.pop_front();
    }
}

std::vector<std::string> Chat::wrapText(const std::string& text, int maxWidth) {
    std::vector<std::string> lines;
    
    if (MeasureText(text.c_str(), fontSize - 2) <= maxWidth) {
        lines.push_back(text);
        return lines;
    }
    
    // Simple word wrapping
    std::string currentLine = "";
    std::string word = "";
    
    for (char c : text) {
        if (c == ' ') {
            std::string testLine = currentLine + (currentLine.empty() ? "" : " ") + word;
            if (MeasureText(testLine.c_str(), fontSize - 2) <= maxWidth) {
                currentLine = testLine;
            } else {
                if (!currentLine.empty()) {
                    lines.push_back(currentLine);
                }
                currentLine = word;
            }
            word = "";
        } else {
            word += c;
        }
    }
    
    // Handle last word
    if (!word.empty()) {
        std::string testLine = currentLine + (currentLine.empty() ? "" : " ") + word;
        if (MeasureText(testLine.c_str(), fontSize - 2) <= maxWidth) {
            currentLine = testLine;
        } else {
            if (!currentLine.empty()) {
                lines.push_back(currentLine);
            }
            currentLine = word;
        }
    }
    
    if (!currentLine.empty()) {
        lines.push_back(currentLine);
    }
    
    return lines;
}