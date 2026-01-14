/* ------------------------------------------------------------------------------------
 * *
 *                                                                                      *
 * EPITECH PROJECT - Tue, Sep, 2025 * Title           -
 * G-CPP-500-COT-5-1-rtype-8                                          *
 * Description     - * logic_functions *
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
 * @file logic_functions.cpp
 * @author Aurel DOSSOU KOHI
 * @brief This file contains definitions of the logic functions
 * @version 0.1
 * @date 2025-10-13
 *
 * @copyright Copyright (c) 2026 - Aurel DOSSOU KOHI
 *
 */

#include "logic_functions.hpp"

#include <cmath>

#include "../../rtype_gameInstance/include/gameInstance.hpp"
#include "Factory.hpp"
#include "entity.hpp"

using namespace component;

double distance(double x1, double y1, double x2, double y2) {
    return sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));
}

void player_logic(double delta, registry& reg, entity en) {
    position& pos = reg.get_components<position>()[(size_t)en].value();
    const controllable& control =
        reg.get_components<controllable>()[(size_t)en].value();
    velocity& vel = reg.get_components<velocity>()[(size_t)en].value();
    static double shoot_timer = 0;

    shoot_timer += delta;

    if (control.left) {
        vel.vx = -PLAYER_SPEED;
    } else if (control.right) {
        vel.vx = PLAYER_SPEED;
    } else {
        vel.vx = 0;
    }

    if (control.up) {
        vel.vy = -PLAYER_SPEED;
    } else if (control.down) {
        vel.vy = PLAYER_SPEED;
    } else {
        vel.vy = 0;
    }

    if (control.space && shoot_timer > PLAYER_SHOOT_COOLDOWN) {
        Factory fac(reg);

        shoot_timer = 0;
        entity missile = fac.make_player_missile();

        position& player_pos =
            reg.get_components<position>()[(size_t)en].value();
        position& missile_pos = reg.get_components<position>()[missile].value();
        missile_pos.x = player_pos.x;
        missile_pos.y = player_pos.y;
    }
}

void player_evil_logic(double delta, registry& reg, entity en) {
    position& pos = reg.get_components<position>()[(size_t)en].value();
    const controllable& control =
        reg.get_components<controllable>()[(size_t)en].value();
    velocity& vel = reg.get_components<velocity>()[(size_t)en].value();
    static double shoot_timer = 0;

    shoot_timer += delta;

    if (control.left) {
        vel.vx = -PLAYER_SPEED;
    } else if (control.right) {
        vel.vx = PLAYER_SPEED;
    } else {
        vel.vx = 0;
    }

    if (control.up) {
        vel.vy = -PLAYER_SPEED;
    } else if (control.down) {
        vel.vy = PLAYER_SPEED;
    } else {
        vel.vy = 0;
    }

    if (control.space && shoot_timer > PLAYER_SHOOT_COOLDOWN) {
        Factory fac(reg);

        shoot_timer = 0;
        entity missile = fac.make_evil_player_missile();

        position& player_pos =
            reg.get_components<position>()[(size_t)en].value();
        position& missile_pos = reg.get_components<position>()[missile].value();
        missile_pos.x = player_pos.x;
        missile_pos.y = player_pos.y;
    }
}

std::vector<int> get_players_ids(registry& reg) {
    std::vector<int> players_ids;
    for (int i = 0; i < reg.getEntityNum(); ++i) {
        try {
            name& name_ = reg.get_components<name>()[i].value();
            if (name_._name == "player1" || name_._name == "player2" || name_._name == "player3" || name_._name == "player4")
                players_ids.push_back(i);
        } catch (...) {
        }
    }
    return players_ids;
}

