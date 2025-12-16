/*
** EPITECH PROJECT, 2025
** G-CPP-500-COT-5-1-rtype-20
** File description:
** stages
*/

#ifndef STAGES_HPP_
#define STAGES_HPP_

#include "Display_Handler.hpp"
#include <string>
#include <vector>

struct StageComponent {
    std::string name;
    std::vector<std::string> backgroundSpriteNames;
    std::vector<SpriteComponent> enemies;
    std::vector<RectangleComponent> colliders;
    bool completed = false;
    float duration; // Example: stage duration in seconds
    float timer = 0.0f;
};


#endif /* !STAGES_HPP_ */