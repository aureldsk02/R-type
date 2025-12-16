#include "GameClient.hpp"
#include "Chat.hpp"
#include "FontManager.hpp"
#include "RoomLobby.hpp"
#include <algorithm>
#include <chrono>
#include <cmath>
#include <cstring>
#include <iostream>
#include <memory>
#include <string>
#include <thread>

GameClient::GameClient()
    : displayHandler(), inputHandler(), audioHandler(), networkClient(nullptr),
      sceneHandler(), sequenceNumber(1), isRunning(false),
      connectPacketSent(false), gameStarted(false), gameOver(false),
      pendingDisconnect(false), multiplayerMode(false), inMenu(true),
      inGameModeMenu(false), inMultiplayerForm(false), inSettingsMenu(false),
      inHelpMenu(false), inRoomMenu(false), inRoomListMenu(false),
      inRoomLobby(false), playerName(""), serverIP(""), serverPort(0),
      playerData{}, playerMovePacket{}, previousPosition{0, 0},
      currentMove(STOP), statusTextIndex(0), playerSpriteIndex(0),
      playerAnimationIndex(0), player{} {

  player.score = 0;
}

void GameClient::handleRoomListAction(const std::string &action) {
  SoundSource menuSound;
  menuSound.pathfile = "res/Sound FX/hoverButton.wav";
  menuSound.name = "menu_click";
  menuSound.volume = 0.7f;
  audioHandler.Update_currentSound(menuSound);
  if (audioHandler.LoadSound()) {
    audioHandler.PlaySound();
  }

  if (action == "joinRoom") {
    uint32_t roomId = sceneHandler.getCurrentRoomListMenu().GetSelectedRoomId();
    if (roomId != 0 && networkClient) {
      if (playerData.room_id == roomId) {
        inRoomListMenu = false;
        inRoomMenu = false;
        if (!inRoomLobby) {
          roomLobby.Initialize();
        }
        roomLobby.SetRoomInfo(std::string("Room ") + std::to_string(roomId),
                              static_cast<int>(roomId), 4);
        roomLobby.SetIsRoomOwner(false);
        roomLobby.AddPlayer(playerName, static_cast<int>(playerData.player_id));
        inRoomLobby = true;
        pendingLobbyJoin = false;
        pendingLobbyRoomId = 0;
        lobbyPlayersRefreshTimer = 0.0f;
        if (networkClient) {
          networkClient->send_get_players_in_room(roomId);
        }
      } else {
        networkClient->send_connect_packet(playerName);
        networkClient->send_connect_to_room(roomId, playerName);
        inRoomListMenu = false;
        inRoomMenu = false;
        if (!inRoomLobby) {
          roomLobby.Initialize();
        }
        roomLobby.SetRoomInfo(std::string("Room ") + std::to_string(roomId),
                              static_cast<int>(roomId), 4);
        roomLobby.SetIsRoomOwner(false);
        roomLobby.AddPlayer(playerName, static_cast<int>(playerData.player_id));
        inRoomLobby = true;
        pendingLobbyJoin = true;
        pendingLobbyRoomId = roomId;
        lobbyPlayersRefreshTimer = 0.0f;
        if (networkClient) {
          networkClient->send_get_players_in_room(roomId);
        }
      }
    }
  } else if (action == "back") {
    inRoomListMenu = false;
    inRoomMenu = true;
  } else if (action == "addRoom") {
    if (networkClient) {
      networkClient->send_create_room();
      networkClient->send_get_room_list();
    }
  }
}

void GameClient::handleRoomList(const RoomListPacket &packet) {
  try {
    auto &menu = sceneHandler.getCurrentRoomListMenu();
    menu.ClearRooms();
    uint32_t count = std::min(packet.num_rooms, (uint32_t)4);
    for (uint32_t i = 0; i < count; ++i) {
      uint32_t id = packet.room_ids[i];
      int players = static_cast<int>(packet.player_per_room[i]);
      int maxPlayers = 4;
      std::string name = std::string("Room ") + std::to_string(id);
      menu.AddRoom(id, name, players, maxPlayers);
    }
    if (!inRoomLobby) {
      inRoomMenu = false;
      inRoomListMenu = true;
    }
  } catch (const std::exception &e) {
  }
}

GameClient::~GameClient() { Cleanup(); }

void GameClient::Initialize(const std::string &name) {
  playerName = name;
  playerData = PlayerJoinPacket{};

  displayHandler.Initialize();
  inputHandler.Initialize();
  audioHandler.Initialize();
  sceneHandler.Initialize();
  auto bgSprites = displayHandler.GetSpritesBackground();
  if (!bgSprites.empty()) {
    sceneHandler.getcurrentGame().setBackground(bgSprites[0]);
    sceneHandler.getcurrentGame().setStars(bgSprites[1]);
  } else {
    SpriteComponent fallbackBg;
    fallbackBg.position = {0, 0};
    fallbackBg.scale = 1.0f;
    fallbackBg.visible = true;
    sceneHandler.getcurrentGame().setBackground(fallbackBg);
    sceneHandler.getcurrentGame().setStars(fallbackBg);
  }

  auto mainMenu = std::make_unique<Menu>();
  mainMenu->Initialize();
  sceneHandler.addMenu(std::move(mainMenu));

  auto gameModeMenu = std::make_unique<GameModeMenu>();
  gameModeMenu->Initialize();
  sceneHandler.addGameModeMenu(std::move(gameModeMenu));

  auto multiplayerForm = std::make_unique<MultiplayerForm>();
  multiplayerForm->Initialize();
  sceneHandler.addMultiplayerForm(std::move(multiplayerForm));

  auto settingsMenu = std::make_unique<SettingsMenu>();
  settingsMenu->Initialize();
  sceneHandler.addSettingsMenu(std::move(settingsMenu));

  auto helpMenu = std::make_unique<HelpMenu>();
  helpMenu->Initialize();
  sceneHandler.addHelpMenu(std::move(helpMenu));

  auto roomMenu = std::make_unique<RoomMenu>();
  roomMenu->Initialize();
  sceneHandler.addRoomMenu(std::move(roomMenu));

  auto roomListMenu = std::make_unique<RoomListMenu>();
  roomListMenu->Initialize();
  sceneHandler.addRoomListMenu(std::move(roomListMenu));

  roomLobby.Initialize();

  // auto currentGame = std::make_unique<Game>();
  // currentGame->setBackground(displayHandler.GetSpritesBackground()[0]);
  // printf("Background set successfully!\n");
  // currentGame->addEntity("Player", displayHandler.GetSpritesPlayer()[0], 0);
  // currentGame->addEntity("Enemy", displayHandler.GetSpritesEnemy()[1], 1);
  // sceneHandler.addGame(std::move(currentGame));

  playerEntity = ecsManager.createEntity();
  ecsManager.addPosition(playerEntity, 100.0f, 100.0f);
  ecsManager.addVelocity(playerEntity, 0.0f, 0.0f);
  ecsManager.addTeam(playerEntity, 0);

  gameChat.Initialize();
  gameChat.SetPlayerName(playerName);

  if (audioHandler.LoadMusic("res/music/startsong.mp3", "Menu Music")) {
    audioHandler.PlayMusic();
    std::cout << "Musique des menus lancée (startsong.mp3)" << std::endl;
  } else {
    std::cout << "Impossible de charger la musique des menus" << std::endl;
  }

  isRunning = true;

  // Add test entities for local display (disabled by default)
  // auto players = displayHandler.GetSpritesPlayer();
  // if (!players.empty()) {
  //     SpriteComponent testPlayer = players[0];
  //     testPlayer.position = {400, 400}; // Center-ish position
  //     testPlayer.visible = true;
  //     testPlayer.scale = 2.0f;
  //     sceneHandler.getcurrentGame().addEntity("Player", testPlayer, 999);
  //     std::cout << "Added test player sprite at position (400, 400)" <<
  //     std::endl;
  // }

  // // Add test enemies
  // auto enemies = displayHandler.GetSpritesEnemy();
  // if (!enemies.empty()) {
  //     SpriteComponent testEnemy1 = enemies[0];
  //     testEnemy1.position = {600, 200};
  //     testEnemy1.visible = true;
  //     testEnemy1.scale = 2.0f;
  //     sceneHandler.getcurrentGame().addEntity("Enemy", testEnemy1, 998);
  //     std::cout << "Added test enemy sprite at position (600, 200)" <<
  //     std::endl;
  //
  //     if (enemies.size() > 1) {
  //         SpriteComponent testEnemy2 = enemies[1];
  //         testEnemy2.position = {800, 300};
  //         testEnemy2.visible = true;
  //         testEnemy2.scale = 2.0f;
  //         sceneHandler.getcurrentGame().addEntity("Enemy", testEnemy2, 997);
  //         std::cout << "Added test enemy2 sprite at position (800, 300)" <<
  //         std::endl;
  //     }
  // }
  //
  // Add test fire/bullet sprites (disabled)
  // auto fire = displayHandler.GetSpritesFire();
  // if (!fire.empty()) {
  //     SpriteComponent testFire = fire[0];
  //     testFire.position = {500, 350};
  //     testFire.visible = true;
  //     testFire.scale = 1.5f;
  //     sceneHandler.getcurrentGame().addEntity("Fire", testFire, 996);
  //     std::cout << "Added test fire sprite at position (500, 350)" <<
  //     std::endl;
  // }
}

