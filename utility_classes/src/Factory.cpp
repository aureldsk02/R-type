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
 *         ░        ░       ░░        ░        ░        ░░      ░░  ░░░░  ░ * ▒
 * ▒▒▒▒▒▒▒  ▒▒▒▒  ▒▒▒▒  ▒▒▒▒▒▒▒  ▒▒▒▒  ▒▒▒▒▒▒▒  ▒▒▒▒  ▒  ▒▒▒▒  ▒             *
 *         ▓      ▓▓▓       ▓▓▓▓▓  ▓▓▓▓▓▓▓  ▓▓▓▓      ▓▓▓  ▓▓▓▓▓▓▓        ▓ * █
 * ███████  ██████████  ███████  ████  ███████  ████  █  ████  █             *
 *         █        █  ███████        ████  ████        ██      ██  ████  █ *
 *                                                                                      *
 * ------------------------------------------------------------------------------------
 */

/**
 * @file Factory.cpp
 * @author Aurel DOSSOU KOHI
 * @brief This file contains the definition of the Factory class
 * @version 0.1
 * @date 2025-10-13
 *
 * @copyright Copyright (c) 2026 - Aurel DOSSOU KOHI
 *
 */

#include "../include/Factory.hpp"

#include "../../rtype_gameInstance/include/gameInstance.hpp"
#include "logic_functions.hpp"

int unique_ids = 0;

#define PLAYER_MISSILE_SPEED 500.0
#define ENEMY_MISSILE_SPEED 5
#define BIG_MISSILE_SPEED 400
#define BACKGROUND_SPEED 50
#define WALKER_SPEED 120
#define PLANE_SPEED 300

Factory::Factory(registry& _reg) : reg(_reg) {}

entity Factory:: make_entity(const std::string& type) {
    if (type == "player1")
        return make_player1();
    else if (type == "player2")
        return make_player2();
    else if (type == "player3")
        return make_player3();
    else if (type == "player4")
        return make_player4();
    else if (type == "red_trooper")
        return make_red_trooper();
    else if (type == "plane")
        return make_plane();
    else if (type == "walker")
        return make_walker();
    else if (type == "player_missile")
        return make_player_missile();
    else if (type == "enemy_missile")
        return make_enemy_missile();
    else if (type == "floor")
        return make_floor();
    else if (type == "ceiling")
        return make_ceiling();
    else if (type == "hit_effect")
        return make_hit_effect();
    else if (type == "explosion")
        return make_explosion();
    else if (type == "boss")
        return make_boss();
    else if (type == "force")
        return make_force();
    else if (type == "space_enemy")
        return make_space_enemy();
    else if (type == "tourelles")
        return make_tourelles();
    else if (type == "big_missile")
        return make_big_missile();
    else if (type == "final_boss")
        return make_final_boss();
    else if (type == "small_shooter")
        return make_small_shooter();
    else if (type == "big_shooter")
        return make_big_shooter();
    else if (type == "player2_flipped")
        return make_player2_flipped();
    else if (type == "evil_player_missile")
        return make_evil_player_missile();
    return entity(-1);
}

entity Factory::make_player1() {
    entity player_id = reg.spawn_entity();

    auto& player_sprite = reg.add_component<component::drawable>(
        player_id, component::drawable()
    );
    player_sprite.setTextureFromName("player1");

    reg.add_component<component::position>(player_id, {0, 0});
    reg.add_component<component::velocity>(player_id, {0, 0});
    reg.add_component<component::controllable>(
        player_id, component::controllable()
    );
    reg.add_component<component::logic>(
        player_id, component::logic{player_logic}
    );

    auto& player_hurtbox =
        reg.add_component<component::hurtbox>(player_id, component::hurtbox());
    player_hurtbox.group = 1;
    player_hurtbox.health = 1;
    player_hurtbox.width = 32;
    player_hurtbox.height = 16;

    auto& player_hitbox =
        reg.add_component<component::hitbox>(player_id, component::hitbox());
    player_hitbox.targeted_group = 3;
    player_hitbox.damage = 100;
    player_hitbox.width = 32;
    player_hitbox.height = 16;

    auto& entity_name =
        reg.add_component<component::name>(player_id, component::name());
    entity_name._name = "player1";

    reg.add_component<component::unique_id>(
        player_id, (component::unique_id)unique_ids
    );
    unique_ids++;
    return player_id;
}

