#include "GameClient.hpp"
#include "Exceptions.hpp"
#include <iostream>

int main()
{
    try {
        GameClient gameClient;
        gameClient.Initialize("Player1");
        gameClient.Run();
    }
    catch (const Error& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    catch (const std::exception& e) {
        std::cerr << "Standard Exception: " << e.what() << std::endl;
        return 1;
    }
    catch (...) {
        std::cerr << "Unknown error occurred!" << std::endl;
        return 1;
    }
    
    return 0;
}
