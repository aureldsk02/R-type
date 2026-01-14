/* ------------------------------------------------------------------------------------
 * *
 *                                                                                      *
 * EPITECH PROJECT - Mon, Sep, 2025 * Title           -
 * G-CPP-500-COT-5-1-rtype-8                                          *
 * Description     - * Factory *
 *                                                                                      *
 * ------------------------------------------------------------------------------------
 * *
 *                                                                                      *
 *       _|_|_|_|  _|_|_|    _|_|_|  _|_|_|_|_|  _|_|_|_|    _|_|_|  _|    _| *
 *       _|        _|    _|    _|        _|      _|        _|        _|    _| *
 *       _|_|_|    _|_|_|      _|        _|      _|_|_|    _|        _|_|_|_| *
 *       _|        _|          _|        _|      _|        _|        _|    _| *
 *       _|_|_|_|  _|        _|_|_|      _|      _|_|_|_|    _|_|_|  _|    _| *
 *                                                                                      *
 * ------------------------------------------------------------------------------------
 */

/**
 * @brief The Factory file. Contains the declaration of the Factory class.
 *
 */
#ifndef INCLUDED_FACTORY_HPP
#define INCLUDED_FACTORY_HPP

#include <iostream>

#include "../../engine/include/components.hpp"
#include "../../engine/include/registry.hpp"

/**
 * @brief The Factory class. Handles the creation of entities.
 *  Since the entities of the ECS are defined by components, the Factory class
 *  is used to create predefined entities with the correct components
 * configuration in a simple and reusable way. The factory then holds various
 * functions, each one creating an entity with the correct components
 * configuration. The factory also holds a reference to the registry, which is
 * used to add the entities to the registry.
 */
class Factory {
  private:
    registry& reg;

  public:
    Factory(registry& _reg);

    /**
     * @brief Creates an entity with the correct components configuration
     * corresponding to the given type. If the type is not found, it returns -1
     *
     * @param type The type of the entity to be created
     * @return entity The id of the newly created entity
     */
    entity make_entity(const std::string& type);

    entity make_player1();

    entity make_player2();

    entity make_player3();
    
    entity make_player4();

    entity make_player_missile();

    entity make_enemy_missile();

    entity make_walker();

    entity make_red_trooper();

    entity make_plane();

    entity make_wall();

    entity make_floor();

    entity make_ceiling();

    entity make_background();

    entity make_explosion();

    entity make_title();

    entity make_start_text();

    entity make_menu_background_music();

    entity make_game_background_music();

    entity make_hit_effect();

    entity make_fade_in_rect();

    entity make_fade_out_rect();

    entity make_boss();

    entity make_green_trooper();

    entity make_tourelles();

    entity make_force();

    entity make_big_missile();

    entity make_final_boss();

    entity make_small_shooter();

    entity make_big_shooter();

    entity make_space_enemy();

    entity make_player2_flipped();

    entity make_evil_player_missile();
};

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
