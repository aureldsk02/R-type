/* ------------------------------------------------------------------------------------ *
 *                                                                                      *
 * EPITECH PROJECT - Wed, Sep, 2025                                                     *
 * Title           - r-type_bs                                                          *
 * Description     -                                                                    *
 *     registry                                                                         *
 *                                                                                      *
 * ------------------------------------------------------------------------------------ *
 *                                                                                      *
 *             ███████╗██████╗ ██╗████████╗███████╗ ██████╗██╗  ██╗                     *
 *             ██╔════╝██╔══██╗██║╚══██╔══╝██╔════╝██╔════╝██║  ██║                     *
 *             █████╗  ██████╔╝██║   ██║   █████╗  ██║     ███████║                     *
 *             ██╔══╝  ██╔═══╝ ██║   ██║   ██╔══╝  ██║     ██╔══██║                     *
 *             ███████╗██║     ██║   ██║   ███████╗╚██████╗██║  ██║                     *
 *             ╚══════╝╚═╝     ╚═╝   ╚═╝   ╚══════╝ ╚═════╝╚═╝  ╚═╝                     *
 *                                                                                      *
 * ------------------------------------------------------------------------------------ */

#ifndef INCLUDED_REGISTRY_SINGLE_COMP_TPP
#define INCLUDED_REGISTRY_SINGLE_COMP_TPP

/**
 * @file registry_single_comp.tpp
 * @author Farouk OKANLA
 * @brief This file contains the implementation of the registry functions to handle single components operations
 * @version 0.1
 * @date 2025-10-13
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#include "../include/registry.hpp"
#include "../include/components.hpp"

/**
 * @brief This function removes a component from an entity
 * 
 * @tparam Component the type of the component to be removed
 * @param from The entity from which the component will be removed
 */
template<typename Component>
void registry::remove_component(entity const &from)
{
    // Check if the type is registered
    if (_components_arrays.find(typeid(Component)) == _components_arrays.end()) {
        throw NonExistentComponentType();
    }
    // Check if the entity is really registered
    if (find(dead_entities.begin(), dead_entities.end(), from) != dead_entities.end()
        || (size_t)from >= entity_num)
        throw NonExistentEntityID();

    // Call the eraser function corresponding to the type
    _erase_functions[typeid(Component)](from);
}

/**
 * @brief This function adds a component to an entity
 * 
 * @tparam Component the type of the component
 * @param to The id of the entity the component will be associated with
 * @param c The component value
 * @return Component& A reference to the newly created component
 */
template<typename Component>
Component &registry::add_component(entity const &to, Component &&c)
{
    // Check if the type is registered
    if (_components_arrays.find(typeid(Component)) == _components_arrays.end()) {
        throw NonExistentComponentType();
    }
    // Check if the entity is really registered
    if (find(dead_entities.begin(), dead_entities.end(), to) != dead_entities.end()
        || (size_t)to >= entity_num)
        throw NonExistentEntityID();

    // Get the table corresponding to the component type
    vector<optional<Component>> &table = get_components<Component>();

    // Check if the table is large enough
    if (table.size() < (size_t)to + 1) {
        // If not, add enough space to reach the entity's id
        table.resize((size_t)to + 1);
    }
    // Move the component into the entity's place
    table[(size_t)to] = std::move(c);

    return *table[(size_t)to];
}

#endif
