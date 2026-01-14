#include "./Network/ClientTCP/include/ClientTCP.hpp"
#include "./UI/include/ClientGraphics.hpp"

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: client <host> <port>\n";
        return 84;
    }

    try {
        auto eventQueue = std::make_shared<ThreadSafeQueue>();
        ClientGraphics clientGraphics(eventQueue);
        asio::io_context io_context;
        tcp::resolver resolver(io_context.get_executor());
        auto endpoints = resolver.resolve(argv[1], argv[2]);

        auto client = std::make_shared<ClientTCP>(io_context, endpoints, eventQueue);
        client->start(endpoints);
        
        
        ActionRegistry::getInstance().setNetworkActions(client);
        clientGraphics.loadPages();

        std::thread t([&io_context]() { io_context.run(); });

        clientGraphics.run();

        io_context.stop();
        t.join();
    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}