void GameClient::handlePlayerConnect(const PlayerConnectPacket &packet) {
  (void)packet;
  sceneHandler.getCurrentRoomMenu().SetServerInfo(serverIP, serverPort);

  inMultiplayerForm = false;
  inRoomMenu = true;
}

void GameClient::createPlayerEntity() {
  player.id = 0;
  player.position_x = 100.0f;
  player.position_y = 300.0f;

  auto players = displayHandler.GetSpritesPlayer();
  if (!players.empty()) {
    sceneHandler.getcurrentGame().addEntity("Player", players[0], player.id);
  } else {
    SpriteComponent fallbackPlayer;
    fallbackPlayer.position = {player.position_x, player.position_y};
    fallbackPlayer.scale = 1.0f;
    fallbackPlayer.visible = true;
    sceneHandler.getcurrentGame().addEntity("Player", fallbackPlayer,
                                            player.id);
  }
}

void GameClient::handlePlayerJoin(const PlayerJoinPacket &packet) {
  std::cout << "=== PlayerJoinPacket Reçu ===" << std::endl;
  std::cout << "Player ID: " << packet.player_id << std::endl;
  std::cout << "Player Name: "
            << std::string(reinterpret_cast<const char *>(packet.player_name))
            << std::endl;
  std::cout << "Room ID: " << packet.room_id << std::endl;
  std::cout << "Position X: " << packet.x << std::endl;
  std::cout << "Position Y: " << packet.y << std::endl;
  std::cout << "Ship Type: " << packet.ship_type << std::endl;
  std::cout << "=============================" << std::endl;

  playerData = packet;

  player.position_x = playerData.x;
  player.position_y = playerData.y;
  player.id = playerData.player_id;

  playerPreviousPositions[playerData.player_id] = {
      static_cast<int32_t>(playerData.x), static_cast<int32_t>(playerData.y)};
  std::cout << "[INIT] Position initiale du joueur " << playerData.player_id
            << ": (" << playerData.x << ", " << playerData.y << ")"
            << std::endl;

  playerMovePacket.player_id = playerData.player_id;
  playerMovePacket.room_id = playerData.room_id;
  if (pendingLobbyJoin &&
      (pendingLobbyRoomId == 0 || pendingLobbyRoomId == playerData.room_id)) {
    inRoomMenu = false;
    inRoomListMenu = false;
    if (!inRoomLobby) {
      roomLobby.Initialize();
    }
    inRoomLobby = true;
    pendingLobbyJoin = false;
    pendingLobbyRoomId = 0;
  }

  if (inRoomLobby) {
    roomLobby.SetRoomInfo(std::string("Room ") +
                              std::to_string(playerData.room_id),
                          static_cast<int>(playerData.room_id), 4);
    roomLobby.SetIsRoomOwner(false);
    if (networkClient) {
      networkClient->send_get_players_in_room(playerData.room_id);
    }
    lobbyPlayersRefreshTimer = 0.0f;
  }

  try {
    std::string joinedName =
        std::string(reinterpret_cast<const char *>(packet.player_name));

    playerNames[packet.player_id] = joinedName;
    if (inRoomLobby) {
      roomLobby.AddPlayer(joinedName, static_cast<int>(packet.player_id));
    }
    if (packet.player_id != player.id) {
      playerPreviousPositions[packet.player_id] = {
          static_cast<int32_t>(packet.x), static_cast<int32_t>(packet.y)};
      std::cout << "[INIT] Position initiale du joueur distant "
                << packet.player_id << ": (" << packet.x << ", " << packet.y
                << ")" << std::endl;
    }
  } catch (...) {
  }

  displayHandler.UpdateText(
      statusTextIndex,
      "Connected! Player ID: " + std::to_string(playerData.player_id) +
          ", Room: " + std::to_string(playerData.room_id));
}

void GameClient::handlePlayerDisconnect(const PlayerDisconnectPacket &packet) {
  std::cout << "========================================" << std::endl;
  std::cout << "=== PlayerDisconnectPacket Reçu ===" << std::endl;
  std::cout << "========================================" << std::endl;
  std::cout << "Player ID déconnecté: " << packet.player_id << std::endl;
  std::cout << "Player local ID: " << player.id << std::endl;
  std::cout << "État du jeu:" << std::endl;
  std::cout << "  - gameStarted: " << (gameStarted ? "true" : "false")
            << std::endl;
  std::cout << "  - inRoomLobby: " << (inRoomLobby ? "true" : "false")
            << std::endl;
  std::cout << "  - multiplayerMode: " << (multiplayerMode ? "true" : "false")
            << std::endl;
  std::cout << "========================================" << std::endl;

  auto allEntities = sceneHandler.getcurrentGame().getallEntity();
  auto found = std::find_if(
      allEntities.begin(), allEntities.end(), [&packet](const auto &p) {
        return p.second == static_cast<int>(packet.player_id);
      });

  if (found != allEntities.end()) {
    std::cout << "[ACTION] Suppression de l'entité visuelle du joueur "
              << packet.player_id << std::endl;
    sceneHandler.getcurrentGame().removeEntity(packet.player_id);
  } else {
    std::cout << "[INFO] Aucune entité visuelle trouvée pour le joueur "
              << packet.player_id << std::endl;
  }

  auto posIt = playerPreviousPositions.find(packet.player_id);
  if (posIt != playerPreviousPositions.end()) {
    std::cout << "[ACTION] Suppression de la position stockée du joueur "
              << packet.player_id << " (était à: " << posIt->second.x << ", "
              << posIt->second.y << ")" << std::endl;
    playerPreviousPositions.erase(packet.player_id);
  } else {
    std::cout << "[INFO] Aucune position stockée pour le joueur "
              << packet.player_id << std::endl;
  }

  if (inRoomLobby) {
    std::cout << "[ACTION] Retrait du joueur " << packet.player_id
              << " du lobby" << std::endl;
    roomLobby.RemovePlayer(static_cast<int>(packet.player_id));
  }

  if (gameStarted && !inRoomLobby) {
    std::cout << "========================================" << std::endl;
    std::cout << "!!!  DÉCONNEXION PROGRAMMÉE  !!!" << std::endl;
    std::cout << "Raison: Déconnexion du joueur " << packet.player_id
              << std::endl;
    std::cout << "========================================" << std::endl;

    try {
      if (audioHandler.LoadMusic("res/music/gameover.mp3", "Game Over Music")) {
        audioHandler.PlayMusic();
        std::cout << "[AUDIO] 💀 Musique Game Over lancée" << std::endl;
      }
    } catch (const std::exception &e) {
      std::cout << "[AUDIO] Erreur: " << e.what() << std::endl;
    }

    pendingDisconnect = true;
  } else {
    std::cout << "[INFO] Déconnexion dans le lobby - simple retrait du joueur"
              << std::endl;
  }

  std::cout << "========================================" << std::endl;
}

void GameClient::handleGameStart(const GameStartPacket &packet) {
  (void)packet;
  if (!inRoomLobby) {
    return;
  }

  createPlayerEntity();
  inRoomLobby = false;
  gameStarted = true;

  if (audioHandler.LoadMusic("res/music/Naruto Akatsuki Theme Song.mp3",
                             "Akatsuki Theme")) {
    audioHandler.PlayMusic();
    std::cout << "Musique Naruto Akatsuki lancée - La partie commence!"
              << std::endl;
  } else {
    std::cout << "Impossible de charger la musique Naruto Akatsuki"
              << std::endl;
  }
}

