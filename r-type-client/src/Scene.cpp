/*
** EPITECH PROJECT, 2025
** G-CPP-500-COT-5-1-rtype-20 [WSL: Ubuntu-24.04]
** File description:
** Scenes
*/

#include "Scene.hpp"

Scene::Scene(SpriteComponent Background)
{
    game.setBackground(Background);
}

Scene::~Scene()
{
}

void Scene::Initialize()
{
}

void Scene::Update()
{
}

void Scene::Cleanup()
{
    all_menus.clear();
    all_gameModeMenus.clear();
    all_multiplayerForms.clear();
    all_settingsMenus.clear();
    all_helpMenus.clear();
    all_roomMenus.clear();
    all_roomListMenus.clear();
}

void Scene::addMenu(std::unique_ptr<Menu> menu)
{
    all_menus.push_back(std::move(menu));
}

void Scene::addGameModeMenu(std::unique_ptr<GameModeMenu> gameModeMenu)
{
    all_gameModeMenus.push_back(std::move(gameModeMenu));
}

void Scene::addMultiplayerForm(std::unique_ptr<MultiplayerForm> multiplayerForm)
{
    all_multiplayerForms.push_back(std::move(multiplayerForm));
}

void Scene::addSettingsMenu(std::unique_ptr<SettingsMenu> settingsMenu)
{
    all_settingsMenus.push_back(std::move(settingsMenu));
}

void Scene::addHelpMenu(std::unique_ptr<HelpMenu> helpMenu)
{
    all_helpMenus.push_back(std::move(helpMenu));
}

void Scene::addRoomMenu(std::unique_ptr<RoomMenu> roomMenu)
{
    all_roomMenus.push_back(std::move(roomMenu));
}

void Scene::addRoomListMenu(std::unique_ptr<RoomListMenu> roomListMenu)
{
    all_roomListMenus.push_back(std::move(roomListMenu));
}

void Scene::UpdateMenu(InputHandlerSystem& inputHandler)
{
    if (currentMenu < static_cast<int>(all_menus.size())) {
        all_menus[currentMenu]->Update(inputHandler);
    }
}

void Scene::UpdateGameModeMenu(InputHandlerSystem& inputHandler)
{
    if (currentGameModeMenu < static_cast<int>(all_gameModeMenus.size())) {
        all_gameModeMenus[currentGameModeMenu]->Update(inputHandler);
    }
}

void Scene::UpdateMultiplayerForm(InputHandlerSystem& inputHandler)
{
    if (currentMultiplayerForm < static_cast<int>(all_multiplayerForms.size())) {
        all_multiplayerForms[currentMultiplayerForm]->Update(inputHandler);
    }
}

void Scene::UpdateSettingsMenu(InputHandlerSystem& inputHandler)
{
    if (currentSettingsMenu < static_cast<int>(all_settingsMenus.size())) {
        all_settingsMenus[currentSettingsMenu]->Update(inputHandler);
    }
}

void Scene::UpdateHelpMenu(InputHandlerSystem& inputHandler)
{
    if (currentHelpMenu < static_cast<int>(all_helpMenus.size())) {
        all_helpMenus[currentHelpMenu]->Update(inputHandler);
    }
}

void Scene::UpdateRoomMenu(InputHandlerSystem& inputHandler)
{
    if (currentRoomMenu < static_cast<int>(all_roomMenus.size())) {
        all_roomMenus[currentRoomMenu]->Update(inputHandler);
    }
}

void Scene::UpdateRoomListMenu(InputHandlerSystem& inputHandler)
{
    if (currentRoomListMenu < static_cast<int>(all_roomListMenus.size())) {
        all_roomListMenus[currentRoomListMenu]->Update(inputHandler);
    }
}

void Scene::UpdateGame(float deltaTime, int id, Vector2 position)
{
    game.Update(deltaTime, id, position);
}

void Scene::RenderMenu(DisplayHandlerSystem& displayHandler)
{
    // std::cout << "RenderMenu called - currentMenu: " << currentMenu << ", all_menus.size(): " << all_menus.size() << std::endl;
    if (currentMenu < static_cast<int>(all_menus.size())) {
        // std::cout << "Rendering menu at index " << currentMenu << std::endl;
        all_menus[currentMenu]->Render(displayHandler);
    } else {
        std::cout << "ERROR: currentMenu index out of bounds!" << std::endl;
    }
}

void Scene::RenderGameModeMenu(DisplayHandlerSystem& displayHandler)
{
    if (currentGameModeMenu < static_cast<int>(all_gameModeMenus.size())) {
        all_gameModeMenus[currentGameModeMenu]->Render(displayHandler);
    }
}

void Scene::RenderMultiplayerForm(DisplayHandlerSystem& displayHandler)
{
    if (currentMultiplayerForm < static_cast<int>(all_multiplayerForms.size())) {
        all_multiplayerForms[currentMultiplayerForm]->Render(displayHandler);
    }
}

void Scene::RenderSettingsMenu(DisplayHandlerSystem& displayHandler)
{
    if (currentSettingsMenu < static_cast<int>(all_settingsMenus.size())) {
        all_settingsMenus[currentSettingsMenu]->Render(displayHandler);
    }
}

void Scene::RenderHelpMenu(DisplayHandlerSystem& displayHandler)
{
    if (currentHelpMenu < static_cast<int>(all_helpMenus.size())) {
        all_helpMenus[currentHelpMenu]->Render(displayHandler);
    }
}

void Scene::RenderRoomMenu(DisplayHandlerSystem& displayHandler)
{
    if (currentRoomMenu < static_cast<int>(all_roomMenus.size())) {
        all_roomMenus[currentRoomMenu]->Render(displayHandler);
    }
}

void Scene::RenderRoomListMenu(DisplayHandlerSystem& displayHandler)
{
    if (currentRoomListMenu < static_cast<int>(all_roomListMenus.size())) {
        all_roomListMenus[currentRoomListMenu]->Render(displayHandler);
    }
}

void Scene::removeMenu(int id)
{
    if (id < static_cast<int>(all_menus.size())) {
        all_menus.erase(all_menus.begin() + id);
    }
}

void Scene::addGame(std::unique_ptr<Game> gamePtr)
{
    if (gamePtr) {
        game = *gamePtr;
    }
}

Game& Scene::getcurrentGame()
{
    return game;
}

Menu& Scene::getcurrentMenu()
{
    return *all_menus[currentMenu];
}

GameModeMenu& Scene::getCurrentGameModeMenu()
{
    return *all_gameModeMenus[currentGameModeMenu];
}

MultiplayerForm& Scene::getCurrentMultiplayerForm()
{
    return *all_multiplayerForms[currentMultiplayerForm];
}

SettingsMenu& Scene::getCurrentSettingsMenu()
{
    return *all_settingsMenus[currentSettingsMenu];
}

HelpMenu& Scene::getCurrentHelpMenu()
{
    return *all_helpMenus[currentHelpMenu];
}

RoomMenu& Scene::getCurrentRoomMenu()
{
    return *all_roomMenus[currentRoomMenu];
}

RoomListMenu& Scene::getCurrentRoomListMenu()
{
    return *all_roomListMenus[currentRoomListMenu];
}

