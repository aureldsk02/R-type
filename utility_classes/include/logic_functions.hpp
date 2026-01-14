/* ------------------------------------------------------------------------------------
 * *
 *                                                                                      *
 * EPITECH PROJECT - Tue, Sep, 2025 * Title           -
 * G-CPP-500-COT-5-1-rtype-8                                          *
 * Description     - * logics_functions *
 *                                                                                      *
 * ------------------------------------------------------------------------------------
 * *
 *                                                                                      *
 *         ░        ░       ░░        ░        ░        ░░      ░░  ░░░░  ░ * ▒
 * ▒▒▒▒▒▒▒  ▒▒▒▒  ▒▒▒▒  ▒▒▒▒▒▒▒  ▒▒▒▒  ▒▒▒▒▒▒▒  ▒▒▒▒  ▒  ▒▒▒▒  ▒             *
 *         ▓      ▓▓▓       ▓▓▓▓▓  ▓▓▓▓▓▓▓  ▓▓▓▓      ▓▓▓  ▓▓▓▓▓▓▓        ▓ * █
 * ███████  ██████████  ███████  ████  ███████  ████  █  ████  █             *
 *         █        █  ███████        ████  ████        ██      ██  ████  █ *
 *                                                                                      *
 * ------------------------------------------------------------------------------------
 */

/**
 * @brief This file contains the logic functions declarations
 * Logic functions are update functions that can be attached to an entity
 * in order to give it a behavior. They are executed at each frame
 *
 */

#ifndef INCLUDED_LOGICS_FUNCTIONS_HPP
#define INCLUDED_LOGICS_FUNCTIONS_HPP

#include "components.hpp"
#include "registry.hpp"

#define PLAYER_MISSISLE_SPEED 10
#define ENEMY_MISSISLE_SPEED 5
#define SCROLL_SPEED 5
#define PLAYER_SPEED 300
#define TROOPER_SPEED_X 150
#define TROOPER_SPEED_Y 60
#define PLAYER_SHOOT_COOLDOWN 0.25
#define ENEMY_SHOOT_COOLDOWN 2
#define BOSS_SHOOT_COOLDOWN 2
#define BOSS_SPAWN_COOLDOWN 5
#define FORCE_SHOOT_COOLDOWN 0.35
#define BOSS_MISSILE_SPEED 200
#define BOSS_SPEED 100

extern bool boss_dead;

extern bool boss1_dead;
extern bool boss2_dead;

extern bool final_boss_dead;

void player_logic(double delta, registry& reg, entity entity);
void red_trooper_logic(double delta, registry& reg, entity entity);
void plane_logic(double delta, registry& reg, entity en);
void walker_logic(double delta, registry& reg, entity entity);
void boss_logic(double delta, registry& reg, entity entity);
void start_text_logic(double delta, registry& reg, entity entity);
void fade_in_rect_logic(double delta, registry& reg, entity entity);
void fade_out_rect_logic(double delta, registry& reg, entity entity);
void force_logic(double delta, registry& reg, entity entity);
void gtrooper_logic(double delta, registry& reg, entity en);
void tourelles_logic(double delta, registry& reg, entity en);
void final_boss_logic(double delta, registry& reg, entity en);
void big_shooter_logic(double delta, registry& reg, entity en);
void small_shooter_logic(double delta, registry& reg, entity en);
void spacenemy_logic(double delta, registry& reg, entity en);
void player_evil_logic(double delta, registry& reg, entity en);

#endif

/* ------------------------------------------------------------------------------------
 * *
 *                                                                                      *
 * MIT License * Copyright (c) 2026 Aurel DOSSOU KOHI *
 *                                                                                      *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * * of this software and associated documentation files (the "Software"), to
 * deal        * in the Software without restriction, including without
 * limitation the rights         * to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell            * copies of the Software, and
 * to permit persons to whom the Software is                * furnished to do
 * so, subject to the following conditions:                             *
 *                                                                                      *
 * The above copyright notice and this permission notice shall be included in
 * all       * copies or substantial portions of the Software. *
 *                                                                                      *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR *
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, *
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE *
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER *
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE        * SOFTWARE. *
 *                                                                                      *
 * ------------------------------------------------------------------------------------
 */