void GameClient::handleGameAi(const GameAiPacket &packet) {
  try {
    int id = packet.entity_id;
    auto allEntities = sceneHandler.getcurrentGame().getallEntity();
    auto entity = find_if(allEntities.begin(), allEntities.end(),
                          [id](const auto &p) { return p.second == id; });

    std::string subtype;
    switch (packet.entity_type) {
    case 0:
      subtype = "BASIC_ENEMY";
      break;
    case 1:
      subtype = "FIRING_ENEMY";
      break;
    case 2:
      subtype = "FOLLOWING_ENEMY";
      break;
    default:
      subtype = "BASIC_ENEMY";
      break;
    }

    auto pickEnemySprite = [&](const std::string &wantedType) {
      SpriteComponent s;
      auto enemies = displayHandler.GetSpritesEnemy();
      bool matched = false;
      if (!enemies.empty()) {
        for (const auto &e : enemies) {
          if (e.type == wantedType && e.layer == sceneHandler.getGameCount()) {
            s = e;
            matched = true;
            break;
          }
        }
        if (!matched) {
          s = enemies[0];
        }
      } else {
        s = SpriteComponent();
        s.visible = true;
        s.tint = {255, 255, 255, 255};
      }
      s.type = wantedType;
      return s;
    };

    if (entity != allEntities.end()) {
      // std::cout << "Updating entity ID: " << id << " to position (" <<
      // packet.position_x << ", " << packet.position_y << ")" << std::endl;
      sceneHandler.getcurrentGame().Update(
          GetFrameTime(), id, Vector2{packet.position_x, packet.position_y});
    } else {
      SpriteComponent sprite = pickEnemySprite(subtype);
      sprite.position = {packet.position_x, packet.position_y};
      sceneHandler.getcurrentGame().addEntity("Enemy", sprite, id);
    }
  } catch (const std::exception &e) {
  }
}

void GameClient::handleGameCollision(const GameCollisionPacket &packet) {
  std::cout << "damage collision between " << packet.entity_id_a << " && "
            << packet.entity_id_b << std::endl;
  if (packet.player_id == player.id) {
    player.score = packet.score;
    std::cout << "Score updated (total): " << player.score << std::endl;
  }

  SoundSource explosionSound;
  explosionSound.pathfile = "res/Sound FX/explosion.wav";
  explosionSound.name = "explosion";
  explosionSound.volume = 1.0f;
  audioHandler.Update_currentSound(explosionSound);
  if (audioHandler.LoadSound()) {
    audioHandler.PlaySound();
  }

  sceneHandler.getcurrentGame().removeEntity(packet.entity_id_a);
  sceneHandler.getcurrentGame().removeEntity(packet.entity_id_b);
}

void GameClient::handlePlayerHasMove(const PlayerHasMovePacket &packet) {
  float server_x = static_cast<float>(packet.position_x);
  float server_y = static_cast<float>(packet.position_y);

  std::cout << "Player " << packet.player_id << " position: (" << server_x
            << ", " << server_y << ")" << std::endl;

  if (packet.player_id == player.id) {
    validateServerMove(packet, server_x, server_y);
  } else {
    auto allEntities = sceneHandler.getcurrentGame().getallEntity();
    auto found = find_if(
        allEntities.begin(), allEntities.end(),
        [&packet](const auto &p) { return p.second == (int)packet.player_id; });
    if (found == allEntities.end()) {
      auto players = displayHandler.GetSpritesPlayer();
      SpriteComponent remote;
      if (!players.empty()) {
        remote = players[0];
        remote.tint = {150, 150, 255, 255};
      } else {
        remote = SpriteComponent();
        remote.visible = true;
        remote.scale = 1.0f;
        remote.tint = {150, 150, 255, 255};
      }
      remote.position = {server_x, server_y};
      sceneHandler.getcurrentGame().addEntity("Player", remote,
                                              (int)packet.player_id);
    } else {
      sceneHandler.getcurrentGame().Update(
          GetFrameTime(), (int)packet.player_id, Vector2{server_x, server_y});
    }
  }
}

void GameClient::handleBullet(const BulletPacket &packet) {
  // std::cout << "Bullet packet received - Bullet ID: " << packet.bullet_id
  //           << ", Player ID: " << packet.player_id
  //           << ", Position: (" << packet.position_x << ", " <<
  //           packet.position_y << ")"
  //           << ", Direction: (" << packet.direction_x << ", " <<
  //           packet.direction_y << ")"
  //           << ", IsPlayer=" << packet.is_player_bullet
  //           << std::endl;

  int id = packet.bullet_id;
  auto allEntities = sceneHandler.getcurrentGame().getallEntity();
  auto entity = find_if(allEntities.begin(), allEntities.end(),
                        [id](const auto &p) { return p.second == id; });
  if (entity != allEntities.end()) {
    sceneHandler.getcurrentGame().Update(
        GetFrameTime(), id, Vector2{packet.position_x, packet.position_y});
    // std::cout << "Updated bullet ID " << id << " position to (" <<
    // packet.position_x << ", " << packet.position_y << ")" << std::endl;
  } else {
    SpriteComponent bulletSprite;

    auto fire = displayHandler.GetSpritesFire();
    if (!fire.empty()) {
      if (packet.is_player_bullet && fire.size() > 1) {
        bulletSprite = fire[1];
      } else {
        bulletSprite = fire[0];
      }
      std::cout << "display rect: " << bulletSprite.sourceRect.x << ", "
                << bulletSprite.sourceRect.y << ", "
                << bulletSprite.sourceRect.width << ", "
                << bulletSprite.sourceRect.height << std::endl;
      bulletSprite.scale = 0.8f;
      std::cout << "Created bullet ID " << id << " using fire sprite"
                << std::endl;
    } else {
      bulletSprite.visible = true;
      bulletSprite.scale = 0.5f;
      bulletSprite.tint = {255, 255, 0, 255};
      std::cout << "Created bullet ID " << id << " using fallback sprite"
                << std::endl;
    }

    bulletSprite.position = {packet.position_x, packet.position_y};
    bulletSprite.type = "Fire";

    sceneHandler.getcurrentGame().addEntity("Fire", bulletSprite, id);
    std::cout << "Fire entity created successfully!" << std::endl;
  }
}

void GameClient::handleMenuAction(const std::string &action) {
  SoundSource menuSound;
  menuSound.pathfile = "res/Sound FX/hoverButton.wav";
  menuSound.name = "menu_click";
  menuSound.volume = 0.7f;
  audioHandler.Update_currentSound(menuSound);
  if (audioHandler.LoadSound()) {
    audioHandler.PlaySound();
  }

  if (action == "startGame") {
    std::cout << "Opening game mode selection..." << std::endl;
    inMenu = false;
    inGameModeMenu = true;
  } else if (action == "openSettings") {
    std::cout << "Opening settings..." << std::endl;
    inMenu = false;
    inSettingsMenu = true;
  } else if (action == "quitGame") {
    std::cout << "Quitting game..." << std::endl;
    isRunning = false;
  }
}

void GameClient::handleGameModeAction(const std::string &action) {
  SoundSource menuSound;
  menuSound.pathfile = "res/Sound FX/hoverButton.wav";
  menuSound.name = "menu_click";
  menuSound.volume = 0.7f;
  audioHandler.Update_currentSound(menuSound);
  if (audioHandler.LoadSound()) {
    audioHandler.PlaySound();
  }

  if (action == "startSolo") {
    std::cout << "Starting solo game..." << std::endl;
    multiplayerMode = false;
    inGameModeMenu = false;
    const float DT = 0.5f;
    const int MAX_FRAMES = 50;
    int frame = 0;
    bool levelComplete = false;
    std::cout << " Frame " << frame << " Time: " << (frame * DT) << "s"
              << std::endl;

    registry.createEntity();
    player.id = 0;
    registry.addComponent(player.id, Position{200.0f, 200.0f});
    registry.addComponent(player.id, Velocity{0.0f, 0.0f});
    registry.addComponent(player.id, Health{100, 100});
    registry.addComponent(player.id, CollisionBox{32.0f, 32.0f});
    registry.addComponent(player.id, Team{Team::Individu::PLAYER});
    auto players = displayHandler.GetSpritesPlayer();
    sceneHandler.getcurrentGame().addEntity("Player", players[0], player.id);
    printf("Created player entity with ID %d\n", player.id);
    scene.loadScene(registry, "firstScene", spawn);
  } else if (action == "startMultiplayer") {
    std::cout << "Opening multiplayer connection form..." << std::endl;
    inGameModeMenu = false;
    inMultiplayerForm = true;
  } else if (action == "backToMain") {
    std::cout << "Returning to main menu..." << std::endl;
    inGameModeMenu = false;
    inMenu = true;
  }
}

