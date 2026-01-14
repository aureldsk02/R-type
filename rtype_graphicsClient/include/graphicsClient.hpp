/*
** EPITECH PROJECT, 2025
** G-CPP-500-COT-5-1-rtype-8
** File description:
** client
*/

#ifndef CLIENT_HPP_
#define CLIENT_HPP_

/**
 * @file client.hpp
 * @author your name (you@domain.com)
 * @brief The client class definition file
 * @version 0.1
 * @date 2025-10-13
 *
 * @copyright Copyright (c) 2026 - Aurel DOSSOU KOHI
 *
 */

#include <algorithm>
#include <map>
#include <vector>

#include "../../utility_classes/include/Network.hpp"
#include "Factory.hpp"
#include "entity.hpp"
#include "registry.hpp"
#include "UIHandler.hpp"
#include "logic_functions.hpp"
#include <chrono>
#include <ranges>
#include "Types.hpp"

#define WINDOW_WIDTH 738
#define WINDOW_HEIGHT 432

inline bool player1_dead = false;
inline bool player2_dead = false;
inline bool player3_dead = false;
inline bool player4_dead = false;

inline int controllable_id = -1;

/**
 * @brief An enum to define the different states of the game
 */
typedef enum { MENU, TRANSITION, LEVEL1, LEVEL2, LEVEL3 } state_t;
typedef enum { GAME_OVER, VICTORY, LEVEL1_START, LEVEL2_START, LEVEL3_START, SUB_NONE } substate_t;

typedef struct menu_info_s {
    entity background;
    entity title;
    entity start_text;
    entity menu_background_music;
    entity menu_fade_in_rect;
    entity menu_fade_out_rect;
} menu_info_t;

/**
 * @brief The client class. Handles the client side of the game.
 * All the client does is send messages to the server if the player tries to
 * move, and also receive the game state from the server in order to update the
 * game
 *
 */
class GraphicsClient {
  private:

    const std::chrono::milliseconds tickDuration = std::chrono::milliseconds(5);

    bool isRunning_;

    /**
     * @brief Window for rendering
     */
    sf::RenderWindow win;

    /**
     * @brief The registry that holds all entities and components
     */
    registry reg;

    /**
     * @brief The factory that creates entities and components
     */
    Factory factory;

    /**
     * @brief The port to connect to the server
     */
    int port_;


    /**
     * @brief The timer for the level
     */
    double levelTimer = 0.0;

    /**
     * @brief The network manager that handles communication with the server
     */
    NetworkManager& client_;

    /**
     * @brief Mutex for thread safety
     */
    std::mutex& mtx;

    /**
     * @brief Map that associates server entity IDs to client entity IDs
     */
    std::unordered_map<size_t, size_t> ids_assoc;

    /**
     * @brief The last message received from the server
     */
    std::vector<int8_t>& lastmsg;

    /**
     * @brief A vector containing entities states received from the server
     */
    std::vector<
        std::pair<
            int64_t,
            std::vector<EnemyMovedResponse>
                >
            > entity_states;

    /**
     * @brief Event for handling window events
     */
    sf::Event event;

    /**
     * @brief The current state of the game
     */
    state_t state;

    /**
     * @brief The current substate of the game
     */
    substate_t substate;

    /**
     * @brief Clock for managing frame time
     */
    sf::Clock frameClock;

    /**
     * @brief The moment of the last message
     */
    chrono::time_point<chrono::steady_clock> clientStarted;

    /**
     * @brief The UI handler for managing the user interface
     */
    UIHandler ui_handler;

    /**
     * @brief Initialize all game related elements
     */
    void initGame(std::vector<sf::Keyboard::Key> keyTab);

    /**
     * @brief Run the game logic
     * @param delta The time elapsed since the last frame
     */
    void runLevel(double delta);

    /**
     * @brief Receive information from the server
     */
    void receiveServerInfo();

    /**
     * @brief Send the player's input to the server
     */
    void sendPlayerInput();

    /**
     * @brief Send the player's action (e.g., shooting) to the server
     */
    void sendPlayerAction();

    GameState decodeGameState(std::vector<int8_t>& buffer);

    /**
     *
     * @param delta jnvflv
     * @param win
     */
    void handleSubStates(double delta, sf::RenderWindow& win);

    /**
     * @brief Calculate the new position of an entity based on the last
     * position, the position to reach, the time elapsed beatween the
     * last position time and now
     *
     * @param pastPos the last position of the entity
     * @param nextPos the position to reach
     * @param now the actual time
     * @param pastTime the time of the last position
     * @param nextTime the time of the next position
     */
    void entityMoveInterpole(EnemyMovedResponse pastPos, int64_t pastTime, int64_t now, EnemyMovedResponse nextPos, int64_t nextTime);

    /**
     * @brief Calculate the new position of an entity based on the last
     * position, the position to reach, the time elapsed beatween the
     * last position time and now
     *
     * @param pastPos the last position of the entity
     * @param pastpastPos the position before the last position
     * @param now the actual time
     * @param pastTime the time of the last position
     * @param pastpastTime the time of the position before the last position
     */
    void entityMovDelete(EnemyMovedResponse pastpastPos, int64_t pastpastTime, int64_t now, EnemyMovedResponse pastPos, int64_t pastTime);

    /**
     * @brief Calculate the new position of an entity based on the last
     * position, the position to reach, the time elapsed beatween the
     * last position time and now
     *
     * @param nextPos the next position of the entity
     * @param nextnextPos the position after the next position
     * @param now the actual time
     * @param nextTime the time of the next position
     * @param nextnextTime the time of the position next the next position
     */
    void entityMovCreate(EnemyMovedResponse nextPos, int64_t nextTime, int64_t now, EnemyMovedResponse nextnextPos, int64_t nextnextTime);

  public:
    /**
     * @brief Construct a new Client object
     * @param p The port to connect to the server
     * @param addr The address of the server
     */
    GraphicsClient(NetworkManager& client, std::vector<int8_t>& lastmsg, std::mutex& mtx, std::vector<sf::Keyboard::Key> keyTab);

    /**
     * @brief Destroy the Client object
     */
    ~GraphicsClient();

    /**
     * @brief Decode a NbrEntity structure from a byte buffer
     * @param buffer The byte buffer containing the encoded data
     * @return The decoded NbrEntity structure
     */
    MessageHeader decodeMessageHeader(std::vector<int8_t>& buffer);

    /**
     * @brief Decode an EnemyMovedResponse structure from a byte buffer
     * @param buffer The byte buffer containing the encoded data
     * @return The decoded EnemyMovedResponse structure
     */
    EnemyMovedResponse decodeEnemyMovedResponse(std::vector<int8_t>& buffer);

    /**
     * @brief Encode a MoveResponse structure into a byte buffer
     * @param pos The MoveResponse structure to encode
     * @return A vector of int8_t representing the encoded data
     */
    std::vector<int8_t> encodeMoveResponse(const MoveResponse& pos);

    /**
     * @brief Encode an ActionResponse structure into a byte buffer
     * @param pos The ActionResponse structure to encode
     * @return A vector of int8_t representing the encoded data
     */
    std::vector<int8_t> encodeActionResponse(const ActionResponse& pos);

    /**
     * @brief Check if the client is running
     * @return true if the client is running, false otherwise
     */
    bool isRunning() const;

    /**
     * @brief Run the client application
     */
    void run();
};

#endif /* !CLIENT_HPP_ */

