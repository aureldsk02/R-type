#ifndef GAMECLIENT_HPP
#define GAMECLIENT_HPP

#include "Audio_Handler.hpp"
#include "Chat.hpp"
#include "Display_Handler.hpp"
#include "EcsWrapper.hpp"
#include "Input_Handler.hpp"
#include "NetworkClient.hpp"
#include "Networkdata.h"
#include "RoomLobby.hpp"
#include "Scene.hpp"
#include "ecs/AiSystem.hpp"
#include "ecs/CollisionSystem.hpp"
#include "ecs/Component.hpp"
#include "ecs/DestroySystem.hpp"
#include "ecs/HealthSystem.hpp"
#include "ecs/MovementSystem.hpp"
#include "ecs/Scene_loader.hpp"
#include "ecs/ShootingSystem.hpp"
#include "ecs/SpawnSystem.hpp"
#include <chrono>
#include <iostream>
#include <map>
#include <string>
#include <thread>

class GameClient {
private:
  // Systems
  SceneLoader scene;
  SpawnSystem spawn;
  AiSystem ai;
  MovementSystem move;
  ShootingSystem shooting_enemy;
  CollisionSystem collision;
  HealthSystem health;
  DestroySystem destroy;
  Registry registry;

  DisplayHandlerSystem displayHandler;
  InputHandlerSystem inputHandler;
  AudioHandlerSystem audioHandler;
  std::unique_ptr<NetworkClient> networkClient;
  Scene sceneHandler;
  Chat gameChat;
  RoomLobby roomLobby;

  EcsManager ecsManager;
  EcsManager::EntityId playerEntity;

  uint32_t sequenceNumber;
  struct PendingMove {
    uint32_t sequence;
    PlayerMove move;
    float timestamp;
    float predicted_x;
    float predicted_y;
  };
  std::vector<PendingMove> pendingMoves;

  struct IntPosition {
    int32_t x;
    int32_t y;
  };
  std::map<uint32_t, IntPosition> playerPreviousPositions;

  std::map<uint32_t, std::string> playerNames;

  bool isRunning;
  bool connectPacketSent;
  bool gameStarted;
  bool gameOver;
  bool pendingDisconnect;
  bool multiplayerMode;
  bool inMenu;
  bool inGameModeMenu;
  bool inMultiplayerForm;
  bool inSettingsMenu;
  bool inHelpMenu;
  bool inRoomMenu;
  bool inRoomListMenu;
  bool inRoomLobby;
  bool pendingLobbyJoin = false;
  uint32_t pendingLobbyRoomId = 0;
  float lobbyPlayersRefreshTimer = 0.0f;
  std::string playerName;
  std::string serverIP;
  int serverPort;
  PlayerJoinPacket playerData;
  PlayerMovePacket playerMovePacket;
  Vector2 previousPosition;
  PlayerMove currentMove = STOP;
  size_t statusTextIndex;
  size_t playerSpriteIndex;
  size_t playerAnimationIndex;
  player_cmp_t player;
  bool playerVisualEntityCreated;

  // Menu* mainMenu;

  void handlePlayerConnect(const PlayerConnectPacket &packet);
  void handlePlayerJoin(const PlayerJoinPacket &packet);
  void handlePlayerDisconnect(const PlayerDisconnectPacket &packet);
  void handleGameAi(const GameAiPacket &packet);
  void handleGameStart(const GameStartPacket &packet);
  void handleGameCollision(const GameCollisionPacket &packet);
  void handlePlayerHasMove(const PlayerHasMovePacket &packet);
  void handleBullet(const BulletPacket &packet);
  // void handleMenuAction(const std::string& action);
  SpriteComponent oldBackground;
  SpriteComponent newBackground;

  float parallaxOffsetX = 0.0f;
  float parallaxBaseSpeed = 60.0f;

  float parallaxOffset = 0.0f;
  float parallaxSpeed = 80.0f;

  void handleMenuAction(const std::string &action);
  void handleGameModeAction(const std::string &action);
  void handleMultiplayerFormAction(const std::string &action);
  void handleSettingsAction(const std::string &action);
  void handleHelpAction(const std::string &action);
  void handleRoomAction(const std::string &action);
  void handleRoomLobbyAction(const std::string &action);
  void handleRoomListAction(const std::string &action);
  void connectToServer(const std::string &ip, int port);
  void createPlayerEntity();
  bool shouldQuitGame() const;
  void updatePlayerGraphics();

  void executeMove(PlayerMove move, uint32_t sequence);
  void validateServerMove(const PlayerHasMovePacket &packet, float server_x,
                          float server_y);
  void reconcilePosition(float server_x, float server_y,
                         uint32_t server_sequence);
  void handleRoomList(const RoomListPacket &packet);

public:
  GameClient();
  ~GameClient();
  void Initialize(const std::string &playerName = "Player1");
  void Run();
  void Update();
  void Cleanup();
  void DisplayGame();
  void DrawParallaxBackground(const SpriteComponent &bg, float deltaTime,
                              float speedMultiplier = 1.0f);
};

#endif // GAMECLIENT_HPP
