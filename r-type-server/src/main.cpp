/*
** EPITECH PROJECT, 2025
** G-CPP-500-COT-5-1-rtype-20
** File description:
** main
*/

#include "../include/NetworkManager.hpp"
#include <thread>

int main (){
    int port = 12345;

    // Create an instance of RoomManager
    NetworkManager networkManager(port);
    RoomManager roomManager;
    networkManager.set_room_manager(&roomManager);
    roomManager.set_socket(networkManager.getSocket());
    std::thread networkThread([&networkManager]() {
        networkManager.start_receive();
        networkManager.run();
    });
    // std::cout << "Serveur UDP en écoute sur le port dans le main " << port << std::endl;
    std::thread roomThread([&roomManager]() {
        roomManager.updateRooms();
    });
    std::cout << "Serveur de gestion des salles en cours d'exécution." << std::endl;
    networkThread.join();
    roomThread.join();
    return 0;
}
