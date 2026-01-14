/* ------------------------------------------------------------------------------------
 * *
 *                                                                                      *
 * EPITECH PROJECT - Thu, Sep, 2025 * Title           - r-type_bs * Description
 * -                                                                    *
 *     components *
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

#ifndef INCLUDED_COMPONENTS_HPP
#define INCLUDED_COMPONENTS_HPP

/**
 * @file components.hpp
 * @author Aurel DOSSOU KOHI
 * @brief This file defines the different types of components available to the
 * engine
 * @version 0.1
 * @date 2025-09-23
 *
 * @copyright Copyright (c) 2026 - Aurel DOSSOU KOHI
 *
 */

#include <memory>
#include <string>

#include "ResourceManager.hpp"
#include "entity.hpp"
#include "registry.hpp"

extern int unique_ids;

namespace component {
    /**
     * @brief The position component, with an x and y position
     */
    typedef struct position_s {
        double x;
        double y;
        double z;
        /**
         * @brief This function helps increment the position by an x and y
         * offset
         *
         * @param _x
         * @param _y
         */
        void setPosition(double _x, double _y) {
            this->x += _x;
            this->y += _y;
        }
    } position;

    /**
     * @brief The velocity component, with an x and y velocity. This component
     * allows an entity to move
     *
     */
    typedef struct velocity_s {
        double vx;
        double vy;
    } velocity;

    /**
     * @brief The drawable component, defined by an sf::Sprite. This component
     * allows an entity to be drawn on the screen
     *
     */
    typedef struct drawable_s {
        sf::Sprite sprite;

        /**
         * @brief This function uses a ResourceManager to set the texture of the
         * sprite
         *
         * @param texture_name The name under which the texture is stored in the
         * ResourceManager
         */
        void setTextureFromName(std::string texture_name) {
            sf::Texture& texture =
                ResourceManager::Instance().getTexture(texture_name);
            this->sprite.setTexture(texture);
        }
    } drawable;

    /**
     * @brief The text component, consisting in a simple sf::Text
     *
     */
    typedef struct text_s {
        sf::Text text;

        /**
         * @brief This function uses a ResourceManager to set the font of the
         * text
         *
         * @param font_name The name under which the font is stored in the
         * ResourceManager
         */
        void setFontFromName(std::string font_name) {
            sf::Font& font = ResourceManager::Instance().getFont(font_name);
            this->text.setFont(font);
        }
    } text;

    /**
     * @brief The animated_drawable component. It is a drawable component with
     * an animated sprite. It has a frame_rect, a frame_duration and a
     * frame_timer to allow animation
     *
     */
    typedef struct animated_drawable_s {
        bool one_shot;
        bool done_once;
        sf::Sprite sprite;
        sf::IntRect frame_rect;
        double frame_duration;
        double frame_timer;

        /**
         * @brief Set the Texture of the sprite to the texture associated with
         * the name parameter in the ResourceManager
         *
         * @param texture_name the name of the texture in the resource manager
         */
        void setTextureFromName(std::string texture_name) {
            one_shot = false;
            done_once = false;
            sf::Texture& texture =
                ResourceManager::Instance().getTexture(texture_name);
            this->sprite.setTexture(texture);
        }

        /**
         * @brief Set the dimensions of the rect representing a frame
         *
         * @param width
         * @param height
         */
        void setFrameRect(int width, int height) {
            this->frame_rect = sf::IntRect(0, 0, width, height);
            this->sprite.setTextureRect(this->frame_rect);
        }

        /**
         * @brief This function animates the sprite
         *
         * @param delta the time elapsed since the last frame
         */
        void animate(double delta) {
            this->frame_timer += delta;

            if (this->frame_timer >= this->frame_duration) {
                this->frame_timer = 0;
                this->frame_rect.left += this->frame_rect.width;
                if (this->frame_rect.left >=
                    this->sprite.getTexture()->getSize().x - 5) {
                    this->frame_rect.left = 0;
                    done_once = true;
                }
                this->sprite.setTextureRect(this->frame_rect);
            }
        }
    } animated_drawable;