void GameClient::handleMultiplayerFormAction(const std::string &action) {
  SoundSource menuSound;
  menuSound.pathfile = "res/Sound FX/hoverButton.wav";
  menuSound.name = "menu_click";
  menuSound.volume = 0.7f;
  audioHandler.Update_currentSound(menuSound);
  if (audioHandler.LoadSound()) {
    audioHandler.PlaySound();
  }

  if (action == "connect") {
    std::string newPlayerName =
        sceneHandler.getCurrentMultiplayerForm().GetPlayerName();
    std::string newServerIP =
        sceneHandler.getCurrentMultiplayerForm().GetServerIP();
    int newServerPort =
        sceneHandler.getCurrentMultiplayerForm().GetServerPort();

    std::cout << "Player " << newPlayerName
              << " connecting to server: " << newServerIP << ":"
              << newServerPort << std::endl;

    playerName = newPlayerName;
    serverIP = newServerIP;
    serverPort = newServerPort;

    connectToServer(serverIP, serverPort);

    multiplayerMode = true;
    inMultiplayerForm = false;
    sceneHandler.getCurrentRoomMenu().SetServerInfo(serverIP, serverPort);
    inRoomMenu = true;

  } else if (action == "back") {
    std::cout << "Returning to game mode menu..." << std::endl;
    inMultiplayerForm = false;
    inGameModeMenu = true;
  }
}

void GameClient::connectToServer(const std::string &ip, int port) {
  std::cout << "Establishing connection to " << ip << ":" << port << std::endl;

  sceneHandler.getCurrentMultiplayerForm().SetConnectionStatus(
      STATUS_CONNECTING, "");

  try {
    if (networkClient) {
      networkClient->stop();
    }

    networkClient = std::make_unique<NetworkClient>(ip, port);

    networkClient->set_player_connect_callback(
        [this](const PlayerConnectPacket &packet) {
          this->handlePlayerConnect(packet);
        });
    networkClient->set_player_join_callback(
        [this](const PlayerJoinPacket &packet) {
          this->handlePlayerJoin(packet);
        });
    networkClient->set_player_disconnect_callback(
        [this](const PlayerDisconnectPacket &packet) {
          this->handlePlayerDisconnect(packet);
        });
    networkClient->set_game_start_callback(
        [this](const GameStartPacket &packet) {
          this->handleGameStart(packet);
        });
    networkClient->set_game_ai_callback(
        [this](const GameAiPacket &packet) { this->handleGameAi(packet); });
    networkClient->set_game_collision_callback(
        [this](const GameCollisionPacket &packet) {
          this->handleGameCollision(packet);
        });
    networkClient->set_player_has_move_callback(
        [this](const PlayerHasMovePacket &packet) {
          this->handlePlayerHasMove(packet);
        });
    networkClient->set_bullet_callback(
        [this](const BulletPacket &packet) { this->handleBullet(packet); });
    networkClient->set_room_list_callback(
        [this](const RoomListPacket &packet) { this->handleRoomList(packet); });
    networkClient->set_room_players_callback(
        [this](const Sendplayer_in_room_Packet &packet) {
          try {
            if (!inRoomLobby)
              return;
            if (packet.room_id != playerData.room_id) {
              if (!(pendingLobbyJoin && pendingLobbyRoomId == packet.room_id))
                return;
            }
            bool localReady = roomLobby.IsLocalPlayerReady();
            roomLobby.Cleanup();
            const char *base =
                reinterpret_cast<const char *>(packet.player_names);
            for (uint32_t i = 0; i < packet.num_players && i < 4; ++i) {
              char namebuf[17];
              const char *src = base + (i * 16);
              std::memcpy(namebuf, src, 16);
              namebuf[16] = '\0';
              std::string name(namebuf);
              roomLobby.AddPlayer(name, static_cast<int>(i));
            }
            roomLobby.SetPlayerReady(static_cast<int>(playerData.player_id),
                                     localReady);
          } catch (...) {
          }
        });

    networkClient->set_chat_broadcast_callback(
        [this](const MessagePacket &packet) {
          std::string message(reinterpret_cast<const char *>(packet.message));
          std::cout << "[CHAT] Message reçu du joueur " << packet.player_id
                    << ": " << message << std::endl;

          std::string senderName;
          auto it = playerNames.find(packet.player_id);
          if (it != playerNames.end()) {
            senderName = it->second;
          } else {
            senderName = "Player " + std::to_string(packet.player_id);
          }

          Color messageColor = (packet.player_id == player.id)
                                   ? Color{100, 255, 100, 255}
                                   : Color{200, 200, 255, 255};
          gameChat.AddMessage(senderName, message, messageColor);
        });
    gameChat.SetSendMessageCallback([this](const std::string &message) {
      if (networkClient) {
        networkClient->send_chat_message(playerData.room_id, message);
        std::cout << "[CHAT] Envoi du message: " << message << std::endl;
      }
    });

    networkClient->initialize(playerName);

    std::cout << "Successfully connected to " << ip << ":" << port << std::endl;
    std::cout << "Ready to play!" << std::endl;

  } catch (const std::exception &e) {
    std::cerr << "Failed to connect to server: " << e.what() << std::endl;
    std::cerr << "Game will continue in offline mode." << std::endl;
  }
}

void GameClient::handleSettingsAction(const std::string &action) {
  SoundSource menuSound;
  menuSound.pathfile = "res/Sound FX/hoverButton.wav";
  menuSound.name = "menu_click";
  menuSound.volume = 0.7f;
  audioHandler.Update_currentSound(menuSound);
  if (audioHandler.LoadSound()) {
    audioHandler.PlaySound();
  }

  if (action == "showHelp") {
    std::cout << "Opening help menu..." << std::endl;
    inSettingsMenu = false;
    inHelpMenu = true;
  } else if (action == "backToMain") {
    std::cout << "Returning to main menu..." << std::endl;
    inSettingsMenu = false;
    inMenu = true;
  }
}

void GameClient::handleHelpAction(const std::string &action) {
  SoundSource menuSound;
  menuSound.pathfile = "res/Sound FX/hoverButton.wav";
  menuSound.name = "menu_click";
  menuSound.volume = 0.7f;
  audioHandler.Update_currentSound(menuSound);
  if (audioHandler.LoadSound()) {
    audioHandler.PlaySound();
  }

  if (action == "backToSettings") {
    std::cout << "Returning to settings menu..." << std::endl;
    inHelpMenu = false;
    inSettingsMenu = true;
  }
}

void GameClient::handleRoomAction(const std::string &action) {
  SoundSource menuSound;
  menuSound.pathfile = "res/Sound FX/hoverButton.wav";
  menuSound.name = "menu_click";
  menuSound.volume = 0.7f;
  audioHandler.Update_currentSound(menuSound);
  if (audioHandler.LoadSound()) {
    audioHandler.PlaySound();
  }

  if (action == "createRoom") {
    std::cout << "Create Room pressed: sending PlayerConnectPacket + "
                 "RoomListRequestPacket"
              << std::endl;
    if (networkClient) {
      networkClient->send_connect_packet(playerName);
      networkClient->send_get_room_list();
    }
    inRoomMenu = false;
    inRoomListMenu = true;
    try {
      sceneHandler.getCurrentRoomListMenu().ClearRooms();
    } catch (...) {
    }

  } else if (action == "joinRoom") {
    std::cout << "Join Room pressed: sending PlayerConnectPacket and opening "
                 "lobby view (server assigns room)"
              << std::endl;
    if (networkClient) {
      networkClient->send_connect_packet(playerName);
    }
    inRoomMenu = false;
    inRoomListMenu = false;
    if (!inRoomLobby) {
      roomLobby.Initialize();
    }
    roomLobby.SetRoomInfo(std::string("Room ") +
                              std::to_string(playerData.room_id),
                          static_cast<int>(playerData.room_id), 4);
    roomLobby.SetIsRoomOwner(false);
    roomLobby.AddPlayer(playerName, static_cast<int>(playerData.player_id));
    inRoomLobby = true;

  } else if (action == "disconnect") {
    std::cout << "Disconnecting from server..." << std::endl;

    if (networkClient) {
      networkClient->stop();
      networkClient.reset();
    }

    multiplayerMode = false;
    gameStarted = false;
    inRoomMenu = false;
    inMenu = true;

    if (audioHandler.LoadMusic("res/music/startsong.mp3", "Menu Music")) {
      audioHandler.PlayMusic();
      std::cout << "Musique des menus relancée" << std::endl;
    }

    std::cout << "Disconnected and returned to main menu" << std::endl;
  }
}