entity Factory::make_player2() {
    entity player_id = reg.spawn_entity();

    auto& player_sprite = reg.add_component<component::drawable>(
        player_id, component::drawable()
    );
    player_sprite.setTextureFromName("player2");

    reg.add_component<component::position>(player_id, {0, 0});
    reg.add_component<component::velocity>(player_id, {0, 0});
    reg.add_component<component::controllable>(
        player_id, component::controllable()
    );
    reg.add_component<component::logic>(
        player_id, component::logic{player_logic}
    );

    auto& player_hurtbox =
        reg.add_component<component::hurtbox>(player_id, component::hurtbox());
    player_hurtbox.group = 1;
    player_hurtbox.health = 1;
    player_hurtbox.width = 32;
    player_hurtbox.height = 16;

    auto& entity_name =
        reg.add_component<component::name>(player_id, component::name());
    entity_name._name = "player2";

    reg.add_component<component::unique_id>(
        player_id, (component::unique_id)unique_ids
    );
    unique_ids++;
    return player_id;
}

entity Factory::make_player3() {
    entity player_id = reg.spawn_entity();

    auto& player_sprite = reg.add_component<component::drawable>(
        player_id, component::drawable()
    );
    player_sprite.setTextureFromName("player3");

    reg.add_component<component::position>(player_id, {0, 0});
    reg.add_component<component::velocity>(player_id, {0, 0});
    reg.add_component<component::controllable>(
        player_id, component::controllable()
    );
    reg.add_component<component::logic>(
        player_id, component::logic{player_logic}
    );

    auto& player_hurtbox =
        reg.add_component<component::hurtbox>(player_id, component::hurtbox());
    player_hurtbox.group = 1;
    player_hurtbox.health = 1;
    player_hurtbox.width = 32;
    player_hurtbox.height = 16;

    auto& entity_name =
        reg.add_component<component::name>(player_id, component::name());
    entity_name._name = "player3";

    reg.add_component<component::unique_id>(
        player_id, (component::unique_id)unique_ids
    );
    unique_ids++;
    return player_id;
}

entity Factory::make_player4() {
    entity player_id = reg.spawn_entity();

    auto& player_sprite = reg.add_component<component::drawable>(
        player_id, component::drawable()
    );
    player_sprite.setTextureFromName("player4");

    reg.add_component<component::position>(player_id, {0, 0});
    reg.add_component<component::velocity>(player_id, {0, 0});
    reg.add_component<component::controllable>(
        player_id, component::controllable()
    );
    reg.add_component<component::logic>(
        player_id, component::logic{player_logic}
    );

    auto& player_hurtbox =
        reg.add_component<component::hurtbox>(player_id, component::hurtbox());
    player_hurtbox.group = 1;
    player_hurtbox.health = 1;
    player_hurtbox.width = 32;
    player_hurtbox.height = 16;

    auto& entity_name =
        reg.add_component<component::name>(player_id, component::name());
    entity_name._name = "player4";

    reg.add_component<component::unique_id>(
        player_id, (component::unique_id)unique_ids
    );
    unique_ids++;
    return player_id;
}


entity Factory::make_player2_flipped() {
    entity player_id = reg.spawn_entity();

    auto& player_sprite = reg.add_component<component::drawable>(
        player_id, component::drawable()
    );
    player_sprite.setTextureFromName("player2_flipped");

    reg.add_component<component::position>(player_id, {0, 0});
    reg.add_component<component::velocity>(player_id, {0, 0});
    reg.add_component<component::controllable>(
        player_id, component::controllable()
    );
    reg.add_component<component::logic>(
        player_id, component::logic{player_evil_logic}
    );

    auto& player_hurtbox =
        reg.add_component<component::hurtbox>(player_id, component::hurtbox());
    player_hurtbox.group = 2;
    player_hurtbox.health = 1;
    player_hurtbox.width = 32;
    player_hurtbox.height = 16;

    auto& entity_name =
        reg.add_component<component::name>(player_id, component::name());
    entity_name._name = "player2_flipped";

    reg.add_component<component::unique_id>(
        player_id, (component::unique_id)unique_ids
    );
    unique_ids++;
    return player_id;
}

entity Factory::make_player_missile() {
    entity missile_id = reg.spawn_entity();
    auto& missile_sprite = reg.add_component<component::drawable>(
        missile_id, component::drawable()
    );
    missile_sprite.setTextureFromName("player_missile");

    reg.add_component<component::position>(missile_id, {100.0, 100.0});
    reg.add_component<component::velocity>(
        missile_id, {PLAYER_MISSILE_SPEED, 0.0}
    );
    auto& missile_hitbox =
        reg.add_component<component::hitbox>(missile_id, component::hitbox());
    missile_hitbox.damage = 10;
    missile_hitbox.targeted_group = 2;
    missile_hitbox.width = 16;
    missile_hitbox.height = 16;
    missile_hitbox.one_shot = true;

    auto& player_shoot_music =
        reg.add_component<component::audio>(missile_id, component::audio());
    player_shoot_music.audio.reset(new sf::Music);
    player_shoot_music.audio->openFromFile("assets/audio/player_shoot.wav");
    player_shoot_music.audio->setLoop(false);
    player_shoot_music.audio->play();

    auto& entity_name =
        reg.add_component<component::name>(missile_id, component::name());
    entity_name._name = "player_missile";

    reg.add_component<component::unique_id>(
        missile_id, (component::unique_id)unique_ids
    );
    unique_ids++;
    return missile_id;
}

