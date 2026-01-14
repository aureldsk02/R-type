/* ------------------------------------------------------------------------------------
 * *
 *                                                                                      *
 * EPITECH PROJECT - Fri, Sep, 2025 * Title           - r-type_bs * Description
 * -                                                                    *
 *     systems *
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
 * @file systems.cpp
 * @author Aurel DOSSOU KOHI
 * @brief This file contains the definition of the different systems of the
 * registry
 * @version 0.1
 * @date 2025-09-23
 *
 * @copyright Copyright (c) 2026 - Aurel DOSSOU KOHI
 *
 */

#include "../include/components.hpp"
#include "../include/registry.hpp"

/**
 * @brief The position system. This system updates the position of each entity
 *  based on its velocity
 * @param delta The amount of time elapsed since the last frame
 * @param reg A reference to the registry
 * @param positions The table of position components
 * @param velocities The table of velocity components
 */
void position_system(
    double delta, registry& reg,
    std::vector<optional<component::position>>& positions,
    std::vector<optional<component::velocity>>& velocities
) {
    if (!reg.getMovement())
        return;

    for (size_t i = 0; i < reg.getEntityNum(); ++i) {
        try {
            auto& pos = positions.at(i);
            auto& vel = velocities.at(i);

            if (pos && vel) {
                pos.value().setPosition(
                    vel.value().vx * delta, vel.value().vy * delta
                );
            }
        } catch (...) {
        }
    }
}

/**
 * @brief The draw system. This system draws to screen all the entities that
 * have a drawable, animated drawable or text component AND a position
 * component.
 *
 * @param delta The amount of time elapsed since the last frame
 * @param reg A reference to the registry
 * @param window A reference to render window
 * @param positions The table of position components
 * @param draws The table of drawable components
 * @param anim_draws The table of animated drawable components
 * @param texts The table of text components
 */
void draw_system(
    double delta, registry& reg, sf::RenderWindow& window,
    std::vector<optional<component::position>>& positions,
    std::vector<optional<component::drawable>>& draws,
    std::vector<optional<component::animated_drawable>>& anim_draws,
    std::vector<optional<component::text>>& texts
) {
    if (reg.getRendering() == false) {
        return;
    }

    window.clear(sf::Color::Black);

    for (size_t i = 0; i < reg.getEntityNum(); ++i) {
        try {
            auto& pos = positions.at(i);
            auto& draw = draws.at(i);

            if (pos && draw) {
                draw.value().sprite.setPosition(pos.value().x, pos.value().y);
                window.draw(draw.value().sprite);
            }
        } catch (...) {
        }
        try {
            auto& pos = positions.at(i);
            auto& anim_draw = anim_draws.at(i);

            if (pos && anim_draw) {
                anim_draw.value().animate(delta);
                if (anim_draw.value().one_shot && anim_draw.value().done_once) {
                    reg.kill_entity(entity(i));
                    continue;
                }
                anim_draw.value().sprite.setPosition(
                    pos.value().x, pos.value().y
                );
                window.draw(anim_draw.value().sprite);
            }
        } catch (...) {
        }
        try {
            auto& pos = positions.at(i);
            auto& text = texts.at(i);

            if (text && pos) {
                text.value().text.setPosition(pos.value().x, pos.value().y);
                window.draw(text.value().text);
            }
        } catch (...) {
        }
    }
    window.display();
}

/**
 * @brief The control system. This system updates the controllable components by
 * listening for inputs and setting the corresponding booleans in the component
 * to true if the key is pressed and false otherwise
 *
 * @param delta The amount of time elapsed since the last frame
 * @param reg A reference to the registry
 * @param controls The table of controllable components
 */
void control_system(
    double delta, registry& reg,
    std::vector<optional<component::controllable>>& controls
) {
    if (reg.getControl() == false) {
        return;
    }

    for (size_t i = 0; i < reg.getEntityNum(); ++i) {
        try {
            if (i >= controls.size() || !controls.at(i).has_value())
                continue;
            auto& control = controls.at(i).value();

            control.getKeyboardInput();
        } catch (...) {
        }
    }
}

/**
 * @brief The collision system. This system goes through each hurtbox and hitbox
 * component and checks if they are colliding
 *
 * @param delta The amount of time elapsed since the last frame
 * @param reg A reference to the registry
 * @param positions The table of position components
 * @param hurtboxes The table of hurtbox components
 * @param hitboxes The table of hitbox components
 */
void collision_system(
    double delta, registry& reg,
    std::vector<optional<component::position>>& positions,
    std::vector<optional<component::hurtbox>>& hurtboxes,
    std::vector<optional<component::hitbox>>& hitboxes
) {
    if (reg.getPhysics() == false) {
        return;
    }

    for (size_t i = 0; i < reg.getEntityNum(); ++i) {
        try {
            // Go through each hurtbox and their positions
            auto& pos = positions.at(i);
            auto& hurtbox = hurtboxes.at(i);

            if (pos && hurtbox) {
                hurtbox.value().hurt = false;
                for (size_t j = 0; j < reg.getEntityNum(); ++j) {
                    // Go through each hitbox and their positions
                    auto& hit_pos = positions.at(j);
                    auto& hitbox = hitboxes.at(j);

                    if (hit_pos && hitbox) {
                        // Create the hitbox and hurtbox rects with their
                        // dimensions
                        sf::IntRect hurt_rect = sf::IntRect(
                            sf::Vector2i(
                                (int)(pos.value().x), (int)(pos.value().y)
                            ),
                            sf::Vector2i(
                                hurtbox.value().width, hurtbox.value().height
                            )
                        );
                        sf::IntRect hit_rect = sf::IntRect(
                            sf::Vector2i(
                                (int)(hit_pos.value().x),
                                (int)(hit_pos.value().y)
                            ),
                            sf::Vector2i(
                                hitbox.value().width, hitbox.value().height
                            )
                        );

                        if (hurt_rect.intersects(hit_rect) &&
                            hurtbox.value().group ==
                                hitbox.value().targeted_group) {
                            hurtbox.value().hurt = true;
                            std::cout << "COLLISION " << i << " " << j << "\n";
                            hurtbox.value().health -= hitbox.value().damage;

                            if (hitbox.value().one_shot) {
                                reg.kill_entity(entity(j));
                            }
                        }
                    }
                }
            }
        } catch (...) {
        }
    }
}

/**
 * @brief The logic system goes through each entity with a logic component and
 * executes its logic function
 * @param delta The amount of time elapsed since the last frame
 * @param reg A reference to the registry
 * @param logics The table of logic components
 */
void logic_system(
    double delta, registry& reg, std::vector<optional<component::logic>>& logics
) {
    if (reg.getLogic() == false) {
        return;
    }

    for (size_t i = 0; i < reg.getEntityNum(); ++i) {
        try {
            auto& logic = logics.at(i);

            if (logic) {
                logic.value().logic_function(delta, reg, entity(i));
            }
        } catch (...) {
        }
    }
}