void GameClient::handleRoomLobbyAction(const std::string &action) {
  SoundSource menuSound;
  menuSound.pathfile = "res/Sound FX/hoverButton.wav";
  menuSound.name = "menu_click";
  menuSound.volume = 0.7f;
  audioHandler.Update_currentSound(menuSound);
  if (audioHandler.LoadSound()) {
    audioHandler.PlaySound();
  }

  if (action == "setReady") {
    std::cout << "Player marked as ready" << std::endl;
    roomLobby.SetPlayerReady(static_cast<int>(playerData.player_id), true);

  } else if (action == "setUnready") {
    std::cout << "Player marked as not ready" << std::endl;
    roomLobby.SetPlayerReady(static_cast<int>(playerData.player_id), false);

  } else if (action == "startGame") {
    std::cout << "Starting game from lobby..." << std::endl;
    createPlayerEntity();

    gameStarted = true;
    inRoomLobby = false;
  }
}

bool GameClient::shouldQuitGame() const {
  return displayHandler.ShouldCloseIgnoreEscapeOnly();
}

void GameClient::Run() {
  std::cout << "Game client running..." << std::endl;
  printf("Displaying game...\n");

  while (isRunning && !shouldQuitGame()) {
    Update();

    DisplayGame();
  }
}

void GameClient::Update() {
  static int updateCount = 0;
  updateCount++;
  if (updateCount <= 5) {
    std::cout << "Update #" << updateCount << " - inMenu: " << inMenu
              << std::endl;
  }

  if (pendingDisconnect) {
    std::cout << "[PENDING DISCONNECT] Traitement de la déconnexion différée..."
              << std::endl;

    gameOver = false;
    gameStarted = false;
    multiplayerMode = false;
    inMenu = true;
    inRoomLobby = false;

    playerPreviousPositions.clear();
    playerNames.clear();

    auto allGameEntities = sceneHandler.getcurrentGame().getallEntity();
    for (const auto &entity : allGameEntities) {
      sceneHandler.getcurrentGame().removeEntity(entity.second);
    }

    if (networkClient) {
      std::cout << "[DISCONNECT] Envoi du paquet de déconnexion..."
                << std::endl;
      networkClient->send_disconnect_packet();
      std::this_thread::sleep_for(std::chrono::milliseconds(100));
      networkClient->stop();
      networkClient.reset();
      std::cout << "[DISCONNECT] Déconnecté du serveur" << std::endl;
    }

    if (audioHandler.LoadMusic("res/music/startsong.mp3", "Menu Music")) {
      audioHandler.PlayMusic();
      std::cout << "Musique des menus relancée" << std::endl;
    }

    pendingDisconnect = false;
    std::cout << "✅ Retour au menu principal terminé" << std::endl;
    return;
  }

  displayHandler.Update(GetFrameTime());
  inputHandler.Update();
  audioHandler.Update();

  if (networkClient) {
    networkClient->process_packets();
  }

  if (multiplayerMode && networkClient &&
      inputHandler.IsKeyPressed(InputKey::KEY_SPACE)) {
    std::cout << "SPACE pressed: sending CreateBulletPacket (playerId="
              << player.id << ", room=" << playerData.room_id << ")"
              << std::endl;
    networkClient->send_create_bullet_packet(playerData.room_id);

    SoundSource shootSound;
    shootSound.pathfile = "res/Sound FX/shot 1.wav";
    shootSound.name = "shoot";
    shootSound.volume = 0.5f;
    audioHandler.Update_currentSound(shootSound);
    if (audioHandler.LoadSound()) {
      audioHandler.PlaySound();
    }
  }

  if (inMenu) {
    sceneHandler.UpdateMenu(inputHandler);
    if (sceneHandler.getcurrentMenu().IsActionSelected()) {
      handleMenuAction(sceneHandler.getcurrentMenu().GetSelectedAction());
      sceneHandler.getcurrentMenu().ResetSelection();
    }
  } else if (inGameModeMenu) {
    sceneHandler.UpdateGameModeMenu(inputHandler);
    if (sceneHandler.getCurrentGameModeMenu().IsActionSelected()) {
      handleGameModeAction(
          sceneHandler.getCurrentGameModeMenu().GetSelectedAction());
      sceneHandler.getCurrentGameModeMenu().ResetSelection();
    }
  } else if (inMultiplayerForm) {
    sceneHandler.UpdateMultiplayerForm(inputHandler);
    if (sceneHandler.getCurrentMultiplayerForm().IsActionSelected()) {
      handleMultiplayerFormAction(
          sceneHandler.getCurrentMultiplayerForm().GetSelectedAction());
      sceneHandler.getCurrentMultiplayerForm().ResetSelection();
    }
  } else if (inSettingsMenu) {
    sceneHandler.UpdateSettingsMenu(inputHandler);
    if (sceneHandler.getCurrentSettingsMenu().IsActionSelected()) {
      handleSettingsAction(
          sceneHandler.getCurrentSettingsMenu().GetSelectedAction());
      sceneHandler.getCurrentSettingsMenu().ResetSelection();
    }
  } else if (inHelpMenu) {
    sceneHandler.UpdateHelpMenu(inputHandler);
    if (sceneHandler.getCurrentHelpMenu().IsActionSelected()) {
      handleHelpAction(sceneHandler.getCurrentHelpMenu().GetSelectedAction());
      sceneHandler.getCurrentHelpMenu().ResetSelection();
    }
  } else if (inRoomMenu) {
    sceneHandler.UpdateRoomMenu(inputHandler);
    if (sceneHandler.getCurrentRoomMenu().IsActionSelected()) {
      handleRoomAction(sceneHandler.getCurrentRoomMenu().GetSelectedAction());
      sceneHandler.getCurrentRoomMenu().ResetSelection();
    }
  } else if (inRoomListMenu) {
    sceneHandler.UpdateRoomListMenu(inputHandler);
    if (sceneHandler.getCurrentRoomListMenu().IsActionSelected()) {
      handleRoomListAction(
          sceneHandler.getCurrentRoomListMenu().GetSelectedAction());
      sceneHandler.getCurrentRoomListMenu().ResetSelection();
    }
  } else if (inRoomLobby) {
    lobbyPlayersRefreshTimer += GetFrameTime();
    if (lobbyPlayersRefreshTimer >= 1.0f && networkClient) {
      uint32_t rid = (pendingLobbyJoin && pendingLobbyRoomId != 0)
                         ? pendingLobbyRoomId
                         : playerData.room_id;
      if (rid != 0) {
        networkClient->send_get_players_in_room(rid);
      }
      lobbyPlayersRefreshTimer = 0.0f;
    }

    roomLobby.Update(inputHandler);
    if (roomLobby.IsActionSelected()) {
      handleRoomLobbyAction(roomLobby.GetSelectedAction());
      roomLobby.ResetSelection();
    }
  } else {
    if (multiplayerMode) {
      if (inputHandler.IsKeyPressed(InputKey::KEY_T)) {
        if (!gameChat.IsVisible()) {
          gameChat.SetVisible(true);
          gameChat.SetInputActive(true);
          gameChat.SetJustActivated(true);
          std::cout << "Chat ouvert avec T" << std::endl;
          return;
        } else if (!gameChat.IsInputActive()) {
          gameChat.SetInputActive(true);
          gameChat.SetJustActivated(true);
          std::cout << "Chat input activé avec T" << std::endl;
          return;
        }
      }

      gameChat.Update(inputHandler);
      if (gameChat.IsInputActive()) {
        return;
      }

      if (inputHandler.IsKeyPressed(InputKey::KEY_ESCAPE)) {
        if (gameOver) {
          std::cout << "========================================" << std::endl;
          std::cout << "Retour au menu principal depuis Game Over" << std::endl;
          std::cout << "========================================" << std::endl;

          try {
            SoundSource clickSound;
            clickSound.pathfile = "res/Sound FX/hoverButton.wav";
            clickSound.name = "menu_back";
            audioHandler.Update_currentSound(clickSound);
            if (audioHandler.LoadSound()) {
              audioHandler.PlaySound();
              std::cout << "[AUDIO] Son de retour au menu joué (hover)"
                        << std::endl;
            }
          } catch (...) {
            std::cout << "[AUDIO] Son de retour au menu non disponible"
                      << std::endl;
          }

          gameOver = false;
          gameStarted = false;
          multiplayerMode = false;
          inMenu = true;
          inRoomLobby = false;

          playerPreviousPositions.clear();

          auto allEntities = sceneHandler.getcurrentGame().getallEntity();
          for (const auto &entity : allEntities) {
            sceneHandler.getcurrentGame().removeEntity(entity.second);
          }

          if (networkClient) {
            std::cout << "Déconnexion du serveur..." << std::endl;
            networkClient->send_disconnect_packet();
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            networkClient->stop();
            networkClient.reset();
          }

          if (audioHandler.LoadMusic("res/music/startsong.mp3", "Menu Music")) {
            audioHandler.PlayMusic();
            std::cout << "🎵 Musique des menus relancée" << std::endl;
          }

          std::cout << "Retour au menu principal terminé" << std::endl;
          return;
        }
      }

      if (multiplayerMode && gameChat.IsVisible()) {
        gameChat.SetVisible(false);
        gameChat.SetInputActive(false);
        std::cout << "Chat fermé avec ESC" << std::endl;
        return;
      }
      std::cout << "ESC pressed but not quitting game" << std::endl;

      PlayerMove newMove = STOP;

      if (inputHandler.IsKeyHeld(InputKey::KEY_W) ||
          inputHandler.IsKeyHeld(InputKey::KEY_UP)) {
        newMove = UP;
      } else if (inputHandler.IsKeyHeld(InputKey::KEY_S) ||
                 inputHandler.IsKeyHeld(InputKey::KEY_DOWN)) {
        newMove = DOWN;
      } else if (inputHandler.IsKeyHeld(InputKey::KEY_A) ||
                 inputHandler.IsKeyHeld(InputKey::KEY_LEFT)) {
        newMove = LEFT;
      } else if (inputHandler.IsKeyHeld(InputKey::KEY_D) ||
                 inputHandler.IsKeyHeld(InputKey::KEY_RIGHT)) {
        newMove = RIGHT;
      }
      if (newMove != currentMove) {
        sequenceNumber++;
        executeMove(newMove, sequenceNumber);

        if (networkClient) {
          networkClient->send_player_move_packet(newMove, sequenceNumber);
        }
        currentMove = newMove;
      }

      ecsManager.updateMovement(GetFrameTime());
      updatePlayerGraphics();
    } else {
      std::this_thread::sleep_for(std::chrono::seconds(1 / 30));
      const float DT = 0.5f;
      int c = 0;
      spawn.update(registry, DT);
      ai.update(registry, DT);
      for (auto _entity : registry.getEntitiesWith<Team>()) {
        c++;
        auto &team = registry.getComponent<Team>(_entity);
        if (team.ind == Team::Individu::PLAYER &&
            registry.hasComponent<Health>(_entity)) {
          Velocity &vel = registry.getComponent<Velocity>(_entity);
          if (inputHandler.IsKeyHeld(InputKey::KEY_W) ||
              inputHandler.IsKeyHeld(InputKey::KEY_UP))
            vel.dy = -30.0f;
          else if (inputHandler.IsKeyHeld(InputKey::KEY_S) ||
                   inputHandler.IsKeyHeld(InputKey::KEY_DOWN))
            vel.dy = 30.0f;
          else if (inputHandler.IsKeyHeld(InputKey::KEY_A) ||
                   inputHandler.IsKeyHeld(InputKey::KEY_LEFT))
            vel.dx = -30.0f;
          else if (inputHandler.IsKeyHeld(InputKey::KEY_D) ||
                   inputHandler.IsKeyHeld(InputKey::KEY_RIGHT))
            vel.dx = 30.0f;
          else {
            vel.dx = 0.0f;
            vel.dy = 0.0f;
          }
          move.update(registry, DT);
        }
        Position &pos = registry.getComponent<Position>(0);
        sceneHandler.getcurrentGame().Update(
            GetFrameTime(), 0,
            Vector2{static_cast<float>(pos.x), static_cast<float>(pos.y)});
      }
      if (inputHandler.IsKeyHeld(InputKey::KEY_SPACE)) {
        Entity shooter = player.id;
        shooting_enemy.shoot(registry, shooter);
      }
      bool collide = false;
      auto collisionPairs = collision.update(registry, DT);
      for (auto _pair : collisionPairs) {
        auto team_a = registry.getComponent<Team>(_pair.first);
        auto team_b = registry.getComponent<Team>(_pair.second);
        std::cout << "COLLISION between" << _pair.first << " and "
                  << _pair.second << std::endl;

        if (!registry.hasComponent<Health>(_pair.first) &&
            team_a.ind == Team::Individu::PLAYER) {
          std::cout << "[WHOSE BULLET] Entity with id :" << _pair.first
                    << " is a player bullet" << std::endl;
          for (int j = 0; j < registry.bullets[0].size(); j++) {
            if (registry.bullets[0][j] == _pair.first) {
              player.score += 50;
              registry.bullets[0].erase(registry.bullets[0].begin() + j);
            }
          }
        } else if (!registry.hasComponent<Health>(_pair.second) &&
                   team_a.ind == Team::Individu::PLAYER) {
          for (int j = 0; j < registry.bullets[0].size(); j++) {
            if (registry.bullets[0][j] == _pair.second) {
              player.score += 50;
              std::cout << "[PLAYER] Bullet packet has made collision"
                        << std::endl;
            }
          }
        }
        sceneHandler.getcurrentGame().removeEntity(_pair.first);
        sceneHandler.getcurrentGame().removeEntity(_pair.second);
        if (_pair.second == 0 || _pair.first == 0) {
          inMenu = true;
        }
      }
      destroy.update(registry, DT);
      auto entities = registry.getEntitiesWith<Team>();
      for (Entity e : entities) {
        auto &team = registry.getComponent<Team>(e);
        if (team.ind == Team::Individu::PLAYER &&
            !registry.hasComponent<Health>(e)) {
          char _msg[sizeof(BulletPacket)];
          BulletPacket _bullet_packet;
          _bullet_packet.header.magic = MAGIC_NUMBER;
          _bullet_packet.header.version = PROTOCOL_VERSION;
          _bullet_packet.header.type = BULLET_MOVE;
          std::cout << "[INFO] bULLET WITH id:" << e << "moved again"
                    << std::endl;
          _bullet_packet.is_player_bullet = 1;
          _bullet_packet.bullet_id = e;
          _bullet_packet.player_id = e;
          auto &pos = registry.getComponent<Position>(e);
          auto &vel = registry.getComponent<Velocity>(e);
          int id = e;
          auto allEntities = sceneHandler.getcurrentGame().getallEntity();
          auto entity = find_if(allEntities.begin(), allEntities.end(),
                                [id](const auto &p) { return p.second == id; });
          if (entity != allEntities.end()) {
            sceneHandler.getcurrentGame().Update(GetFrameTime(), id,
                                                 Vector2{pos.x, pos.y});
            // std::cout << "Updated bullet ID " << id << " position to (" <<
            // packet.position_x << ", " << packet.position_y << ")" <<
            // std::endl;
          } else {
            SpriteComponent bulletSprite;
            auto fire = displayHandler.GetSpritesFire();
            if (!fire.empty()) {
              if (_bullet_packet.is_player_bullet && fire.size() > 1) {
                bulletSprite = fire[1];
              } else {
                bulletSprite = fire[0];
              }
              std::cout << "display rect: " << bulletSprite.sourceRect.x << ", "
                        << bulletSprite.sourceRect.y << ", "
                        << bulletSprite.sourceRect.width << ", "
                        << bulletSprite.sourceRect.height << std::endl;
              bulletSprite.scale = 0.8f;
              std::cout << "Created bullet ID " << id << " using fire sprite"
                        << std::endl;
            } else {
              bulletSprite.visible = true;
              bulletSprite.scale = 0.5f;
              bulletSprite.tint = {255, 255, 0, 255};
              std::cout << "Created bullet ID " << id
                        << " using fallback sprite" << std::endl;
            }

            bulletSprite.position = {pos.x, pos.y};
            bulletSprite.type = "Fire";

            sceneHandler.getcurrentGame().addEntity("Fire", bulletSprite, id);
            std::cout << "Fire entity created successfully!" << std::endl;
          }

        } else if (team.ind == Team::Individu::ENEMY) {
          // Distinguer missiles des ennemis
          // exit(24);
          if (registry.hasComponent<Damage>(e) &&
              !registry.hasComponent<Health>(e)) {
            char _msg[sizeof(BulletPacket)];
            BulletPacket _bullet_packet;
            _bullet_packet.header.magic = MAGIC_NUMBER;
            _bullet_packet.header.version = PROTOCOL_VERSION;
            _bullet_packet.header.type = BULLET_MOVE;
            std::cout << "[INFO] bULLET WITH id:" << e << "moved again"
                      << std::endl;
            _bullet_packet.is_player_bullet = 0;
            _bullet_packet.bullet_id = e;
            _bullet_packet.player_id = e;
            auto &pos = registry.getComponent<Position>(e);
            auto &vel = registry.getComponent<Velocity>(e);
            _bullet_packet.position_x = pos.x;
            _bullet_packet.position_y = pos.y;
            _bullet_packet.direction_x = vel.dx;
            _bullet_packet.direction_y = vel.dy;
            int id = e;
            auto allEntities = sceneHandler.getcurrentGame().getallEntity();
            auto entity =
                find_if(allEntities.begin(), allEntities.end(),
                        [id](const auto &p) { return p.second == id; });
            if (entity != allEntities.end()) {
              sceneHandler.getcurrentGame().Update(GetFrameTime(), id,
                                                   Vector2{pos.x, pos.y});
              // std::cout << "Updated bullet ID " << id << " position to (" <<
              // packet.position_x << ", " << packet.position_y << ")" <<
              // std::endl;
            } else {
              SpriteComponent bulletSprite;
              auto fire = displayHandler.GetSpritesFire();
              if (!fire.empty()) {
                if (_bullet_packet.is_player_bullet && fire.size() > 1) {
                  bulletSprite = fire[1];
                } else {
                  bulletSprite = fire[0];
                }
                std::cout << "display rect: " << bulletSprite.sourceRect.x
                          << ", " << bulletSprite.sourceRect.y << ", "
                          << bulletSprite.sourceRect.width << ", "
                          << bulletSprite.sourceRect.height << std::endl;
                bulletSprite.scale = 0.8f;
                std::cout << "Created bullet ID " << id << " using fire sprite"
                          << std::endl;
              } else {
                bulletSprite.visible = true;
                bulletSprite.scale = 0.5f;
                bulletSprite.tint = {255, 255, 0, 255};
                std::cout << "Created bullet ID " << id
                          << " using fallback sprite" << std::endl;
              }

              bulletSprite.position = {pos.x, pos.y};
              bulletSprite.type = "Fire";

              sceneHandler.getcurrentGame().addEntity("Fire", bulletSprite, id);
              std::cout << "Fire entity created successfully!" << std::endl;
            }
            // std::cout << "Sending bullet: " << "x" <<
            // _bullet_packet.position_x << " y: " << _bullet_packet.position_y
            // << " move packet for entity " << e << std::endl;
          } else {
            if (registry.hasComponent<Health>(e)) {
              char _msg[sizeof(GameAiPacket)];
              GameAiPacket _ai_packet;
              _ai_packet.header.magic = MAGIC_NUMBER;
              _ai_packet.header.version = PROTOCOL_VERSION;
              _ai_packet.header.type = AI_HAS_MOVE;
              _ai_packet.entity_id = e;
              if (std::string(spawn.getEnemyTypeName(
                      registry.getComponent<Ai>(e).type)) == "BasicEnemy") {
                _ai_packet.entity_type = WHICH_AI::BASIC_ENEMY;
                std::cout << "[INFO] Basic enemy with id: " << e
                          << " moved again" << std::endl;
              } else if (std::string(spawn.getEnemyTypeName(
                             registry.getComponent<Ai>(e).type)) ==
                         "ShootingEnemy") {
                _ai_packet.entity_type = WHICH_AI::SHOOTING_ENEMY;
                std::cout << "[INFO] Shooting enemy with id: " << e
                          << " moved again" << std::endl;
              } else if (std::string(spawn.getEnemyTypeName(
                             registry.getComponent<Ai>(e).type)) ==
                         "FollowingEnemy") {
                _ai_packet.entity_type = WHICH_AI::FOLLOWING_ENEMY;
                std::cout << "[INFO] Following enemy with id: " << e
                          << " moved again" << std::endl;
              }
              auto &pos = registry.getComponent<Position>(e);
              auto &vel = registry.getComponent<Velocity>(e);
              _ai_packet.position_x = pos.x;
              _ai_packet.position_y = pos.y;
              _ai_packet.velocity_x = vel.dx;
              _ai_packet.velocity_y = vel.dy;
              std::memcpy(_msg, &_ai_packet, sizeof(_ai_packet));
              int id = e;
              auto allEntities = sceneHandler.getcurrentGame().getallEntity();
              auto entity =
                  find_if(allEntities.begin(), allEntities.end(),
                          [id](const auto &p) { return p.second == id; });

              std::string subtype;
              switch (_ai_packet.entity_type) {
              case 0:
                subtype = "BASIC_ENEMY";
                break;
              case 1:
                subtype = "FIRING_ENEMY";
                break;
              case 2:
                subtype = "FOLLOWING_ENEMY";
                break;
              default:
                subtype = "BASIC_ENEMY";
                break;
              }

              auto pickEnemySprite = [&](const std::string &wantedType) {
                SpriteComponent s;
                auto enemies = displayHandler.GetSpritesEnemy();
                bool matched = false;
                if (!enemies.empty()) {
                  for (const auto &e : enemies) {
                    if (e.type == wantedType &&
                        e.layer == sceneHandler.getGameCount()) {
                      s = e;
                      matched = true;
                      break;
                    }
                  }
                  if (!matched) {
                    s = enemies[0];
                  }
                } else {
                  s = SpriteComponent();
                  s.visible = true;
                  s.tint = {255, 255, 255, 255};
                }
                s.type = wantedType;
                return s;
              };

              if (entity != allEntities.end()) {
                // std::cout << "Updating entity ID: " << id << " to position ("
                // << packet.position_x << ", " << packet.position_y << ")" <<
                // std::endl;
                sceneHandler.getcurrentGame().Update(GetFrameTime(), id,
                                                     Vector2{pos.x, pos.y});
              } else {
                SpriteComponent sprite = pickEnemySprite(subtype);
                sprite.position = {pos.x, pos.y};
                sceneHandler.getcurrentGame().addEntity("Enemy", sprite, id);
              }
            }
          }
        }

        destroy.update(registry, DT);
      }
    }
  }
}