bool shoot_at_player(registry& reg, position enemy_pos, double attack_range) {

    double min_distance = 1e9;
    int target_id = -1;
    std::vector<int> player_ids = get_players_ids(reg);
    if (player_ids.size() == 0)
        return false;

    // Trouver le joueur le plus proche dans la portée
    for (auto& player_id : player_ids) {
        if (player_id == -1)
            continue;

        auto player_pos_opt = reg.get_components<position>()[player_id];
        if (!player_pos_opt.has_value())
            continue;

        position player_pos = player_pos_opt.value();
        double d = distance(player_pos.x, player_pos.y, enemy_pos.x, enemy_pos.y);

        if (d < attack_range && d < min_distance) {
            min_distance = d;
            target_id = player_id;
        }
    }

    // Aucun joueur dans la portée
    if (target_id == -1)
        return false;

    // Obtenir la position de la cible
    position target_pos = reg.get_components<position>()[target_id].value();

    // Créer et diriger le missile
    Factory fac(reg);
    entity missile = fac.make_enemy_missile();

    position& missile_pos = reg.get_components<position>()[missile].value();
    missile_pos.x = enemy_pos.x + 8;
    missile_pos.y = enemy_pos.y + 8;

    velocity& vel = reg.get_components<velocity>()[missile].value();
    vel.vx = target_pos.x - enemy_pos.x;
    vel.vy = target_pos.y - enemy_pos.y;

    return true;
}

void plane_logic(double delta, registry& reg, entity en) {
    hurtbox& hb = reg.get_components<hurtbox>()[en].value();

    if (hb.hurt) {
        Factory fac(reg);
        entity hit_effect = fac.make_hit_effect();
        position& pos = reg.get_components<position>()[en].value();
        position& hit_pos = reg.get_components<position>()[hit_effect].value();
        hit_pos.x = pos.x;
        hit_pos.y = pos.y;
    }

    if (hb.health <= 0) {
        Factory fac(reg);
        entity explosion = fac.make_explosion();
        position& pos = reg.get_components<position>()[en].value();
        position& explosion_pos =
            reg.get_components<position>()[explosion].value();
        explosion_pos.x = pos.x;
        explosion_pos.y = pos.y;

        reg.kill_entity(en);
    }
}


void red_trooper_logic(double delta, registry& reg, entity en) {
    static bool shot = false;
    static double shoot_timer = 0;
    static double t = 0.0;
    hurtbox& hb = reg.get_components<hurtbox>()[en].value();
    velocity& vel = reg.get_components<velocity>()[en].value();

    if (shot) {
        shoot_timer += delta;
        if (shoot_timer > ENEMY_SHOOT_COOLDOWN) {
            shoot_timer = 0;
            shot = false;
        }
    } else {
        shoot_timer = 0;
    }

    t += delta;
    vel.vx = -TROOPER_SPEED_X;
    vel.vy = sin(t * 2) * TROOPER_SPEED_Y;

    if (hb.hurt) {
        Factory fac(reg);
        entity hit_effect = fac.make_hit_effect();
        position& pos = reg.get_components<position>()[en].value();
        position& hit_pos = reg.get_components<position>()[hit_effect].value();
        hit_pos.x = pos.x;
        hit_pos.y = pos.y;
    }

    if (hb.health <= 0) {
        Factory fac(reg);
        entity explosion = fac.make_explosion();
        position& pos = reg.get_components<position>()[en].value();
        position& explosion_pos =
            reg.get_components<position>()[explosion].value();
        explosion_pos.x = pos.x;
        explosion_pos.y = pos.y;

        reg.kill_entity(en);
    }

    position& pos = reg.get_components<position>()[en].value();
    if (!shot)
        if (shoot_at_player(reg, pos, 200))
            shot = true;
}

void start_text_logic(double delta, registry& reg, entity en) {
    static double t = 0.0;

    t += delta;

    text& text_var = reg.get_components<text>()[en].value();
    text_var.text.setFillColor(sf::Color(255, 255, 255, abs(sin(t * 2) * 255)));
}

void walker_logic(double delta, registry& reg, entity en) {
    static bool walker_shot = false;
    static double walker_shoot_timer = 0;
    hurtbox& hb = reg.get_components<hurtbox>()[en].value();

    if (hb.health <= 0) {
        Factory fac(reg);
        entity explosion = fac.make_explosion();
        position& pos = reg.get_components<position>()[en].value();
        position& explosion_pos =
            reg.get_components<position>()[explosion].value();
        explosion_pos.x = pos.x;
        explosion_pos.y = pos.y;

        reg.kill_entity(en);
    }

    if (walker_shot) {
        walker_shoot_timer += delta;
        if (walker_shoot_timer > ENEMY_SHOOT_COOLDOWN) {
            walker_shoot_timer = 0;
            walker_shot = false;
        }
    } else {
        walker_shoot_timer = 0;
    }

    position& pos = reg.get_components<position>()[en].value();
    if (!walker_shot) {
        if (shoot_at_player(reg, pos, 200))
            walker_shot = true;
    }

    if (hb.hurt) {
        Factory fac(reg);
        entity hit_effect = fac.make_hit_effect();
        position& pos = reg.get_components<position>()[en].value();
        position& hit_pos = reg.get_components<position>()[hit_effect].value();
        hit_pos.x = pos.x;
        hit_pos.y = pos.y;
    }
}