entity Factory::make_evil_player_missile() {
    entity missile_id = reg.spawn_entity();
    auto& missile_sprite = reg.add_component<component::drawable>(
        missile_id, component::drawable()
    );
    missile_sprite.setTextureFromName("player_missile_flipped");

    reg.add_component<component::position>(missile_id, {100.0, 100.0});
    reg.add_component<component::velocity>(
        missile_id, {-PLAYER_MISSILE_SPEED, 0.0}
    );
    auto& missile_hitbox =
        reg.add_component<component::hitbox>(missile_id, component::hitbox());
    missile_hitbox.damage = 10;
    missile_hitbox.targeted_group = 1;
    missile_hitbox.width = 16;
    missile_hitbox.height = 16;
    missile_hitbox.one_shot = true;

    auto& player_shoot_music =
        reg.add_component<component::audio>(missile_id, component::audio());
    player_shoot_music.audio.reset(new sf::Music);
    player_shoot_music.audio->openFromFile("assets/audio/player_shoot.wav");
    player_shoot_music.audio->setLoop(false);
    player_shoot_music.audio->play();

    auto& entity_name =
        reg.add_component<component::name>(missile_id, component::name());
    entity_name._name = "evil_player_missile";

    reg.add_component<component::unique_id>(
        missile_id, (component::unique_id)unique_ids
    );
    unique_ids++;
    return missile_id;
}

entity Factory::make_enemy_missile() {
    entity missile_id = reg.spawn_entity();

    auto& missile_sprite = reg.add_component<component::animated_drawable>(
        missile_id, component::animated_drawable()
    );
    missile_sprite.setTextureFromName("enemy_missile");
    missile_sprite.setFrameRect(17, 16);
    missile_sprite.frame_duration = 0.25;

    reg.add_component<component::position>(missile_id, {0, 0});
    reg.add_component<component::velocity>(
        missile_id, {ENEMY_MISSILE_SPEED, 0}
    );
    auto& missile_hitbox =
        reg.add_component<component::hitbox>(missile_id, component::hitbox());
    missile_hitbox.damage = 10;
    missile_hitbox.targeted_group = 1;
    missile_hitbox.width = 16;
    missile_hitbox.height = 16;
    missile_hitbox.one_shot = true;

    auto& enemy_shoot_music =
        reg.add_component<component::audio>(missile_id, component::audio());
    enemy_shoot_music.audio.reset(new sf::Music);
    enemy_shoot_music.audio->openFromFile("assets/audio/enemy_shoot.wav");
    enemy_shoot_music.audio->setLoop(false);
    enemy_shoot_music.audio->play();

    auto& entity_name =
        reg.add_component<component::name>(missile_id, component::name());
    entity_name._name = "enemy_missile";

    reg.add_component<component::unique_id>(
        missile_id, (component::unique_id)unique_ids
    );
    unique_ids++;
    return missile_id;
}

entity Factory::make_force() {
    entity force_id = reg.spawn_entity();

    auto& force_sprite = reg.add_component<component::animated_drawable>(
        force_id, component::animated_drawable()
    );
    force_sprite.setTextureFromName("force");
    force_sprite.setFrameRect(24, 19);
    force_sprite.frame_duration = 0.25;

    reg.add_component<component::position>(force_id, {200, 200});
    reg.add_component<component::velocity>(
        force_id, {0, 0}
    );
    auto& force_hurtbox =
        reg.add_component<component::hurtbox>(force_id, component::hurtbox());
    force_hurtbox.health = 100;
    force_hurtbox.group = 3;
    force_hurtbox.width = 24;
    force_hurtbox.height = 19;

    auto& entity_name =
        reg.add_component<component::name>(force_id, component::name());
    entity_name._name = "force";

    reg.add_component<component::unique_id>(
        force_id, (component::unique_id)unique_ids
    );

    reg.add_component<component::logic>(
        force_id, component::logic{force_logic}
    );
    unique_ids++;
    return force_id;
}