void GameClient::Cleanup() {
  std::cout << "Game client shutting down." << std::endl;
  if (networkClient) {
    networkClient->stop();
  }
  audioHandler.Cleanup();
  inputHandler.Cleanup();
  displayHandler.Cleanup();
  sceneHandler.Cleanup();
  gameChat.Cleanup();
}

void GameClient::DisplayGame() {
  BeginDrawing();
  ClearBackground(BLACK);

  if (inMenu) {
    sceneHandler.RenderMenu(displayHandler);
  } else if (inGameModeMenu) {
    sceneHandler.RenderGameModeMenu(displayHandler);
  } else if (inMultiplayerForm) {
    sceneHandler.RenderMultiplayerForm(displayHandler);
  } else if (inSettingsMenu) {
    sceneHandler.RenderSettingsMenu(displayHandler);
  } else if (inHelpMenu) {
    sceneHandler.RenderHelpMenu(displayHandler);
  } else if (inRoomMenu) {
    sceneHandler.RenderRoomMenu(displayHandler);
  } else if (inRoomListMenu) {
    sceneHandler.RenderRoomListMenu(displayHandler);
  } else if (inRoomLobby) {
    roomLobby.Render(displayHandler);
  } else {
    float deltaTime = GetFrameTime();
    parallaxOffset += parallaxSpeed * deltaTime;

    SpriteComponent bg = sceneHandler.getcurrentGame().getBackground();
    displayHandler.DrawParallaxBackground(bg, parallaxOffset, 1.0f);
    SpriteComponent stars = sceneHandler.getcurrentGame().getStars();
    displayHandler.DrawParallaxBackground(stars, parallaxOffset, 1.25f);
    auto entities = sceneHandler.getcurrentGame().getallEntity();
    static float lastPrint = 0;
    if (GetTime() - lastPrint > 3.0f) {
      std::cout << "=== PARALLAX DEBUG ===" << std::endl;
      std::cout << "Parallax offset: " << parallaxOffset << std::endl;
      std::cout << "Background: " << bg.textureName << " visible=" << bg.visible
                << std::endl;
      std::cout << "Entities: " << entities.size() << std::endl;
      std::cout << "======================" << std::endl;
      lastPrint = GetTime();
    }

    // Draw all game entities on top of background
    for (auto sprit : entities) {
      displayHandler.DrawSprite(sprit.first.sprite);
    }

    FontManager &fontManager = FontManager::getInstance();
    fontManager.Initialize(displayHandler);

    std::string scoreText = "Score: " + std::to_string(player.score);

    float screenWidth = static_cast<float>(GetScreenWidth());
    float scoreX = screenWidth - 250.0f;
    float scoreY = 20.0f;

    TextComponent scoreDisplay = fontManager.CreateText(
        scoreText, {scoreX, scoreY}, FontManager::PIXEL_REGULAR,
        FontManager::HUGE_SIZE, Color{255, 255, 255, 255});
    displayHandler.DrawText(scoreDisplay);

    if (multiplayerMode) {
      gameChat.Render(displayHandler);

      if (!gameChat.IsVisible()) {
        FontManager &fontManager = FontManager::getInstance();
        fontManager.Initialize(displayHandler);

        TextComponent chatIndicator = fontManager.CreateText(
            "Appuyez sur [T] pour ouvrir le chat",
            {20.0f, static_cast<float>(GetScreenHeight() - 30)},
            FontManager::PIXEL_REGULAR, FontManager::SMALL,
            Color{200, 200, 200, 150});
        displayHandler.DrawText(chatIndicator);
      }
    }

    if (gameOver) {
      FontManager &fontManager = FontManager::getInstance();
      fontManager.Initialize(displayHandler);

      int screenWidth = GetScreenWidth();
      int screenHeight = GetScreenHeight();

      DrawRectangle(0, 0, screenWidth, screenHeight, Color{0, 0, 0, 180});

      std::string gameOverStr = "GAME OVER";
      int gameOverFontSize = 80;
      int gameOverWidth = MeasureText(gameOverStr.c_str(), gameOverFontSize);
      TextComponent gameOverText = fontManager.CreateText(
          gameOverStr,
          {static_cast<float>((screenWidth - gameOverWidth) / 2),
           static_cast<float>(screenHeight / 2 - 80)},
          FontManager::PIXEL_REGULAR, FontManager::HUGE_SIZE,
          Color{255, 0, 0, 255});
      displayHandler.DrawText(gameOverText);

      std::string disconnectStr = "Un joueur s'est deconnecte";
      int disconnectFontSize = 40;
      int disconnectWidth =
          MeasureText(disconnectStr.c_str(), disconnectFontSize);
      TextComponent disconnectMsg = fontManager.CreateText(
          disconnectStr,
          {static_cast<float>((screenWidth - disconnectWidth) / 2),
           static_cast<float>(screenHeight / 2)},
          FontManager::PIXEL_REGULAR, FontManager::MEDIUM,
          Color{255, 255, 255, 255});
      displayHandler.DrawText(disconnectMsg);

      std::string instructionStr = "Appuyez sur ECHAP pour revenir au menu";
      int instructionFontSize = 25;
      int instructionWidth =
          MeasureText(instructionStr.c_str(), instructionFontSize);
      TextComponent instructionText = fontManager.CreateText(
          instructionStr,
          {static_cast<float>((screenWidth - instructionWidth) / 2),
           static_cast<float>(screenHeight / 2 + 60)},
          FontManager::PIXEL_REGULAR, FontManager::SMALL,
          Color{200, 200, 200, 255});
      displayHandler.DrawText(instructionText);
    }
  }
  EndDrawing();
}