void fade_in_rect_logic(double delta, registry& reg, entity entity) {
    Factory fac(reg);
    static double t = 0.0;
    static double instancited = false;
    drawable& dr = reg.get_components<drawable>()[entity].value();

    t += delta;
    dr.sprite.setColor(sf::Color(0, 0, 0, (int)round(t * 128)));
    if (dr.sprite.getColor().a >= 255) {
        reg.kill_entity(entity);
    }
    if (dr.sprite.getColor().a >= 220 && !instancited) {
        instancited = true;
        fac.make_fade_out_rect();
    }
}

void fade_out_rect_logic(double delta, registry& reg, entity entity) {
    static double t = 0.0;
    drawable& dr = reg.get_components<drawable>()[entity].value();

    t += delta;
    dr.sprite.setColor(sf::Color(0, 0, 0, 255 - (int)round(t * 128)));
    if (dr.sprite.getColor().a <= 0) {
        reg.kill_entity(entity);
    }
}

void boss_logic(double delta, registry& reg, entity en) {
    static double t = 0.0;
    static double t1 = 0.0;
    hurtbox& hb = reg.get_components<hurtbox>()[en].value();
    velocity& vel = reg.get_components<velocity>()[en].value();
    position& pos = reg.get_components<position>()[en].value();

    t1 += delta;

    vel.vy = sin(t1 * 2) * BOSS_SPEED;

    if (pos.x > 500) {
        vel.vx = -BOSS_SPEED;
    } else {
        vel.vx = 0;
    }

    if (t < BOSS_SHOOT_COOLDOWN) {
        t += delta;
    } else {
        t = 0;
        Factory fac(reg);
        entity missile1 = fac.make_enemy_missile();
        entity missile2 = fac.make_enemy_missile();
        entity missile3 = fac.make_enemy_missile();

        position& pos = reg.get_components<position>()[en].value();
        position& pos1 = reg.get_components<position>()[missile1].value();
        position& pos2 = reg.get_components<position>()[missile2].value();
        position& pos3 = reg.get_components<position>()[missile3].value();

        velocity& vel1 = reg.get_components<velocity>()[missile1].value();
        velocity& vel2 = reg.get_components<velocity>()[missile2].value();
        velocity& vel3 = reg.get_components<velocity>()[missile3].value();

        pos1.x = pos.x + 30;
        pos2.x = pos.x + 30;
        pos3.x = pos.x + 30;
        pos1.y = pos.y + 15;
        pos2.y = pos.y + 15;
        pos3.y = pos.y + 15;

        vel1.vx = -BOSS_MISSILE_SPEED;
        vel1.vy = -BOSS_MISSILE_SPEED / 2;
        vel2.vx = -BOSS_MISSILE_SPEED;
        vel2.vy = BOSS_MISSILE_SPEED / 2;
        vel3.vx = -BOSS_MISSILE_SPEED;
        vel3.vy = 0;
    }

    if (hb.hurt) {
        Factory fac(reg);
        entity hit_effect = fac.make_hit_effect();
        position& hit_pos = reg.get_components<position>()[hit_effect].value();
        hit_pos.x = pos.x;
        hit_pos.y = pos.y;
    }

}

void gtrooper_logic(double delta, registry& reg, entity en) {
    return;
}

// ------------------------ SPACE ENEMY -----------------------

