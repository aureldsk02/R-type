/*
** EPITECH PROJECT, 2025
** system
** File description:
** system
*/

#pragma once

class ISystem
{
public:
    virtual ~ISystem() = default;
    virtual void update(float dt) = 0;
};
