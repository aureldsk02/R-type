/* ------------------------------------------------------------------------------------
 * *
 *                                                                                      *
 * EPITECH PROJECT - Wed, Sep, 2025 * Title           - r-type_bs * Description
 * -                                                                    *
 *     registry *
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

#ifndef INCLUDED_REGISTRY_HPP
#define INCLUDED_REGISTRY_HPP

/**
 * @file registry.hpp
 * @author Aurel DOSSOU KOHI
 * @brief This file defines the registry class. The registry is a container for
 * all the entities and components of the game. It is home to the ECS engine. It
 * stores the entities and the components and provides ways to interact with
 * them. It also provides the systems that will be executed at each frame to
 * update the state of the entities and their components
 * @version 0.1
 * @date 2025-10-13
 *
 * @copyright Copyright (c) 2026 - Aurel DOSSOU KOHI
 *
 */

#include <algorithm>
#include <any>
#include <functional>
#include <iostream>
#include <optional>
#include <typeindex>
#include <unordered_map>
#include <vector>

#include <SFML/Graphics.hpp>

#include "ResourceManager.hpp"
#include "entity.hpp"
#include "exceptions.hpp"

using namespace std;

/**
 * @brief The registry class. The registry is a container for all the entities
 * and components of the game. It is home to the ECS engine. It stores the
 * entities and the components and provides ways to interact with them. It also
 * provides the systems that will be executed at each frame to update the state
 * of the entities and their components.
 *
 */
class registry {
  public:
    void toggleMovement() {movement_active = !movement_active;};
    void togglePhysics() {collisions_active = !collisions_active;};
    void toggleRendering() {render_active = !render_active;};
    void toggleControl() {control_active = !control_active;};
    void toggleLogic() {logic_active = !logic_active;};

    bool  getMovement() {return movement_active;};
    bool  getPhysics() {return collisions_active;};
    bool  getRendering() {return render_active;};
    bool  getControl() {return control_active;};
    bool  getLogic() {return logic_active;};

    /**
     * @brief A vector of entities id that are dead and can be reused
     *
     */
    vector<entity> dead_entities;

    registry(sf::RenderWindow& window);
    registry();

    /* Tables management */
    template <typename Component>
    vector<optional<Component>>& register_components();

    template <typename Component> vector<optional<Component>>& get_components();

    template <typename Component>
    vector<optional<Component>> const& get_components() const;

    /* Single component management */
    template <typename Component> void remove_component(entity const& from);

    template <typename Component>
    Component& add_component(entity const& to, Component&& c);

    /* Entities managament */
    entity spawn_entity();
    void kill_entity(const entity& e);

    /* Systems managment */
    void add_system(const function<void(double)>& system);
    void run_systems(double delta);

    /* Getters */
    /**
     * @brief Get the highest entity id
     *
     * @return size_t
     */
    size_t getEntityNum(void) const {
        return entity_num;
    }

    /**
     * @brief Get the number of registered types of components
     *
     * @return size_t
     */
    size_t getComponentNum(void) const {
        return _components_arrays.size();
    }

    /**
     * @brief Get the number of registered erase functions
     *
     * @return size_t
     */
    size_t getEraseFunctionNum(void) const {
        return _erase_functions.size();
    }

    /**
     * @brief Get the window
     *
     * @return sf::RenderWindow&
     */
    sf::RenderWindow& get_window() {
        return window;
    }

  private:
    unordered_map<type_index, any> _components_arrays;
    unordered_map<type_index, function<void(const entity&)>> _erase_functions;

    vector<function<void(double)>> _systems;

    size_t entity_num = 0;

    sf::RenderWindow& window;
    sf::RenderWindow tmp;

    sf::Clock clock;

    void register_all_systems();
    /**
     * @brief A boolean that indicates if the collisions system is active
     *
     */
    bool collisions_active = false;
    /**
     * @brief A boolean that indicates if the render system is active
     *
     */
    bool render_active = false;
    /**
     * @brief A boolean that indicates if the logic system is active
     *
     */
    bool logic_active = false;
    /**
     * @brief A boolean that indicates if the control system is active
     *
     */
    bool control_active = false;

    /**
     * @brief A boolean that indicates if the movement system is active
     * 
     */
    bool movement_active = false;
};

#include "../src/registry_single_comp.tpp"
#include "../src/registry_tables.tpp"

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
