/*
** EPITECH PROJECT, 2025
** G-CPP-500-COT-5-1-rtype-20 [WSL: Ubuntu-24.04]
** File description:
** Game
*/

#ifndef GAME_HPP
#define GAME_HPP

#include <iostream>
#include <string>
#include <chrono>
#include <thread>
#include "Display_Handler.hpp"
#include "Input_Handler.hpp"
#include "Audio_Handler.hpp"

struct Entit {
    std::string type;
    SpriteComponent sprite;
    int Id;

    Entit() : type(""), sprite(), Id(0) {}
};

class Game {
    public:
        Game();
        ~Game();
        void Initialize();
        void Update(float deltaTime, int id, Vector2 position);
        void Cleanup();
        void removeEntity(int id);
        void addEntity(const std::string &type, const SpriteComponent &sprite, int id);
        void setBackground(const SpriteComponent &sprite);
        void setStars(const SpriteComponent &sprite) {stars = sprite;};
        SpriteComponent getStars() {return stars;};
        std::vector<std::pair<Entit, int>> getallEntity();
        SpriteComponent getBackground();
    private:
        Entit createEntity(const std::string &type, const SpriteComponent &sprite);
        std::vector<std::pair<Entit, int>> elements;
        SpriteComponent background;
        SpriteComponent stars;
};


#endif // GAME_HPP