entity Factory::make_plane() {
    entity plane_id = reg.spawn_entity();

    auto& plane_sprite = reg.add_component<component::animated_drawable>(
        plane_id, component::animated_drawable()
    );
    plane_sprite.setFrameRect(33, 36);
    plane_sprite.frame_duration = 0.25;
    plane_sprite.setTextureFromName("plane");

    reg.add_component<component::position>(plane_id, {0, 0});
    reg.add_component<component::velocity>(plane_id, {-PLANE_SPEED, 0});
    reg.add_component<component::logic>(
        plane_id, component::logic{plane_logic}
    );
    auto& plane_hurtbox =
        reg.add_component<component::hurtbox>(plane_id, component::hurtbox());
    plane_hurtbox.group = 2;
    plane_hurtbox.health = 20;
    plane_hurtbox.width = 33;
    plane_hurtbox.height = 36;

    auto& plane_hitbox =
        reg.add_component<component::hitbox>(plane_id, component::hitbox());
    plane_hitbox.targeted_group = 1;
    plane_hitbox.damage = 20;
    plane_hitbox.width = 33;
    plane_hitbox.height = 36;
    plane_hitbox.one_shot = false;

    auto& entity_name =
        reg.add_component<component::name>(plane_id, component::name());
    entity_name._name = "plane";

    reg.add_component<component::unique_id>(
        plane_id, (component::unique_id)unique_ids
    );
    unique_ids++;
    return plane_id;
}

entity Factory::make_red_trooper() {
    entity troper_id = reg.spawn_entity();

    auto& troper_sprite = reg.add_component<component::animated_drawable>(
        troper_id, component::animated_drawable()
    );
    troper_sprite.setFrameRect(33, 36);
    troper_sprite.frame_duration = 0.25;
    troper_sprite.setTextureFromName("red_trooper");

    reg.add_component<component::position>(troper_id, {0, 0});
    reg.add_component<component::velocity>(troper_id, {-WALKER_SPEED, 0});
    reg.add_component<component::logic>(
        troper_id, component::logic{red_trooper_logic}
    );
    auto& troper_hurtbox =
        reg.add_component<component::hurtbox>(troper_id, component::hurtbox());
    troper_hurtbox.group = 2;
    troper_hurtbox.health = 20;
    troper_hurtbox.width = 33;
    troper_hurtbox.height = 36;

    auto& troper_hitbox =
        reg.add_component<component::hitbox>(troper_id, component::hitbox());
    troper_hitbox.targeted_group = 1;
    troper_hitbox.damage = 20;
    troper_hitbox.width = 33;
    troper_hitbox.height = 36;
    troper_hitbox.one_shot = false;

    auto& entity_name =
        reg.add_component<component::name>(troper_id, component::name());
    entity_name._name = "red_trooper";

    reg.add_component<component::unique_id>(
        troper_id, (component::unique_id)unique_ids
    );
    unique_ids++;
    return troper_id;
}

entity Factory::make_walker() {
    entity walker_id = reg.spawn_entity();

    auto& walker_sprite = reg.add_component<component::animated_drawable>(
        walker_id, component::animated_drawable()
    );
    walker_sprite.setFrameRect(33, 33);
    walker_sprite.frame_duration = 0.25;
    walker_sprite.setTextureFromName("walker");

    reg.add_component<component::position>(walker_id, {0, 0});
    reg.add_component<component::velocity>(walker_id, {-WALKER_SPEED, 0});
    reg.add_component<component::logic>(
        walker_id, component::logic{walker_logic}
    );

    auto& walker_hurtbox =
        reg.add_component<component::hurtbox>(walker_id, component::hurtbox());
    walker_hurtbox.group = 2;
    walker_hurtbox.health = 40;
    walker_hurtbox.width = 33;
    walker_hurtbox.height = 33;

    auto& walker_hitbox =
        reg.add_component<component::hitbox>(walker_id, component::hitbox());
    walker_hitbox.targeted_group = 1;
    walker_hitbox.damage = 20;
    walker_hitbox.width = 33;
    walker_hitbox.height = 33;
    walker_hitbox.one_shot = false;

    auto& entity_name =
        reg.add_component<component::name>(walker_id, component::name());
    entity_name._name = "walker";

    reg.add_component<component::unique_id>(
        walker_id, (component::unique_id)unique_ids
    );
    unique_ids++;
    return walker_id;
}

entity Factory::make_explosion() {
    entity explosion_id = reg.spawn_entity();

    reg.add_component<component::position>(explosion_id, {0, 0});
    auto& explosion_sprite = reg.add_component<component::animated_drawable>(
        explosion_id, component::animated_drawable()
    );
    explosion_sprite.setTextureFromName("explosion");
    explosion_sprite.sprite.setScale(0.33, 0.33);
    explosion_sprite.one_shot = true;
    explosion_sprite.setFrameRect(96, 96);
    explosion_sprite.frame_duration = 0.1;

    auto& explosion_music =
        reg.add_component<component::audio>(explosion_id, component::audio());
    explosion_music.audio.reset(new sf::Music);
    explosion_music.audio->openFromFile("assets/audio/explosion.wav");
    explosion_music.audio->setLoop(false);
    explosion_music.audio->play();

    auto& entity_name =
        reg.add_component<component::name>(explosion_id, component::name());
    entity_name._name = "explosion";

    reg.add_component<component::unique_id>(
        explosion_id, (component::unique_id)unique_ids
    );
    unique_ids++;
    return explosion_id;
}

