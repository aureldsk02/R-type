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
 *             ███████╗██████╗ ██╗████████╗███████╗ ██████╗██╗  ██╗ *
 *             ██╔════╝██╔══██╗██║╚══██╔══╝██╔════╝██╔════╝██║  ██║ * █████╗
 * ██████╔╝██║   ██║   █████╗  ██║     ███████║                     * ██╔══╝
 * ██╔═══╝ ██║   ██║   ██╔══╝  ██║     ██╔══██║                     *
 *             ███████╗██║     ██║   ██║   ███████╗╚██████╗██║  ██║ *
 *             ╚══════╝╚═╝     ╚═╝   ╚═╝   ╚══════╝ ╚═════╝╚═╝  ╚═╝ *
 *                                                                                      *
 * ------------------------------------------------------------------------------------
 */

/**
 * @file registry_entities.cpp
 * @author Aurel DOSSOU KOHI
 * @brief This file contains the definition of the registry functions that
 * handle the entities
 * @version 0.1
 * @date 2025-09-23
 *
 * @copyright Copyright (c) 2026 - Aurel DOSSOU KOHI
 *
 */

#include "../include/registry.hpp"

/**
 * @brief This function creates a new entity
 *
 * @return entity The id of the newly created entity
 */
entity registry::spawn_entity() {
    // If there's a dead entity id, just re-use it
    if (dead_entities.size() > 0) {
        entity last_id = dead_entities[dead_entities.size() - 1];
        dead_entities.pop_back();
        return last_id;
    } else {
        // Else increment the number of ids
        entity_num += 1;
        return entity(entity_num - 1);
    }
}

/**
 * @brief This function deletes an entity and all the components associated with
 * itbased on its id
 *
 * @param e The id of the entity to be deleted
 */
void registry::kill_entity(const entity& e) {
    // Check if the entity is really registered
    if (find(dead_entities.begin(), dead_entities.end(), e) !=
            dead_entities.end() ||
        (size_t)e >= entity_num)
        throw NonExistentEntityID();

    // Add the entity to the dead_entities
    dead_entities.push_back(e);

    // Call each eraser function to delete all related components
    for (const auto& [key, value] : _erase_functions) {
        value(e);
    }
}
