/*
** EPITECH PROJECT, 2025
** G-CPP-500-COT-5-1-rtype-20 [WSL: Ubuntu-24.04]
** File description:
** scenes
*/


#ifndef SCENES_HPP
#define SCENES_HPP

#include <iostream>
#include <string>
#include <chrono>
#include <thread>
#include <memory>
#include <vector>
#include "Display_Handler.hpp"
#include "Input_Handler.hpp"
#include "Audio_Handler.hpp"
#include "NetworkClient.hpp"
#include "Game.hpp"
#include "menu.hpp"
#include "GameModeMenu.hpp"
#include "MultiplayerForm.hpp"
#include "SettingsMenu.hpp"
#include "HelpMenu.hpp"
#include "RoomMenu.hpp"
#include "RoomListMenu.hpp"

class Scene {
    public:
        Scene() {};                             // default ctor
        Scene(SpriteComponent Background);   // existing ctor
        ~Scene();
        void Initialize();
        void Update();
        void Cleanup();
        void setGame(Game game) { this->game = game; }
        int getGameCount() const { return gameCount; }
        void setGameCount(int count) { gameCount = count; }
        // void addMenu(std::unique_ptr<Menu> menu);
        void setcurrentMenu(int id) { currentMenu = id; }
        void addGame(std::unique_ptr<Game> game);
        void addMenu(std::unique_ptr<Menu> menu);
        void addGameModeMenu(std::unique_ptr<GameModeMenu> gameModeMenu);
        void addMultiplayerForm(std::unique_ptr<MultiplayerForm> multiplayerForm);
        void addSettingsMenu(std::unique_ptr<SettingsMenu> settingsMenu);
        void addHelpMenu(std::unique_ptr<HelpMenu> helpMenu);
        void addRoomMenu(std::unique_ptr<RoomMenu> roomMenu);
        void addRoomListMenu(std::unique_ptr<RoomListMenu> roomListMenu);
        // void setcurrentMenu(int id) { currentMenu = id; }
        void setcurrentGame(int id) { currentGame = id; }
        void setGameOrScene(bool value) { game_or_scene = value; }
        bool getGameOrScene() { return game_or_scene; }
        Game& getcurrentGame();
        Menu& getcurrentMenu();
        GameModeMenu& getCurrentGameModeMenu();
        MultiplayerForm& getCurrentMultiplayerForm();
        SettingsMenu& getCurrentSettingsMenu();
        HelpMenu& getCurrentHelpMenu();
        RoomMenu& getCurrentRoomMenu();
        RoomListMenu& getCurrentRoomListMenu();
        void UpdateMenu(InputHandlerSystem& inputHandler);
        void UpdateGameModeMenu(InputHandlerSystem& inputHandler);
        void UpdateMultiplayerForm(InputHandlerSystem& inputHandler);
        void UpdateSettingsMenu(InputHandlerSystem& inputHandler);
        void UpdateHelpMenu(InputHandlerSystem& inputHandler);
        void UpdateRoomMenu(InputHandlerSystem& inputHandler);
        void UpdateRoomListMenu(InputHandlerSystem& inputHandler);
        void UpdateGame(float deltaTime, int id, Vector2 position);
        void RenderMenu(DisplayHandlerSystem& displayHandler);
        void RenderGameModeMenu(DisplayHandlerSystem& displayHandler);
        void RenderMultiplayerForm(DisplayHandlerSystem& displayHandler);
        void RenderSettingsMenu(DisplayHandlerSystem& displayHandler);
        void RenderHelpMenu(DisplayHandlerSystem& displayHandler);
        void RenderRoomMenu(DisplayHandlerSystem& displayHandler);
        void RenderRoomListMenu(DisplayHandlerSystem& displayHandler);
        void removeMenu(int id);

    private:
    // std::vector<std::unique_ptr<Menu>> all_menus;
    // void RenderMenu(DisplayHandlerSystem& displayHandler);
        // void RenderGameModeMenu(DisplayHandlerSystem& displayHandler);
        // void RenderMultiplayerForm(DisplayHandlerSystem& displayHandler);
        // void RenderSettingsMenu(DisplayHandlerSystem& displayHandler);
        // void RenderHelpMenu(DisplayHandlerSystem& displayHandler);
        // void RenderRoomMenu(DisplayHandlerSystem& displayHandler);
        // void removeMenu(int id);
        int currentGame = 0;
        int oldGame = 0;
        int currentMenu = 0;
        Game game;
        int currentGameModeMenu = 0;
        int currentMultiplayerForm = 0;
        int currentSettingsMenu = 0;
        int currentHelpMenu = 0;
        int currentRoomMenu = 0;
        int currentRoomListMenu = 0;
        bool game_or_scene = true;
        int gameCount = 1;
        std::vector<std::unique_ptr<Menu>> all_menus;
        std::vector<std::unique_ptr<GameModeMenu>> all_gameModeMenus;
        std::vector<std::unique_ptr<MultiplayerForm>> all_multiplayerForms;
        std::vector<std::unique_ptr<SettingsMenu>> all_settingsMenus;
        std::vector<std::unique_ptr<HelpMenu>> all_helpMenus;
        std::vector<std::unique_ptr<RoomMenu>> all_roomMenus;
        std::vector<std::unique_ptr<RoomListMenu>> all_roomListMenus;
};

#endif // SCENES_HPP
