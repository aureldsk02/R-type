/* ------------------------------------------------------------------------------------
 * *
 *                                                                                      *
 * EPITECH PROJECT - Mon, Sep, 2025 * Title           -
 * G-CPP-500-COT-5-1-rtype-8                                          *
 * Description     - * registry *
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

/**
 * @file registry_systems.cpp
 * @author Aurel DOSSOU KOHI
 * @brief This file contains the definition of the registry functions that
 * handle the systems
 * @version 0.1
 * @date 2025-09-23
 *
 * @copyright Copyright (c) 2026 - Aurel DOSSOU KOHI
 *
 */

#include "../include/registry.hpp"
#include "../include/systems.hpp"

/**
 * @brief This function registers all the systems in the registry. Systems are
 * functions that will be stored in the registry, and then executed at each
 * frame to update the game state.
 */
void registry::register_all_systems() {
    register_components<component::position>();
    register_components<component::velocity>();
    register_components<component::controllable>();
    register_components<component::hurtbox>();
    register_components<component::hitbox>();
    register_components<component::logic>();
    register_components<component::audio>();
    register_components<component::name>();
    register_components<component::unique_id>();
    register_components<component::drawable>();
    register_components<component::animated_drawable>();
    register_components<component::text>();

    add_system(
        [this, &controllables = this->get_components<component::controllable>(),
         &velocities = this->get_components<component::velocity>()](double delta
        ) { control_system(delta, *this, controllables); }
    );

    add_system([this, &positions = this->get_components<component::position>(),
                &velocities =
                    this->get_components<component::velocity>()](double delta) {
        position_system(delta, *this, positions, velocities);
    });

    add_system([this, &positions = this->get_components<component::position>(),
                &hurtboxes = this->get_components<component::hurtbox>(),
                &hitboxes =
                    this->get_components<component::hitbox>()](double delta) {
        collision_system(delta, *this, positions, hurtboxes, hitboxes);
    });

    add_system([this, &logics = this->get_components<component::logic>(
                      )](double delta) { logic_system(delta, *this, logics); });
}

/**
 * @brief Construct a new registry::registry object. This version doesn't use a
 * window and doesn't register the drawable system
 *
 */
registry::registry() : tmp(sf::RenderWindow()), window(tmp) {
    register_all_systems();
    window.close();
}

/**
 * @brief Construct a new registry::registry object. This version takes a
 * renderwindow as parameter and registers the drawable system
 *
 * @param _window
 */
registry::registry(sf::RenderWindow& _window) : window(_window) {
    register_all_systems();

    add_system([this, &window = this->window,
                &positions = this->get_components<component::position>(),
                &draws = this->get_components<component::drawable>(),
                &anim_draws =
                    this->get_components<component::animated_drawable>(),
                &texts =
                    this->get_components<component::text>()](double delta) {
        draw_system(delta, *this, window, positions, draws, anim_draws, texts);
    });
}

/**
 * @brief Add a system to the registry. The best way to add a system is by
 * passing a lambda capturing by reference all the needed components
 *
 * @param system a lambda capturing by reference all the needed components
 */
void registry::add_system(const function<void(double)>& system) {
    _systems.push_back(system);
}

/**
 * @brief This function runs all the systems registered in the registry
 *
 */
void registry::run_systems(double delta) {
    for (const auto& system : _systems) {
        system(delta);
    }
}
