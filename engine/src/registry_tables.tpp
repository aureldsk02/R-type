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

#ifndef INCLUDED_REGISTRY_TABLES_TPP
#define INCLUDED_REGISTRY_TABLES_TPP

/**
 * @file registry_tables.tpp
 * @author Farouk OKANLA
 * @brief This file contains the implementation of the registry functions to handle components tables
 * @version 0.1
 * @date 2025-10-13
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#include "../include/registry.hpp"

template<typename Component>
/**
 * @brief Registers a table of a new type of components
 * 
 * @tparam Component The new type of component
 * @return vector<optional<Component>>& a reference to the newly added table
 */
vector<optional<Component>> &registry::register_components()
{
    // get the component type
    type_index new_type = typeid(Component);

    // Check if the corresponding type table is already stored or not
    if (_components_arrays.find(new_type) == _components_arrays.end()) {
        // If not found, create the component table
        _components_arrays.emplace(
            new_type,
            std::make_any<std::vector<std::optional<Component>>>()
        );
    }

    // Get the final table
    auto &table = std::any_cast<std::vector<std::optional<Component>>&>(_components_arrays[new_type]);

    // Create a lambda that will erase this type of component
    // for a given entity
    auto delete_func = [&table] (const entity &e) {
        if (e.getId() < table.size())
            table[e].reset();
    };

    // Add the lambda to the erasing functions container
    _erase_functions[new_type] = delete_func;

    // Return the reference to the final table (newly created or not)
    return table;
}

template<typename Component>
/**
 * @brief Get the table of a specific type of component
 * 
 * @tparam Component The type of the component table
 * @return vector<optional<Component>>& a reference to the table
 */
vector<optional<Component>> &registry::get_components()
{
    // Check if the type is registered
    if (_components_arrays.find(typeid(Component)) == _components_arrays.end()) {
        throw NonExistentComponentType();
    }
    // Get the corresponding table as an any
    any &table = _components_arrays[typeid(Component)];

    // Casting the any into its real type
    return any_cast<vector<optional<Component>>&>(table);
}

/**
 * @brief Get the table of a specific type of component (Const version)
 * 
 * @tparam Component The type of the component table
 * @return vector<optional<Component>>& a reference to the table
 *
 */
template<typename Component>
vector<optional<Component>> const &registry::get_components() const
{
    // Check if the type is registered
    if (_components_arrays.find(typeid(Component)) == _components_arrays.end()) {
        throw NonExistentComponentType();
    }
    // Get the corresponding table as an any
    const any &table = _components_arrays.at(typeid(Component));

    // Casting the any into its real type
    return any_cast<vector<optional<Component>>&>(table);
}

#endif