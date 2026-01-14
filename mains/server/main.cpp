#include "../../rtype_gameManager/include/gameManager.hpp"
#include "../../network/RTypeServer/Network/ServerTCP/include/ServerTCP.hpp"

#include "logic_functions.hpp"

void printServerUsage() {
    std::cerr << "Usage:\n\t./r-type_server [tcpport] [udpport]\n" << std::endl;
    std::cerr << "\t\ttcpport: The TCP port number to run the server on" << std::endl;
    std::cerr << "\t\tudpport: The UDP port number to run the server on" << std::endl;
}

void checkServerArgs(int ac, char **av) {
    if (ac == 2 && std::string(av[1]) == "-h") {
        printServerUsage();
        exit(0);
    }
    if (ac != 3) {
        printServerUsage();
        exit(84);
    }
    try {
        std::stoi(av[1]);
        std::stoi(av[2]);
    } catch (...) {
        printServerUsage();
        exit(84);
    }
}

int main(int ac, char **av)
{
    checkServerArgs(ac, av);

    unsigned int nThreads = std::max(1u, std::thread::hardware_concurrency());

    std::vector<std::pair<asio::ip::udp::endpoint, std::vector<int8_t>>> messages;

    std::mutex mtx;

    asio::io_context ioc(nThreads);

    std::vector<std::thread> v;
    
    NetworkManager server_(std::stoi(av[2]), std::ref(messages), std::ref(mtx), ioc);
    
    GameManager::init(messages, mtx, server_);

    ServerTCP server(ioc, std::stoi(av[1]));

    for (unsigned int i = 0; i < nThreads; ++i) {
        v.emplace_back([&ioc]() { ioc.run(); });
    }

    for (auto& t : v) t.join();
}