entity Factory::make_hit_effect() {
    entity hit_effect_id = reg.spawn_entity();

    reg.add_component<component::position>(hit_effect_id, {0, 0});
    auto& hit_effect_sprite = reg.add_component<component::animated_drawable>(
        hit_effect_id, component::animated_drawable()
    );
    hit_effect_sprite.setTextureFromName("hit_effect");
    hit_effect_sprite.one_shot = true;
    hit_effect_sprite.setFrameRect(33, 32);
    hit_effect_sprite.frame_duration = 0.05;

    auto& hit_effect_music =
        reg.add_component<component::audio>(hit_effect_id, component::audio());
    hit_effect_music.audio.reset(new sf::Music);
    hit_effect_music.audio->openFromFile("assets/audio/enemy_shoot.wav");
    hit_effect_music.audio->setLoop(false);
    hit_effect_music.audio->play();

    auto& entity_name =
        reg.add_component<component::name>(hit_effect_id, component::name());
    entity_name._name = "hit_effect";

    reg.add_component<component::unique_id>(
        hit_effect_id, (component::unique_id)unique_ids
    );
    unique_ids++;
    return hit_effect_id;
}

entity Factory::make_ceiling() {
    entity ceiling_id = reg.spawn_entity();

    auto& ceiling_sprite = reg.add_component<component::drawable>(
        ceiling_id, component::drawable()
    );
    ceiling_sprite.setTextureFromName("ceiling");
    ceiling_sprite.sprite.setTextureRect(sf::IntRect(0, 0, 100000, 64));
    ResourceManager::Instance().getTexture("ceiling").setRepeated(true);

    auto& floor_hitbox =
        reg.add_component<component::hitbox>(ceiling_id, component::hitbox());
    floor_hitbox.damage = 10;
    floor_hitbox.height = 64;
    floor_hitbox.width = 10000;
    floor_hitbox.targeted_group = 1;

    reg.add_component<component::position>(ceiling_id, {0, -32});
    reg.add_component<component::velocity>(ceiling_id, {-BACKGROUND_SPEED, 0});

    auto& entity_name =
        reg.add_component<component::name>(ceiling_id, component::name());
    entity_name._name = "ceiling";

    reg.add_component<component::unique_id>(
        ceiling_id, (component::unique_id)unique_ids
    );
    unique_ids++;
    return ceiling_id;
}

entity Factory::make_floor() {
    entity floor_id = reg.spawn_entity();

    auto& floor_sprite =
        reg.add_component<component::drawable>(floor_id, component::drawable());
    floor_sprite.setTextureFromName("ceiling");
    floor_sprite.sprite.setTextureRect(sf::IntRect(0, 0, 100000, 64));
    ResourceManager::Instance().getTexture("ceiling").setRepeated(true);

    auto& floor_hitbox =
        reg.add_component<component::hitbox>(floor_id, component::hitbox());
    floor_hitbox.damage = 10;
    floor_hitbox.height = 64;
    floor_hitbox.width = 10000;
    floor_hitbox.targeted_group = 1;

    reg.add_component<component::position>(
        floor_id, {0, WINDOW_HEIGHT - 64 + 32}
    );
    reg.add_component<component::velocity>(floor_id, {-BACKGROUND_SPEED, 0});

    auto& entity_name =
        reg.add_component<component::name>(floor_id, component::name());
    entity_name._name = "floor";

    reg.add_component<component::unique_id>(
        floor_id, (component::unique_id)unique_ids
    );
    unique_ids++;
    return floor_id;
}

entity Factory::make_wall() {
    entity wall_id = reg.spawn_entity();

    auto& wall_sprite =
        reg.add_component<component::drawable>(wall_id, component::drawable());
    wall_sprite.setTextureFromName("assets/sprites/bakcground/wall.gif");

    reg.add_component<component::position>(wall_id, {0, 0});
    reg.add_component<component::velocity>(wall_id, {0, 0});

    auto& entity_name =
        reg.add_component<component::name>(wall_id, component::name());
    entity_name._name = "wall";

    reg.add_component<component::unique_id>(
        wall_id, (component::unique_id)unique_ids
    );
    unique_ids++;
    return wall_id;
}

