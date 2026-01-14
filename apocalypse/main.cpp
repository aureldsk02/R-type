/* ------------------------------------------------------------------------------------ *
 *                                                                                      *
 * EPITECH PROJECT - Mon, Nov, 2025                                                     *
 * Title           - G-CPP-500-COT-5-1-rtype-8                                          *
 * Description     -                                                                    *
 *     main                                                                             *
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

#include "registry.hpp"

using namespace component;

int player_entity_id = -1;

#define PLAYER_SPEED 150
#define ASTEROID_SPEED 250

entity make_player(registry& reg) {
    entity player = reg.spawn_entity();
    reg.add_component<position>(player, position({300, 200}));
    reg.add_component<velocity>(player, velocity({0, 0}));
    reg.add_component<hurtbox>(player, hurtbox({1, 1, 32, 32}));
    drawable &sprite = reg.add_component<drawable>(player, drawable());
    sprite.setTextureFromName("player");
    reg.add_component<controllable>(player, controllable());

    return player;
}

entity make_background(registry& reg) {
    entity background_id = reg.spawn_entity();

    reg.add_component<position>(background_id, position({0, 0}));
    auto& background_sprite = reg.add_component<component::drawable>(
        background_id, component::drawable()
    );
    background_sprite.setTextureFromName("background");

    return background_id;
}

entity make_asteroid(registry &reg) {
    entity asteroid = reg.spawn_entity();
    reg.add_component<position>(asteroid, position({double(rand() % 769), -50, 0}));
    reg.add_component<velocity>(asteroid, velocity({0, ASTEROID_SPEED}));
    reg.add_component<hitbox>(asteroid, hitbox({10, 1, 40, 34}));
    drawable &sprite = reg.add_component<drawable>(asteroid, drawable());
    sprite.setTextureFromName("asteroid");
    return asteroid;
}

void initializeGame(registry &reg) {
    make_background(reg);
    player_entity_id = make_player(reg);
}

void loadTextures()
{
    ResourceManager::Instance().load("assets/sprites/apocalypse/background_apo.png", "background", TEXTURE);
    ResourceManager::Instance().load("assets/sprites/apocalypse/player.png","player",  TEXTURE);
    ResourceManager::Instance().load("assets/sprites/apocalypse/platform.png", "platform", TEXTURE);
    ResourceManager::Instance().load("assets/sprites/apocalypse/asteroid.png", "asteroid",  TEXTURE);
    ResourceManager::Instance().load("assets/sprites/apocalypse/explosion.png", "explosion", TEXTURE);
}

void updatePlayer(registry &reg)
{
    controllable &player_con = reg.get_components<component::controllable>()[player_entity_id].value();
    velocity &player_vel = reg.get_components<component::velocity>()[player_entity_id].value();
    hurtbox &player_hurtbox = reg.get_components<component::hurtbox>()[player_entity_id].value();

    if (player_hurtbox.health <= 0) {
        printf("Game Over\n");
        exit(0);
    }

    player_vel.vx = 0;
    player_vel.vy = 0;

    if (player_con.up) {
        player_vel.vy = -PLAYER_SPEED; 
    }
    if (player_con.down) {
        player_vel.vy = PLAYER_SPEED ;
    }
    if (player_con.left) {
        player_vel.vx = -PLAYER_SPEED;
    }
    if (player_con.right) {
        player_vel.vx = PLAYER_SPEED;
    }
}

int main() {
    sf::RenderWindow win(sf::VideoMode(768, 432), "Apocalypse");
    sf::Event event;
    sf::Clock clock;
    registry reg(win);

    reg.toggleMovement();
    reg.toggleRendering();
    reg.toggleControl();
    reg.togglePhysics();

    loadTextures();
    initializeGame(reg);

    srand(time(0));
    make_asteroid(reg);
    make_asteroid(reg);
    make_asteroid(reg);
    make_asteroid(reg);
    make_asteroid(reg);
    make_asteroid(reg);

    double t = 0.0;

    while (win.isOpen()) {
        double delta = clock.restart().asSeconds();
        while (win.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                win.close();
        }

        t += delta;
        if (t >= 0.2) {
            make_asteroid(reg);
            t = 0.0;
        }

        updatePlayer(reg);
        reg.run_systems(delta);
    }
    
    return 0;
}