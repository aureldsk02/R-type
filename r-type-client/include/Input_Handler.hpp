/*
** EPITECH PROJECT, 2025
** G-CPP-500-COT-5-1-rtype-20 [WSL: Ubuntu-24.04]
** File description:
** ECS Input Handler System
*/

#ifndef INPUT_HANDLER_HPP_
#define INPUT_HANDLER_HPP_

#include <raylib.h>
#include <unordered_map>
#include <vector>
#include <functional>
#include <string>

// Input action types
enum class InputKey {
    KEY_W,
    KEY_UP,
    KEY_S,
    KEY_DOWN,
    KEY_A,
    KEY_LEFT,
    KEY_RIGHT,
    KEY_D,
    KEY_P,
    KEY_T,
    KEY_SPACE,
    KEY_LEFT_SHIFT,
    KEY_ESCAPE,
    KEY_ENTER,
    KEY_KP_ADD,
    KEY_KP_SUBTRACT,
    MOUSE_BUTTON_LEFT,
    MOUSE_BUTTON_RIGHT,
    NONE
};

// Input event structure
struct InputEvent {
    InputKey Key_action;
    bool isPressed;
    bool isHeld;
    bool isReleased;
    Vector2 mousePosition;
};

// ECS Input Handler System
class InputHandlerSystem {
    private:
        std::unordered_map<int, bool> previousState;
        std::unordered_map<int, bool> currentState;
        std::unordered_map<int, InputKey> keyBindings;
        InputEvent currentEvent;
        Vector2 mousePosition {0, 0};
        bool initialized;

    public:
        InputHandlerSystem();
        ~InputHandlerSystem();

        InputEvent GetCurrentEvent();
        void Initialize();
        void Update();
        void Cleanup();
        void BindKey(InputKey key_pressed, int key);
        // Input state queries
        bool IsKeyPressed(InputKey Key) const;
        bool IsKeyHeld(InputKey Key) const;
        bool IsKeyReleased(InputKey Key) const;
        // Mouse functions
        Vector2 GetMousePosition() const { return mousePosition; }

};

#endif /* !INPUT_HANDLER_HPP_ */