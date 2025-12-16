/*
** EPITECH PROJECT, 2025
** G-CPP-500-COT-5-1-rtype-20 [WSL: Ubuntu-24.04]
** File description:
** ECS Input Handler System Implementation
*/

#include "Input_Handler.hpp"
#include <iostream>
#include <algorithm>

InputHandlerSystem::InputHandlerSystem() : initialized(false) {}

InputHandlerSystem::~InputHandlerSystem() {
    Cleanup();
}

void InputHandlerSystem::BindKey(InputKey key_pressed, int key) {
    keyBindings[key] = key_pressed;
    currentState[key] = false;
    previousState[key] = false;
}

InputEvent InputHandlerSystem::GetCurrentEvent() {
    return currentEvent;
}

void InputHandlerSystem::Initialize(){

    keyBindings.clear();
    previousState.clear();
    currentState.clear();

    BindKey(InputKey::KEY_W, KEY_W);
    BindKey(InputKey::KEY_UP, KEY_UP);
    BindKey(InputKey::KEY_S, KEY_S);
    BindKey(InputKey::KEY_DOWN, KEY_DOWN);
    BindKey(InputKey::KEY_A, KEY_A);
    BindKey(InputKey::KEY_LEFT, KEY_LEFT);
    BindKey(InputKey::KEY_D, KEY_D);
    BindKey(InputKey::KEY_RIGHT, KEY_RIGHT);
    BindKey(InputKey::KEY_T, KEY_T);
    BindKey(InputKey::KEY_SPACE, KEY_SPACE);
    BindKey(InputKey::KEY_LEFT_SHIFT, KEY_LEFT_SHIFT);
    BindKey(InputKey::KEY_ESCAPE, KEY_ESCAPE);
    BindKey(InputKey::KEY_ENTER, KEY_ENTER);
    BindKey(InputKey::KEY_KP_ADD, KEY_KP_ADD);
    BindKey(InputKey::KEY_KP_SUBTRACT, KEY_KP_SUBTRACT);
    BindKey(InputKey::MOUSE_BUTTON_LEFT, MOUSE_BUTTON_LEFT);
    BindKey(InputKey::MOUSE_BUTTON_RIGHT, MOUSE_BUTTON_RIGHT);

    initialized = true;
}

void InputHandlerSystem::Update()
{
    if (!initialized)
        return;

    mousePosition = GetMousePosition();

    // Copy current to previous
    for (const auto &pair : currentState)
        previousState[pair.first] = pair.second;

    // Update current states
    for (auto &[keycode, inputKey] : keyBindings) {
        bool state = false;
        if (keycode >= MOUSE_BUTTON_LEFT && keycode <= MOUSE_BUTTON_RIGHT)
            state = IsMouseButtonDown(keycode);
        else
            state = IsKeyDown(keycode);
        currentState[keycode] = state;
    }

    // Build current event
    currentEvent = {};
    currentEvent.mousePosition = mousePosition;
    currentEvent.Key_action = InputKey::NONE;

    for (auto &[keycode, inputKey] : keyBindings) {
        if (IsKeyPressed(inputKey) || IsKeyHeld(inputKey) || IsKeyReleased(inputKey)) {
            currentEvent.Key_action = inputKey;
            currentEvent.isPressed = IsKeyPressed(inputKey);
            currentEvent.isHeld = IsKeyHeld(inputKey);
            currentEvent.isReleased = IsKeyReleased(inputKey);
            break;
        }
    }
}

void InputHandlerSystem::Cleanup() {
    if (initialized) {
        keyBindings.clear();
        previousState.clear();
        currentState.clear();
        initialized = false;
        std::cout << "InputHandlerSystem: Cleaned up" << std::endl;
    }
}

bool InputHandlerSystem::IsKeyPressed(InputKey Key) const
{
    for (auto &[keycode, mappedKey] : keyBindings) {
        if (mappedKey == Key) {
            bool prev = previousState.at(keycode);
            bool curr = currentState.at(keycode);
            return (curr && !prev);
        }
    }
    return false;
}

bool InputHandlerSystem::IsKeyHeld(InputKey Key) const
{
    for (auto &[keycode, mappedKey] : keyBindings) {
        if (mappedKey == Key) {
            bool prev = previousState.at(keycode);
            bool curr = currentState.at(keycode);
            return (curr && prev);
        }
    }
    return false;
}

bool InputHandlerSystem::IsKeyReleased(InputKey Key) const
{
    for (auto &[keycode, mappedKey] : keyBindings) {
        if (mappedKey == Key) {
            bool prev = previousState.at(keycode);
            bool curr = currentState.at(keycode);
            return (!curr && prev);
        }
    }
    return false;
}

// --- IGNORE ---