void GameClient::updatePlayerGraphics() {
  try {
    if (ecsManager.hasPosition(playerEntity)) {
      float x, y;
      ecsManager.getPosition(playerEntity, x, y);

      player.position_x = x;
      player.position_y = y;

      sceneHandler.UpdateGame(GetFrameTime(), player.id, Vector2{x, y});
    }
  } catch (const std::exception &e) {
    std::cerr << "Error in updatePlayerGraphics: " << e.what() << std::endl;
  }
}

void GameClient::executeMove(PlayerMove move, uint32_t sequence) {
  std::cout << "Executing move " << move << " with sequence " << sequence
            << std::endl;

  float current_x, current_y;
  ecsManager.getPosition(playerEntity, current_x, current_y);

  if (ecsManager.hasVelocity(playerEntity)) {
    if (move == UP)
      ecsManager.setVelocity(playerEntity, 0.0f, -30.0f);
    else if (move == DOWN)
      ecsManager.setVelocity(playerEntity, 0.0f, 30.0f);
    else if (move == LEFT)
      ecsManager.setVelocity(playerEntity, -30.0f, 0.0f);
    else if (move == RIGHT)
      ecsManager.setVelocity(playerEntity, 30.0f, 0.0f);
    else if (move == STOP) {
      ecsManager.setVelocity(playerEntity, 0.0f, 0.0f);
    }
  }

  PendingMove pendingMove;
  pendingMove.sequence = sequence;
  pendingMove.move = move;
  pendingMove.timestamp = GetTime();
  pendingMove.predicted_x = current_x;
  pendingMove.predicted_y = current_y;

  pendingMoves.push_back(pendingMove);

  if (pendingMoves.size() > 10) {
    pendingMoves.erase(pendingMoves.begin());
  }
}