entity Factory::make_background() {
    entity background_id = reg.spawn_entity();

    auto& background_sprite = reg.add_component<component::drawable>(
        background_id, component::drawable()
    );
    background_sprite.setTextureFromName("background");
    background_sprite.sprite.setTextureRect(sf::IntRect(0, 0, 100000, 500));
    ResourceManager::Instance().getTexture("background").setRepeated(true);

    reg.add_component<component::position>(background_id, {0, 0});
    reg.add_component<component::velocity>(
        background_id, {-BACKGROUND_SPEED, 0}
    );

    auto& entity_name =
        reg.add_component<component::name>(background_id, component::name());
    entity_name._name = "background";

    reg.add_component<component::unique_id>(
        background_id, (component::unique_id)unique_ids
    );
    unique_ids++;
    return background_id;
}

entity Factory::make_title() {
    entity title_id = reg.spawn_entity();

    reg.add_component<component::position>(title_id, {75, 25});
    auto& title_text =
        reg.add_component<component::text>(title_id, component::text());
    title_text.setFontFromName("arcade");
    title_text.text.setString("R  TYPE");
    title_text.text.setCharacterSize(200);
    title_text.text.setFillColor(sf::Color::White);
    title_text.text.setOutlineColor(sf::Color(10, 14, 69));
    title_text.text.setOutlineThickness(2);

    auto& entity_name =
        reg.add_component<component::name>(title_id, component::name());
    entity_name._name = "title";

    reg.add_component<component::unique_id>(
        title_id, (component::unique_id)unique_ids
    );
    unique_ids++;
    return title_id;
}

entity Factory::make_start_text() {
    entity text_id = reg.spawn_entity();

    reg.add_component<component::position>(
        text_id, {(WINDOW_WIDTH / 2) - 110, WINDOW_HEIGHT - 100}
    );
    auto& title_text =
        reg.add_component<component::text>(text_id, component::text());
    title_text.setFontFromName("arcade");
    title_text.text.setString("Press Space to start");
    title_text.text.setCharacterSize(16);
    title_text.text.setFillColor(sf::Color::White);
    title_text.text.setOutlineColor(sf::Color(10, 14, 69));
    title_text.text.setOutlineThickness(1);
    reg.add_component<component::logic>(
        text_id, component::logic{start_text_logic}
    );

    auto& entity_name =
        reg.add_component<component::name>(text_id, component::name());
    entity_name._name = "start_text";

    reg.add_component<component::unique_id>(
        text_id, (component::unique_id)unique_ids
    );
    unique_ids++;
    return text_id;
}

entity Factory::make_menu_background_music() {
    entity music_id = reg.spawn_entity();
    auto& music =
        reg.add_component<component::audio>(music_id, component::audio());
    music.audio.reset(new sf::Music);
    music.audio->openFromFile("assets/audio/menu.ogg");
    music.audio->setLoop(true);
    music.audio->play();

    auto& entity_name =
        reg.add_component<component::name>(music_id, component::name());
    entity_name._name = "menu_background_music";

    reg.add_component<component::unique_id>(
        music_id, (component::unique_id)unique_ids
    );
    unique_ids++;
    return music_id;
}

entity Factory::make_game_background_music() {
    entity music_id = reg.spawn_entity();
    auto& music =
        reg.add_component<component::audio>(music_id, component::audio());
    music.audio.reset(new sf::Music);
    music.audio->openFromFile("assets/audio/incredible.ogg");
    music.audio->setLoop(true);
    music.audio->play();

    auto& entity_name =
        reg.add_component<component::name>(music_id, component::name());
    entity_name._name = "game_background_music";

    reg.add_component<component::unique_id>(
        music_id, (component::unique_id)unique_ids
    );
    unique_ids++;
    return music_id;
}

entity Factory::make_fade_in_rect() {
    entity fade_id = reg.spawn_entity();

    reg.add_component<component::position>(fade_id, {0, 0});
    auto& fade_sprite =
        reg.add_component<component::drawable>(fade_id, component::drawable());
    ResourceManager::Instance().getTexture("black").setRepeated(true);
    fade_sprite.setTextureFromName("black");
    fade_sprite.sprite.setTextureRect(sf::IntRect(0, 0, 800, 500));
    fade_sprite.sprite.setColor(sf::Color(0, 0, 0, 0));

    reg.add_component<component::logic>(
        fade_id, component::logic{fade_in_rect_logic}
    );

    auto& entity_name =
        reg.add_component<component::name>(fade_id, component::name());
    entity_name._name = "fade_in_rect";

    reg.add_component<component::unique_id>(
        fade_id, (component::unique_id)unique_ids
    );
    unique_ids++;
    return fade_id;
}

