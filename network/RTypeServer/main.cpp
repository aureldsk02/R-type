#include "./Network/ServerTCP/include/ServerTCP.hpp"

int main(int argc, char* argv[])
{
    std::string address = argv[1];
    unsigned short port = static_cast<unsigned short>(std::stoi(argv[2]));

    asio::io_context ioc{1};

    tcp::endpoint endpoint{asio::ip::make_address(address), port};
    ServerTCP server(ioc, 1379);

    unsigned int nThreads = std::max(1u, std::thread::hardware_concurrency());

    std::vector<std::thread> v;
    for (unsigned int i = 0; i < nThreads; ++i) {
        v.emplace_back([&ioc]() { ioc.run(); });
    }

    for (auto& t : v) t.join();

    return 0;
}