void spacenemy_logic(double delta, registry& reg, entity en) {
    static double t = 0.0;
    std::vector<int> player_ids = get_players_ids(reg);
    static size_t target_player_id = -1;
    if (player_ids.size() == 0) {
        return;
    }
    static double retarget_timer = 0.0;
    const double RETARGET_INTERVAL = 3.0;
    const float TARGET_X = 500.0f;
    const float X_THRESHOLD = 10.0f;
    const float ALIGN_THRESHOLD = 20.0f;

    hurtbox& hb = reg.get_components<hurtbox>()[en].value();
    velocity& vel = reg.get_components<velocity>()[en].value();
    position& pos = reg.get_components<position>()[en].value();

    // Déplacement vers TARGET_X
    if (pos.x > TARGET_X + X_THRESHOLD) {
        vel.vx = -BOSS_SPEED;
        vel.vy = 0;
        return;
    }
    vel.vx = 0;

    // Choix aléatoire de cible tous les RETARGET_INTERVAL secondes
    retarget_timer += delta;
    if (retarget_timer >= RETARGET_INTERVAL) {
        std::vector<int> valid_players;
        auto& positions = reg.get_components<position>();

        for (auto& player_id : player_ids) {
            if (player_id != -1 && positions[player_id].has_value())
                valid_players.push_back(player_id);
        }

        if (!valid_players.empty()) {
            target_player_id = valid_players[rand() % valid_players.size()];
        }
        retarget_timer = 0.0;
    }

    auto& positions = reg.get_components<position>();
    if (!positions[target_player_id].has_value()) {
        vel.vy = 0;
        return;
    }

    position& player_pos = positions[target_player_id].value();
    float diff_y = player_pos.y - pos.y;

    vel.vy = (fabs(diff_y) > ALIGN_THRESHOLD) ? ((diff_y > 0) ? BOSS_SPEED : -BOSS_SPEED) : 0;

    bool is_aligned = (fabs(diff_y) <= ALIGN_THRESHOLD);

    // Tir automatique quand aligné
    t += delta;
    if (is_aligned && t >= BOSS_SHOOT_COOLDOWN) {
        t = 0;
        Factory fac(reg);
        entity missile1 = fac.make_big_missile();
        position& missile_pos1 = reg.get_components<position>()[missile1].value();
        missile_pos1.x = pos.x + 30;
        missile_pos1.y = pos.y + 15;
    }

    // Effet de hit
    if (hb.hurt) {
        Factory fac(reg);
        entity hit_effect = fac.make_hit_effect();
        position& hit_pos = reg.get_components<position>()[hit_effect].value();
        hit_pos.x = pos.x;
        hit_pos.y = pos.y;
    }

    // Mort du boss
    if (hb.health <= 0) {
        Factory fac(reg);
        entity explosion = fac.make_explosion();
        entity force = fac.make_force();
        position& explosion_pos = reg.get_components<position>()[explosion].value();
        position& force_pos = reg.get_components<position>()[force].value();
        explosion_pos.x = pos.x;
        explosion_pos.y = pos.y;
        force_pos.x = pos.x;
        force_pos.y = pos.y;
        reg.kill_entity(en);
    }
}

void force_logic(double delta, registry& reg, entity en)
{
    static int attached_to = -1;
    static double shoot_cooldown = 0.0;
    hurtbox& hb = reg.get_components<hurtbox>()[en].value();

    // Si le force est mort, il se rattache au joueur le plus proche
    if (hb.health <= 0) {
        position& pos = reg.get_components<position>()[en].value();

        std::vector<int> player_ids = get_players_ids(reg);
        double min_distance = 1e9;
        attached_to = -1;

        for (auto& player_id : player_ids) {
            if (player_id == -1)
                continue;

            auto player_pos_opt = reg.get_components<position>()[player_id];
            if (!player_pos_opt.has_value())
                continue;

            position player_pos = player_pos_opt.value();
            double d = distance(player_pos.x, player_pos.y, pos.x, pos.y);
            if (d < min_distance) {
                min_distance = d;
                attached_to = player_id;
            }
        }

        if (attached_to == -1)
            return;

        hurtbox& new_hb = reg.add_component<component::hurtbox>(en, component::hurtbox());
        new_hb.health = 20;
        new_hb.group = 1;
        new_hb.width = 24;
        new_hb.height = 19;
    }

    if (attached_to != -1) {
        position& pos = reg.get_components<position>()[en].value();
        position& player_pos = reg.get_components<position>()[attached_to].value();

        // Positionner le force à côté du joueur
        pos.x = player_pos.x + 50;
        pos.y = player_pos.y;

        // Tir automatique
        shoot_cooldown += delta;
        if (shoot_cooldown >= FORCE_SHOOT_COOLDOWN) {
            Factory fac(reg);

            auto spawn_missile = [&](double offset_y) {
                entity missile = fac.make_player_missile();
                position& m_pos = reg.get_components<position>()[missile].value();
                m_pos.x = player_pos.x + 55;
                m_pos.y = player_pos.y + offset_y;
            };

            spawn_missile(-15);
            spawn_missile(15);
            shoot_cooldown = 0;
        }
    }

    // Effet de hit
    if (hb.hurt && attached_to != -1) {
        Factory fac(reg);
        entity hit_effect = fac.make_hit_effect();
        position& pos = reg.get_components<position>()[en].value();
        position& hit_pos = reg.get_components<position>()[hit_effect].value();
        hit_pos.x = pos.x;
        hit_pos.y = pos.y;
    }

    if (hb.health <= 0) {
        Factory fac(reg);
        entity explosion = fac.make_explosion();
        position& pos = reg.get_components<position>()[en].value();
        position& explosion_pos =
            reg.get_components<position>()[explosion].value();
        explosion_pos.x = pos.x;
        explosion_pos.y = pos.y;

        reg.kill_entity(en);
    }
}