entity Factory::make_fade_out_rect() {
    entity fade_id = reg.spawn_entity();

    reg.add_component<component::position>(fade_id, {0, 0});
    auto& fade_sprite =
        reg.add_component<component::drawable>(fade_id, component::drawable());
    ResourceManager::Instance().getTexture("black").setRepeated(true);
    fade_sprite.setTextureFromName("black");
    fade_sprite.sprite.setTextureRect(sf::IntRect(0, 0, 800, 500));
    fade_sprite.sprite.setColor(sf::Color(0, 0, 0, 255));

    reg.add_component<component::logic>(
        fade_id, component::logic{fade_out_rect_logic}
    );

    auto& entity_name =
        reg.add_component<component::name>(fade_id, component::name());
    entity_name._name = "fade_out_rect";

    reg.add_component<component::unique_id>(
        fade_id, (component::unique_id)unique_ids
    );
    unique_ids++;
    return fade_id;
}

entity Factory::make_green_trooper() {
    entity gtrooper_id = reg.spawn_entity();

    auto& gtrooper_sprite =
        reg.add_component<component::animated_drawable>(
        gtrooper_id, component::animated_drawable()
    );
    gtrooper_sprite.setFrameRect(55, 58);
    gtrooper_sprite.frame_duration = 0.25;
    gtrooper_sprite.setTextureFromName("green_trooper");

    reg.add_component<component::position>(gtrooper_id, {0, 0});
    reg.add_component<component::velocity>(gtrooper_id, {0, 0});

    reg.add_component<component::hurtbox>(gtrooper_id, {500, 2, 130, 50});
    reg.add_component<component::hitbox>(gtrooper_id, {10, 1, 130, 50, false});

    auto& entity_name =
        reg.add_component<component::name>(gtrooper_id, component::name());
    entity_name._name = "green_trooper";

    reg.add_component<component::logic>(gtrooper_id, component::logic{gtrooper_logic});

    reg.add_component<component::unique_id>(
        gtrooper_id, (component::unique_id)unique_ids
    );
    unique_ids++;
    return gtrooper_id;
}

// ---------------- SPACE ENEMY ---------------------

entity Factory::make_space_enemy() {
    entity spacenemy_id = reg.spawn_entity();

    auto& spacenemy_sprite =
        reg.add_component<component::animated_drawable>(
        spacenemy_id, component::animated_drawable()
    );
    spacenemy_sprite.setFrameRect(63, 50);
    spacenemy_sprite.frame_duration = 0.25;
    spacenemy_sprite.setTextureFromName("space_enemy");

    reg.add_component<component::position>(spacenemy_id, {0, 0});
    reg.add_component<component::velocity>(spacenemy_id, {0, 0});

    reg.add_component<component::hurtbox>(spacenemy_id, {100, 2, 130, 50});
    reg.add_component<component::hitbox>(spacenemy_id, {10, 1, 130, 50, false});

    auto& entity_name =
        reg.add_component<component::name>(spacenemy_id, component::name());
    entity_name._name = "space_enemy";

    reg.add_component<component::logic>(spacenemy_id, component::logic{spacenemy_logic});

    reg.add_component<component::unique_id>(
        spacenemy_id, (component::unique_id)unique_ids
    );
    unique_ids++;
    return spacenemy_id;
}


entity Factory::make_tourelles() {
    entity tourelles_id = reg.spawn_entity();

    auto& tourelles_sprite = reg.add_component<component::drawable>(
        tourelles_id, component::drawable()
    );
    tourelles_sprite.setTextureFromName("tourelles");

    reg.add_component<component::position>(tourelles_id, {0, 0});
    reg.add_component<component::velocity>(tourelles_id, {-BACKGROUND_SPEED, 0});
    reg.add_component<component::logic>(
        tourelles_id, component::logic{tourelles_logic}
    );

    auto& tourelles_hurtbox =
        reg.add_component<component::hurtbox>(tourelles_id, component::hurtbox());
    tourelles_hurtbox.group = 2;
    tourelles_hurtbox.health = 40;
    tourelles_hurtbox.width = 33;
    tourelles_hurtbox.height = 33;

    auto& tourelles_hitbox =
        reg.add_component<component::hitbox>(tourelles_id, component::hitbox());
    tourelles_hitbox.targeted_group = 1;
    tourelles_hitbox.damage = 20;
    tourelles_hitbox.width = 33;
    tourelles_hitbox.height = 33;
    tourelles_hitbox.one_shot = false;

    auto& entity_name =
        reg.add_component<component::name>(tourelles_id, component::name());
    entity_name._name = "tourelles";

    reg.add_component<component::unique_id>(
        tourelles_id, (component::unique_id)unique_ids
    );
    unique_ids++;
    return tourelles_id;
}