    /**
     * @brief The controllable component, defined by a set of booleans. This
     * component allows an entity to be controlled by the user's keyboard input
     *
     */
    typedef struct controllable_s {
        sf::Keyboard::Key up_key;
        sf::Keyboard::Key down_key;
        sf::Keyboard::Key left_key;
        sf::Keyboard::Key right_key;
        sf::Keyboard::Key space_key;

        /**
         * @var bool up
         * @brief  Check if up is pressed
         */
        bool up;

        /**
         * @var bool down
         * @brief  Check if down is pressed
         */
        bool down;

        /**
         * @var bool left
         * @brief  Check if left is pressed
         */
        bool left;

        /**
         * @var bool right
         * @brief  Check if right is pressed
         */
        bool right;

        /**
         * @var bool space
         * @brief  Check if space is pressed
         */
        bool space;

        /**
         * @brief Default constructor
         */
        controllable_s()
            : up(false), down(false), left(false), right(false), space(false) {
            up_key = sf::Keyboard::Up;
            down_key = sf::Keyboard::Down;
            left_key = sf::Keyboard::Left;
            right_key = sf::Keyboard::Right;
            space_key = sf::Keyboard::Space;
        }

        controllable_s(
            sf::Keyboard::Key _up, sf::Keyboard::Key _down,sf::Keyboard::Key _left,
            sf::Keyboard::Key _right, sf::Keyboard::Key _space
        )
            : up(false), down(false), left(false), right(false), space(false),
            up_key(_up), down_key(_down), left_key(_left), right_key(_right), space_key(_space) {

        }

        /**
         * @brief Constructor with preinitialized value
         *
         * @param u True if Up is Pressed False if not
         * @param d True if Down is Pressed False if not
         * @param l True if Left is Pressed False if not
         * @param r True if Right is Pressed False if not
         * @param s True if Space is Pressed False if not
         */
        controllable_s(bool u, bool d, bool l, bool r, bool s)
            : up(u), down(d), left(l), right(r), space(s) {}

        /**
         * @brief Get the key that has been pressed
         */
        void getKeyboardInput() {
            this->up = sf::Keyboard::isKeyPressed(up_key);
            this->down = sf::Keyboard::isKeyPressed(down_key);
            this->left = sf::Keyboard::isKeyPressed(left_key);
            this->right = sf::Keyboard::isKeyPressed(right_key);
            this->space = sf::Keyboard::isKeyPressed(space_key);
        }
    } controllable;

    /**
     * @brief The hurtbox component, defined by a size, health and a group (1
     * for the player, 2 for the enemies for example). A hitbox will detect
     * collision with a hurtbox if its targeted group corresponds to the hurtbox
     * group
     *
     */
    typedef struct hurtbox_s {
        /**
         * @brief The health of the hurtbox. When it reaches 0, the entity is
         * considered dead
         */
        int health;

        /**
         * @brief The group of the hurtbox. For example, if the hurtbox group is
         * 1 for the player and 2 for the enemies, the player's hitbox should
         * have a targeted_group of 2 to damage enemies
         */
        int group;

        /**
         * @brief The width of the hurtbox
         */
        int width;

        /**
         * @brief The height of the hurtbox
         */
        int height;

        /**
         * @brief A bool to indicate if the hurtbox is hurt
         *
         */
        bool hurt;
    } hurtbox;

    /**
     * @brief The hitbox component, defined by a size, damage and a
     * targeted_group (2 for the player, 1 for the enemies for example). A
     * hitbox will detect collision with a hurtbox if its targeted group
     * corresponds to the hurtbox group
     *
     */
    typedef struct hitbox_s {
        /**
         * @brief The damage that will be inflicted to the hurtbox on collision
         */
        int damage;

        /**
         * @brief The group that this hitbox will target. For example, if the
         * hurtbox group is 1 for the player and 2 for the enemies, the player's
         * hitbox should have a targeted_group of 2 to damage enemies
         */
        int targeted_group;

        /**
         * @brief The width of the hitbox
         */
        int width;

        /**
         * @brief The height of the hitbox
         */
        int height;

        /**
         * @brief If this bool is set to true, the hitbox will be destroyed on
         * collision
         *
         */
        bool one_shot;
    } hitbox;

    /**
     * @brief The logic component, defined by a function pointer. This component
     * allows an entity to have a logic defined by the user
     */
    typedef struct logic_s {
        /**
         * @brief A pointer to a function that takes a delta time, a reference
         * to the registry and an entity as parameters and returns void
         * @param delta The time elapsed since the last frame
         * @param reg A reference to the registry
         * @param entity The entity that owns this logic component
         */
        void (*logic_function)(double, class registry&, entity);
    } logic;

    /**
     * @brief The name component, defined by a string. It lets you give a name
     * to each entity
     *
     */
    typedef struct name_s {
        std::string _name;
    } name;

    /**
     * @brief The audio component defined by a single sf::Music. This component
     * is not used by the engine systems but can be directly played, stopped,
     * paused by the user
     *
     */
    typedef struct audio_s {
        std::shared_ptr<sf::Music> audio;
    } audio;

    /**
     * @brief The unique_id component, defined by an int. It lets you give a
     * unique id to each entity
     *
     */
    typedef int unique_id;
} // namespace component

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