void tourelles_logic(double delta, registry& reg, entity en) {
    static bool tourelles_shot = false;
    static double tourelles_shoot_timer = 0;
    hurtbox& hb = reg.get_components<hurtbox>()[en].value();

    if (hb.health <= 0) {
        Factory fac(reg);
        entity explosion = fac.make_explosion();
        position& pos = reg.get_components<position>()[en].value();
        position& explosion_pos =
            reg.get_components<position>()[explosion].value();
        explosion_pos.x = pos.x;
        explosion_pos.y = pos.y;

        reg.kill_entity(en);
    }

    if (tourelles_shot) {
        tourelles_shoot_timer += delta;
        if (tourelles_shoot_timer > ENEMY_SHOOT_COOLDOWN) {
            tourelles_shoot_timer = 0;
            tourelles_shot = false;
        }
    } else {
        tourelles_shoot_timer = 0;
    }

    position& pos = reg.get_components<position>()[en].value();
    if (!tourelles_shot) {
        if (shoot_at_player(reg, pos, 200))
            tourelles_shot = true;
    }

    if (hb.hurt) {
        Factory fac(reg);
        entity hit_effect = fac.make_hit_effect();
        position& pos = reg.get_components<position>()[en].value();
        position& hit_pos = reg.get_components<position>()[hit_effect].value();
        hit_pos.x = pos.x;
        hit_pos.y = pos.y;
    }
}

void small_shooter_logic(double delta, registry& reg, entity en) {
    static double t = 0.0;
    static double t1 = 0.0;
    static bool ready = false;
    hurtbox& hb = reg.get_components<hurtbox>()[en].value();
    velocity& vel = reg.get_components<velocity>()[en].value();
    position& pos = reg.get_components<position>()[en].value();

    t1 += delta;

    if (pos.x <= 400)
        ready = true;

    if (!ready) {
        vel.vx = -BOSS_SPEED;
        return;
    }

    vel.vy = sin(t1 * 2) * BOSS_SPEED;
    vel.vx = sin(t1 * 2) * BOSS_SPEED;


    if (t < BOSS_SHOOT_COOLDOWN) {
        t += delta;
    } else {
        t = 0;
        Factory fac(reg);
        entity missile1 = fac.make_enemy_missile();
        entity missile2 = fac.make_enemy_missile();
        entity missile3 = fac.make_enemy_missile();

        position& pos = reg.get_components<position>()[en].value();
        position& pos1 = reg.get_components<position>()[missile1].value();
        position& pos2 = reg.get_components<position>()[missile2].value();
        position& pos3 = reg.get_components<position>()[missile3].value();

        velocity& vel1 = reg.get_components<velocity>()[missile1].value();
        velocity& vel2 = reg.get_components<velocity>()[missile2].value();
        velocity& vel3 = reg.get_components<velocity>()[missile3].value();

        pos1.x = pos.x + 30;
        pos2.x = pos.x + 30;
        pos3.x = pos.x + 30;
        pos1.y = pos.y + 15;
        pos2.y = pos.y + 15;
        pos3.y = pos.y + 15;

        vel1.vx = -BOSS_MISSILE_SPEED;
        vel1.vy = -BOSS_MISSILE_SPEED / 2;
        vel2.vx = -BOSS_MISSILE_SPEED;
        vel2.vy = BOSS_MISSILE_SPEED / 2;
        vel3.vx = -BOSS_MISSILE_SPEED;
        vel3.vy = 0;
    }

    if (hb.hurt) {
        Factory fac(reg);
        entity hit_effect = fac.make_hit_effect();
        position& hit_pos = reg.get_components<position>()[hit_effect].value();
        hit_pos.x = pos.x;
        hit_pos.y = pos.y;
    }

}

