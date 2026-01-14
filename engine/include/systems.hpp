/* ------------------------------------------------------------------------------------
 * *
 *                                                                                      *
 * EPITECH PROJECT - Mon, Sep, 2025 * Title           -
 * G-CPP-500-COT-5-1-rtype-8                                          *
 * Description     - * systems *
 *                                                                                      *
 * ------------------------------------------------------------------------------------
 * *
 *                                                                                      *
 *       ▄▀▀█▄▄▄▄  ▄▀▀▄▀▀▀▄  ▄▀▀█▀▄    ▄▀▀▀█▀▀▄  ▄▀▀█▄▄▄▄  ▄▀▄▄▄▄   ▄▀▀▄ ▄▄ * ▐
 * ▄▀   ▐ █   █   █ █   █  █  █    █  ▐ ▐  ▄▀   ▐ █ █    ▌ █  █   ▄▀ * █▄▄▄▄▄  ▐
 * █▀▀▀▀  ▐   █  ▐  ▐   █       █▄▄▄▄▄  ▐ █      ▐  █▄▄▄█             * █    ▌
 * █          █        █        █    ▌    █         █   █             * ▄▀▄▄▄▄
 * ▄▀        ▄▀▀▀▀▀▄   ▄▀        ▄▀▄▄▄▄    ▄▀▄▄▄▄▀   ▄▀  ▄▀             * █    ▐
 * █         █       █ █          █    ▐   █     ▐   █   █               * ▐ ▐
 * ▐       ▐ ▐          ▐        ▐         ▐   ▐               *
 *                                                                                      *
 * ------------------------------------------------------------------------------------
 */

#ifndef INCLUDED_SYSTEMS_HPP
#define INCLUDED_SYSTEMS_HPP

/**
 * @file systems.hpp
 * @author Aurel DOSSOU KOHI
 * @brief This file contains the declaration of the different systems of the
 * registry. Systems are functions that will be executed at each frame to update
 * the state of all the components
 * @version 0.1
 * @date 2025-10-13
 *
 * @copyright Copyright (c) 2026 - Aurel DOSSOU KOHI
 *
 */

#include "components.hpp"
#include "registry.hpp"

/**
 * @brief The position system goes through each entity and updates its position
 * based on its velocity
 *
 * @param reg The reference to the registry
 * @param positions The table of positions components
 * @param velocities The table of velocities components
 * @param controls The table of control components
 */
void position_system(
    double delta, registry& reg,
    std::vector<optional<component::position>>& positions,
    std::vector<optional<component::velocity>>& velocities
);

/**
 * @brief
 *
 * @param reg  The reference to the registry
 * @param window The reference to a sf::RenderWindow
 * @param positions The table of positions components
 * @param draws The table of drawable components
 */
void draw_system(
    double delta, registry& reg, sf::RenderWindow& window,
    std::vector<optional<component::position>>& positions,
    std::vector<optional<component::drawable>>& draws,
    std::vector<optional<component::animated_drawable>>& anim_draws,
    std::vector<optional<component::text>>& texts
);

/**
 * @brief The control system goes through each constrollable component and
 * updates its state based on the player input
 *
 * @param reg The reference to the registry
 * @param controls The controllable components
 */
void control_system(
    double delta, registry& reg,
    std::vector<optional<component::controllable>>& controls
);

/**
 * @brief The collision system goes through each hurtbox and hitbox component
 * and checks if they are colliding
 *
 * @param reg The reference to the registry
 * @param positions The positions components
 * @param hurtboxes The hurtbox components
 * @param hitboxes The hitbox components
 */
void collision_system(
    double delta, registry& reg,
    std::vector<optional<component::position>>& positions,
    std::vector<optional<component::hurtbox>>& hurtboxes,
    std::vector<optional<component::hitbox>>& hitboxes
);

/**
 * @brief The logic system goes through each entity with a logic component and
 * executes its logic function
 * @param reg The reference to the registry
 * @param logics The logic components
 */
void logic_system(
    double delta, registry& reg, std::vector<optional<component::logic>>& logics
);
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
