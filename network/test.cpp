#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <memory>
#include <ctime>
#include <sodium.h> // libsodium
#include "server.hpp"
#include <cstdlib>

TokenManager* TokenManager::s_pInstance = nullptr;
UserManager* UserManager::s_pInstance = nullptr;
DatabaseManager* DatabaseManager::s_pInstance = nullptr;

int main() {
    try {
        DatabaseManager::Instance()->open("game.db");
        DatabaseManager::Instance()->initialize();
        DatabaseManager::Instance()->generateServerKey();

        // === 4. Création de quelques utilisateurs ===
        std::cout << "\n=== Création d'utilisateurs ===" << std::endl;
        // UserManager::Instance()->createUser("Alice", "password123", DatabaseManager::Instance()->getDB());
        UserManager::Instance()->createUser("Bob", "secret456", DatabaseManager::Instance()->getDB());
        UserManager::Instance()->loadAllUsers(DatabaseManager::Instance()->getDB());
        UserManager::Instance()->displayUsers();
        // === 5. Test d’authentification ===
        std::cout << "\n=== Test d'authentification ===" << std::endl;

        // Authentification réussie
        if (UserManager::Instance()->authenticate("Alice", "password123", DatabaseManager::Instance()->getServerKey())) {
            std::cout << "Alice authentifiée avec succès !" << std::endl;

            auto aliceOpt = UserManager::Instance()->getUserByUsername("Alice");
            if (aliceOpt) {
                User& alice = aliceOpt.value().get();
                std::cout << "Auth token (hex): " << alice.getAuthTokenHex() << std::endl;
            }
        } else {
            std::cout << "Échec d'authentification pour Alice !" << std::endl;
        }

        // Mauvais mot de passe
        if (UserManager::Instance()->authenticate("Bob", "wrongpass", DatabaseManager::Instance()->getServerKey())) {
            std::cout << "Bob authentifié (erreur attendue !)" << std::endl;
        } else {
            std::cout << "Bob échec d'authentification (comme prévu)" << std::endl;
        }

        if (UserManager::Instance()->authenticate("Bob", "secret456", DatabaseManager::Instance()->getServerKey())) {
            std::cout << "Bob authentifié avec succès !" << std::endl;

            auto bobOpt = UserManager::Instance()->getUserByUsername("Bob");
            if (bobOpt) {
                User& bob = bobOpt.value().get();
                std::cout << "Auth token (hex): " << bob.getAuthTokenHex() << std::endl;
            }
        }

        // // === 6. Fermeture propre ===
        DatabaseManager::Instance()->close();
        std::cout << "\n Fin du test — base fermée proprement." << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "Erreur critique : " << e.what() << std::endl;
        return 84;
    }

    return 0;
}