void big_shooter_logic(double delta, registry& reg, entity en) {
    static double t = 0.0;
    static double t1 = 0.0;
    static bool ready = false;
    hurtbox& hb = reg.get_components<hurtbox>()[en].value();
    velocity& vel = reg.get_components<velocity>()[en].value();
    position& pos = reg.get_components<position>()[en].value();

    t1 += delta;

    if (pos.x <= 400)
        ready = true;

    if (!ready) {
        vel.vx = -BOSS_SPEED;
        return;
    }


    vel.vy = sin(t1 * 2) * BOSS_SPEED;
    vel.vx = sin(t1 * 2) * BOSS_SPEED;


    if (t < BOSS_SHOOT_COOLDOWN) {
        t += delta;
    } else {
        t = 0;
        Factory fac(reg);
        entity missile1 = fac.make_big_missile();

        position& pos = reg.get_components<position>()[en].value();
        position& pos1 = reg.get_components<position>()[missile1].value();

        velocity& vel1 = reg.get_components<velocity>()[missile1].value();

        pos1.x = pos.x + 30;
        pos1.y = pos.y + 15;

        vel1.vx = -BOSS_MISSILE_SPEED;
        vel1.vy = 0.0;
    }

    if (hb.hurt) {
        Factory fac(reg);
        entity hit_effect = fac.make_hit_effect();
        position& hit_pos = reg.get_components<position>()[hit_effect].value();
        hit_pos.x = pos.x;
        hit_pos.y = pos.y;
    }

}

void final_boss_logic(double delta, registry& reg, entity en) {
    static double t = 0.0;
    static double t1 = 0.0;
    static double t2 = 0.0;
    static bool ready = false;
    hurtbox& hb = reg.get_components<hurtbox>()[en].value();
    velocity& vel = reg.get_components<velocity>()[en].value();
    position& pos = reg.get_components<position>()[en].value();

    t1 += delta;

    if (pos.x <= 400)
        ready = true;

    if (!ready) {
        vel.vx = -BOSS_SPEED;
        return;
    }

    vel.vy = sin(t1 * 2) * BOSS_SPEED;
    vel.vx = 0;

    if (t < BOSS_SHOOT_COOLDOWN) {
        t += delta;
    } else {
        t = 0;
        Factory fac(reg);
        entity missile1 = fac.make_big_missile();

        position& pos = reg.get_components<position>()[en].value();
        position& pos1 = reg.get_components<position>()[missile1].value();

        velocity& vel1 = reg.get_components<velocity>()[missile1].value();

        pos1.x = pos.x + 30;
        pos1.y = pos.y + 15;

        vel1.vx = -BOSS_MISSILE_SPEED;
        vel1.vy = 0.0;
    }
    if (t2 < BOSS_SPAWN_COOLDOWN) {
        t2 += delta;
    } else {
        t2 = 0;
        Factory fac(reg);
        entity plane = fac.make_plane();

        position& pos = reg.get_components<position>()[en].value();
        position& pos1 = reg.get_components<position>()[plane].value();

        pos1.x = pos.x + 30;
        pos1.y = pos.y + 15;
    }

    if (hb.hurt) {
        Factory fac(reg);
        entity hit_effect = fac.make_hit_effect();
        position& hit_pos = reg.get_components<position>()[hit_effect].value();
        hit_pos.x = pos.x;
        hit_pos.y = pos.y;
    }

}
