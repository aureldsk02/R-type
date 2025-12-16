/*
** EPITECH PROJECT, 2025
** G-CPP-500-COT-5-1-rtype-20 [WSL: Ubuntu-24.04]
** File description:
** Game
*/

#include "Game.hpp"

Game::Game()
{
}

Game::~Game()
{
    elements.clear();
}

void Game::addEntity(const std::string &type, const SpriteComponent &sprite, int id)
{
    printf("Adding entity - Type: %s, ID: %d\n", type.c_str(), id);
    elements.push_back(std::make_pair(createEntity(type, sprite), id));
    printf("Entity added successfully!\n");
}

void Game::Initialize()
{

}

std::vector<std::pair<Entit, int>> Game::getallEntity()
{
    return elements;
}

void Game::removeEntity(int id)
{
    for (auto it = elements.begin(); it != elements.end(); ++it) {
        if (it->second == id) {
            elements.erase(it);
            break;
        }
    }
}

void Game::Update(float deltaTime, int id, Vector2 position)
{
    for (auto &elem : elements) {
        if (elem.second == id) {
            elem.first.sprite.position = position;
            AnimationComponent& animation = elem.first.sprite.animation;
        if (animation.playing && !animation.frames.empty()) {
            animation.timer += deltaTime;
            if (animation.timer >= animation.frameTime) {
                animation.timer = 0.0f;
                animation.currentFrame += animation.direction;

                if (animation.pingpong) {
                    if (animation.currentFrame >= (int)animation.frames.size()) {
                        animation.direction = -1;
                        animation.currentFrame = animation.frames.size() - 2;
                    } else if (animation.currentFrame < 0) {
                        animation.direction = 1;
                        animation.currentFrame = 1;
                        // Do NOT set animation.playing = false here
                    }
                } else if (animation.loop) {
                    if (animation.currentFrame >= (int)animation.frames.size())
                        animation.currentFrame = 0;
                } else {
                    if (animation.currentFrame >= (int)animation.frames.size()) {
                        animation.currentFrame = animation.frames.size() - 1;
                        animation.playing = false;
                    }
                }
            }
            elem.first.sprite.sourceRect = animation.frames[animation.currentFrame];
            break;
            }
        }
    }
}

void Game::Cleanup()
{}

Entit Game::createEntity(const std::string &type, const SpriteComponent &sprite)
{
    printf("Creating entity - Type: %s\n", type.c_str());
    Entit newEntity;
    newEntity.type = type;
    newEntity.sprite = sprite;
    printf("Entity created successfully!\n");
    return newEntity;
}

void Game::setBackground(const SpriteComponent &sprite)
{
    printf("Setting background - Type: %s\n", sprite.type.c_str());
    background = sprite;
    printf("Background set successfully!\n");
}

SpriteComponent Game::getBackground()
{
    return background;
}

//