#ifndef CLIENTGRAPHICS
    #define CLIENTGRAPHICS
    #include "./ClientUI.hpp"
    #include "./UIRenderer.hpp"
    #include "../../Network/ClientTCP/include/ThreadSafeQueue.hpp"
    #include "../../Managers/ActionRegistry.hpp"

class ClientGraphics {
    public:
        ClientUI clientUI;
        UIRenderer renderer;
        std::shared_ptr<ThreadSafeQueue> _queue;
        bool _isAdmin = false;
        bool _inGame = false;
        sf::Keyboard::Key UP;
        sf::Keyboard::Key DOWN;
        sf::Keyboard::Key LEFT;
        sf::Keyboard::Key RIGHT;
        sf::Keyboard::Key SPACE;

        ClientGraphics(std::shared_ptr<ThreadSafeQueue> queue);
        void handleNetworkEvent(const std::string &string);
        void loadPages();
        ~ClientGraphics();
        bool isAdmin();
        bool inGame();
        void run();
        const std::vector<sf::Keyboard::Key> getControllable() const;
};
#endif