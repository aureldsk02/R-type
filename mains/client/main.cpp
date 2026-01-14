#include "../../rtype_graphicsClient/include/graphicsClient.hpp"
#include "../../network/RTypeClient/UI/include/ClientGraphics.hpp"

void printClientUsage() {
    std::cerr << "Usage:\n\t./r-type_client [tcp_port] [udp_port] [ip_address]\n" << std::endl;
    std::cerr << "\t\ttcp_port: The TCP port number of the server" << std::endl;
    std::cerr << "\t\tudp_port: The UDP port number of the server" << std::endl;
    std::cerr << "\t\tip_address: The ip address of the server" << std::endl;

}

void checkClientArgs(int ac, char **av) {
    if (ac == 2 && std::string(av[1]) == "-h") {
        printClientUsage();
        exit(0);
    }
    if (ac != 4) {
        printClientUsage();
        exit(84);
    }
    try {
        std::stoi(av[1]);
    } catch (...) {
        printClientUsage();
        exit(84);
    }
}

void launchAdminConsole(std::shared_ptr<ClientTCP> client, std::shared_ptr<ThreadSafeQueue> network_queue) 
{
    std::cout << "\n======================================================\n";
    std::cout << "          ADMINISTRATOR MODE (Console)          \n";
    std::cout << "======================================================\n";
    std::cout << "Available commands: list | ban <user> | release <user> | exit\n";

    std::string input;
    std::string network_result = "";    

    while (network_queue->try_pop(network_result)) {}

    while (true) {
        while (network_queue->try_pop(network_result)) {
            std::cout << network_result << "\n";
        }
        std::cout << "\nAdmin > " << std::flush;

        std::getline(std::cin, input);

        std::istringstream iss(input);
        std::string cmd;
        std::string arg;

        iss >> cmd;

        if (cmd == "exit") {
            std::cout << "Closing the administrator console.\n";
            break;
        } 
        
        else if (cmd == "list") {
            client->write("LIST\n");
        } 
        
        else if (cmd == "ban" || cmd == "release") {
            if (!(iss >> arg)) {
                std::cout << "ERROR: Incorrect syntax. Use: " << cmd << " <username>\n";
                continue;
            }
            client->write((cmd == "ban" ? "BAN " : "RELEASE ") + arg + "\n");
        }
        
        else if (!cmd.empty()) {
            std::cout << "ERROR: Unknown command. Type 'exit' to quit.\n";
        }
        input.clear();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

int main(int ac, char **av) {
    checkClientArgs(ac, av);

    int port = std::stoi(av[1]);
    int udp_port = std::stoi(av[2]);
    std::string address = av[3];
    std::vector<int8_t> lastmsg;
    std::mutex mtx;


    auto eventQueue = std::make_shared<ThreadSafeQueue>();
    ClientGraphics clientGraphics(eventQueue);

    asio::io_context context;

    asio::ip::tcp::resolver resolver(context);
    auto endpoints = resolver.resolve(address, std::to_string(port));
    auto client = std::make_shared<ClientTCP>(context, endpoints, eventQueue);
    client->start(endpoints);

    NetworkManager networkManager(udp_port, address, lastmsg, mtx, context);
    ActionRegistry::getInstance().setNetworkActions(client);
    ActionRegistry::getInstance().setInstanceActions(networkManager, client);
    clientGraphics.loadPages();

    asio::ip::udp::endpoint endpoint = networkManager.getEndpoint();

    std::cout << "Client UDP endpoint: " << endpoint.address().to_string() << ":" << endpoint.port() << std::endl;

    std::thread t([&context]() { context.run(); });

    clientGraphics.run();

    for (auto key : clientGraphics.getControllable()) {
        std::cout << key << std::endl;
    }
    GraphicsClient grClient(networkManager, lastmsg, mtx, clientGraphics.getControllable());

    printf("window closed\n");
    if (clientGraphics.isAdmin())
        launchAdminConsole(client, eventQueue);

    if (clientGraphics.inGame())
        grClient.run();
    printf("Stopping context\n");

    ActionRegistry::getInstance().clear();

    client.reset();
    
    context.stop();
    
    t.join();
}
