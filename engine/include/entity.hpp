/* ------------------------------------------------------------------------------------
 * *
 *                                                                                      *
 * EPITECH PROJECT - Wed, Sep, 2025 * Title           - r-type_bs * Description
 * -                                                                    * entity
 * *
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

#ifndef ENTITY_HPP
#define ENTITY_HPP

/**
 * @file entity.hpp
 * @author Aurel DOSSOU KOHI
 * @brief This file contains the definition of the entity class
 * @version 0.1
 * @date 2025-09-23
 *
 * @copyright Copyright (c) 2026 - Aurel DOSSOU KOHI
 *
 */
#include <iostream>

/**
 * @brief The entity class, defined by an id to which components will be
 * associated.
 *
 */
class entity {
  private:
    size_t id;

  public:
    /**
     * @brief Construct a new entity object explicitly from a size_t
     *
     * @param new_id
     */
    explicit entity(size_t new_id) {
        id = new_id;
    }

    entity() {
        id = -1;
    }

    /**
     * @brief Get the Id of the entity
     *
     * @return size_t
     */
    size_t getId(void) const {
        return id;
    }

    /**
     * @brief Allows implicit conversion to size_t
     *
     */
    operator size_t(void) const {
        return id;
    }
};

#endif