entity Factory::make_boss() {
    entity boss_id = reg.spawn_entity();

    auto& boss_sprite =
        reg.add_component<component::drawable>(boss_id, component::drawable());
    boss_sprite.setTextureFromName("boss");

    reg.add_component<component::position>(boss_id, {0, 0});
    reg.add_component<component::velocity>(boss_id, {0, 0});

    reg.add_component<component::hurtbox>(boss_id, {500, 2, 130, 50});
    reg.add_component<component::hitbox>(boss_id, {10, 1, 130, 50, false});

    auto& entity_name =
        reg.add_component<component::name>(boss_id, component::name());
    entity_name._name = "boss";

    reg.add_component<component::logic>(boss_id, component::logic{boss_logic});

    reg.add_component<component::unique_id>(
        boss_id, (component::unique_id)unique_ids
    );
    unique_ids++;
    return boss_id;
}


entity Factory::make_big_missile() {
    entity missile_id = reg.spawn_entity();

    auto& missile_sprite = reg.add_component<component::animated_drawable>(
        missile_id, component::animated_drawable()
    );
    missile_sprite.setTextureFromName("big_missile");
    missile_sprite.setFrameRect(64, 22);
    missile_sprite.frame_duration = 0.25;

    reg.add_component<component::position>(missile_id, {0, 0});
    reg.add_component<component::velocity>(
        missile_id, {-BIG_MISSILE_SPEED, 0}
    );
    auto& missile_hitbox =
        reg.add_component<component::hitbox>(missile_id, component::hitbox());
    missile_hitbox.damage = 10;
    missile_hitbox.targeted_group = 1;
    missile_hitbox.width = 64;
    missile_hitbox.height = 22;
    missile_hitbox.one_shot = false;

    auto& enemy_shoot_music =
        reg.add_component<component::audio>(missile_id, component::audio());
    enemy_shoot_music.audio.reset(new sf::Music);
    enemy_shoot_music.audio->openFromFile("assets/audio/enemy_shoot.wav");
    enemy_shoot_music.audio->setLoop(false);
    enemy_shoot_music.audio->play();

    auto& entity_name =
        reg.add_component<component::name>(missile_id, component::name());
    entity_name._name = "big_missile";

    reg.add_component<component::unique_id>(
        missile_id, (component::unique_id)unique_ids
    );
    unique_ids++;
    return missile_id;
}

entity Factory::make_small_shooter() {
    entity boss_id = reg.spawn_entity();

    auto& boss_sprite =
        reg.add_component<component::drawable>(boss_id, component::drawable());
    boss_sprite.setTextureFromName("small_shooter");

    reg.add_component<component::position>(boss_id, {0, 0});
    reg.add_component<component::velocity>(boss_id, {0, 0});

    reg.add_component<component::hurtbox>(boss_id, {300, 2, 118, 87});
    reg.add_component<component::hitbox>(boss_id, {10, 1, 118, 87, false});

    auto& entity_name =
        reg.add_component<component::name>(boss_id, component::name());
    entity_name._name = "small_shooter";

    reg.add_component<component::logic>(boss_id, component::logic{small_shooter_logic});

    reg.add_component<component::unique_id>(
        boss_id, (component::unique_id)unique_ids
    );
    unique_ids++;
    return boss_id;
}

entity Factory::make_big_shooter() {
    entity boss_id = reg.spawn_entity();

    auto& boss_sprite =
        reg.add_component<component::drawable>(boss_id, component::drawable());
    boss_sprite.setTextureFromName("big_shooter");

    reg.add_component<component::position>(boss_id, {0, 0});
    reg.add_component<component::velocity>(boss_id, {0, 0});

    reg.add_component<component::hurtbox>(boss_id, {300, 2, 103, 84});
    reg.add_component<component::hitbox>(boss_id, {10, 1, 103, 84, false});

    auto& entity_name =
        reg.add_component<component::name>(boss_id, component::name());
    entity_name._name = "big_shooter";

    reg.add_component<component::logic>(boss_id, component::logic{big_shooter_logic});

    reg.add_component<component::unique_id>(
        boss_id, (component::unique_id)unique_ids
    );
    unique_ids++;
    return boss_id;
}

entity Factory::make_final_boss() {
    entity boss_id = reg.spawn_entity();

    auto& boss_sprite =
        reg.add_component<component::animated_drawable>(boss_id, component::animated_drawable());
    boss_sprite.setTextureFromName("final_boss");
    boss_sprite.setFrameRect(184, 150);
    boss_sprite.frame_duration = 0.25;

    reg.add_component<component::position>(boss_id, {0, 0});
    reg.add_component<component::velocity>(boss_id, {0, 0});

    reg.add_component<component::hurtbox>(boss_id, {700, 2, 184, 150});
    reg.add_component<component::hitbox>(boss_id, {10, 1, 184, 150, false});

    auto& entity_name =
        reg.add_component<component::name>(boss_id, component::name());
    entity_name._name = "final_boss";

    reg.add_component<component::logic>(boss_id, component::logic{final_boss_logic});

    reg.add_component<component::unique_id>(
        boss_id, (component::unique_id)unique_ids
    );
    unique_ids++;
    return boss_id;
}