void GameClient::validateServerMove(const PlayerHasMovePacket &packet,
                                    float server_x, float server_y) {
  // std::cout << "Validating server move with sequence " << packet.header.seq
  // << std::endl;

  auto it = std::find_if(pendingMoves.begin(), pendingMoves.end(),
                         [&packet](const PendingMove &move) {
                           return move.sequence == packet.header.seq;
                         });

  if (it != pendingMoves.end()) {
    float tolerance = 5.0f; // Tolérance de 5 pixels
    float current_x, current_y;
    ecsManager.getPosition(playerEntity, current_x, current_y);

    float diff_x = std::abs(current_x - server_x);
    float diff_y = std::abs(current_y - server_y);

    if (diff_x > tolerance || diff_y > tolerance) {
      std::cout << "Position mismatch! Reconciling..." << std::endl;
      std::cout << "Client: (" << current_x << ", " << current_y << ")"
                << std::endl;
      std::cout << "Server: (" << server_x << ", " << server_y << ")"
                << std::endl;

      reconcilePosition(server_x, server_y, packet.header.seq);
    } else {
      std::cout << "Position validated successfully!" << std::endl;
    }

    pendingMoves.erase(pendingMoves.begin(), it + 1);
  } else {
    if (packet.header.seq > 0) {
      // std::cout << "Warning: Received validation for unknown sequence " <<
      // packet.header.seq << std::endl;
    }
  }
}

void GameClient::reconcilePosition(float server_x, float server_y,
                                   uint32_t server_sequence) {
  ecsManager.setPosition(playerEntity, server_x, server_y);

  for (const auto &pendingMove : pendingMoves) {
    if (pendingMove.sequence > server_sequence) {
      if (ecsManager.hasVelocity(playerEntity)) {
        if (pendingMove.move == UP)
          ecsManager.setVelocity(playerEntity, 0.0f, -30.0f);
        else if (pendingMove.move == DOWN)
          ecsManager.setVelocity(playerEntity, 0.0f, 30.0f);
        else if (pendingMove.move == LEFT)
          ecsManager.setVelocity(playerEntity, -30.0f, 0.0f);
        else if (pendingMove.move == RIGHT)
          ecsManager.setVelocity(playerEntity, 30.0f, 0.0f);
        else if (pendingMove.move == STOP) {
          ecsManager.setVelocity(playerEntity, 0.0f, 0.0f);
        }
      }

      float elapsed = GetTime() - pendingMove.timestamp;
      ecsManager.updateMovement(elapsed);
    }
  }
}
