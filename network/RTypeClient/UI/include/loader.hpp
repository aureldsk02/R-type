#ifndef LOADER
    #define LOADER
    #include "./librairies.hpp"
    #include "../../Managers/UIManager.hpp"
    #include "../parsers/include/parseUI.hpp"

void loadUIFromFile(const std::string& filePath) {
    try {
        libconfig::Config cfg;
        cfg.readFile(filePath.c_str());

        const libconfig::Setting& root = cfg.getRoot();

        for (int i = 0; i < root.getLength(); ++i) {
            const libconfig::Setting& uiSetting = root[i];
            auto ui = parseUI(uiSetting);
            if (ui) {
                UIManager::getInstance().addUI(ui);
            }
        }
    }
    catch (const libconfig::FileIOException& e) {
        std::cerr << "Impossible to read the file: " << filePath << std::endl;
    }
    catch (const libconfig::ParseException& e) {
        std::cerr << "Parsing error in file " << filePath 
                  << " at line " << e.getLine() << ": " << e.getError() << std::endl;
    }
}

void loadAllUI(void)
{
    std::vector<std::string> _files = {
        "UIScenes/infospage.cfg",
        "UIScenes/homepage.cfg",
        "UIScenes/helppage.cfg"
    };

    for (auto elem : _files)
        loadUIFromFile(elem);
    return;
}

void loadDynamicUI(void)
{
    std::vector<std::string> _files = {
        "UIScenes/registerpage.cfg", 
        "UIScenes/loginpage.cfg",
        "UIScenes/createParty.cfg",
        "UIScenes/customParty.cfg",
        "UIScenes/joinParty.cfg",
        "UIScenes/leaderboard.cfg",
        "UIScenes/dashboard_player.cfg",
        "UIScenes/settings.cfg"
    };

    for (auto elem : _files)
        loadUIFromFile(elem);
    return;
}
#endif