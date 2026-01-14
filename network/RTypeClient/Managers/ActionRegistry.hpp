#ifndef ACTIONREGISTRY
    #define ACTIONREGISTRY
    #include "../UI/include/librairies.hpp"
    #include "UIManager.hpp"
    #include "../Network/ClientTCP/include/ClientTCP.hpp"
    #include "../UI/components/include/InputFieldElement.hpp"
    #include "../../utility_classes/include/Network.hpp"

inline std::string endpoint_to_string(const asio::ip::udp::endpoint& ep)
{
    std::string address = ep.address().to_string();
    
    unsigned short port = ep.port();
    
    return address + ":" + std::to_string(port);
}

class ActionRegistry {
    public:
        using Action = std::function<void()>;
        std::unordered_map<std::string, Action> actions;
    
        static ActionRegistry& getInstance() {
            static ActionRegistry instance;
            return instance;
        }
    
        void registerAction(const std::string& name, Action action) {
            actions[name] = std::move(action);
        }
    
        Action get(const std::string& name) {
            auto it = actions.find(name);
            if (it != actions.end())
                return it->second;
            return nullptr;
        }

        void setDefaultActions() {
            const std::vector<std::string> pages = {
                "Homepage", "Registerpage", "Loginpage", "Dashboardpage", 
                "AboutUsPage", "HelpPage", "SettingsPage", "JoinParty", "CustomParty"
            };
    
            for (const auto& page : pages) {
                registerAction(page, [page]() {
                    UIManager::getInstance().setUI(page);
                });
            }
        }

        void setNetworkActions(std::shared_ptr<ClientTCP> client) {

            registerAction("Login", [weak_client = std::weak_ptr<ClientTCP>(client)]() {
                if (auto client = weak_client.lock()) {
                    auto ui = UIManager::getInstance().getCurrentUI();
                    if (!ui) return;

                    auto usernameField = std::dynamic_pointer_cast<InputFieldElement>(ui->getElementById("username"));
                    auto passwordField = std::dynamic_pointer_cast<InputFieldElement>(ui->getElementById("password"));
        
                    if (usernameField && passwordField) {
                        std::string username = usernameField->getText();
                        std::string password = passwordField->getText();
        
                        std::string message = "LOGIN " + username + " " + password + "\n";
                        client->write(message);
                    }
                }
            });
        
            registerAction("Leaderboard", [weak_client = std::weak_ptr<ClientTCP>(client)]() {
                if (auto client = weak_client.lock()) {
                    client->write("LEADERBOARD\n");
                }
            });
        
            registerAction("Register", [weak_client = std::weak_ptr<ClientTCP>(client)]() {
                if (auto client = weak_client.lock()) {
                    auto ui = UIManager::getInstance().getCurrentUI();
                    if (!ui) return;

                    auto usernameField = std::dynamic_pointer_cast<InputFieldElement>(ui->getElementById("username"));
                    auto passwordField = std::dynamic_pointer_cast<InputFieldElement>(ui->getElementById("password"));
        
                    if (usernameField && passwordField) {
                        std::string username = usernameField->getText();
                        std::string password = passwordField->getText();
        
                        std::string message = "REGISTER " + username + " " + password + "\n";
                        client->write(message);
                    }
                }
            });
        
            registerAction("SaveSettings", [weak_client = std::weak_ptr<ClientTCP>(client)]() {
                if (auto client = weak_client.lock()) {
                    auto ui = UIManager::getInstance().getCurrentUI();
                    if (!ui) return;
                    auto up_el = std::dynamic_pointer_cast<InputFieldElement>(ui->getElementById("input_up"));
                    auto down_el = std::dynamic_pointer_cast<InputFieldElement>(ui->getElementById("input_down"));
                    auto left_el = std::dynamic_pointer_cast<InputFieldElement>(ui->getElementById("input_left"));
                    auto right_el = std::dynamic_pointer_cast<InputFieldElement>(ui->getElementById("input_right"));
                    auto shoot_el = std::dynamic_pointer_cast<InputFieldElement>(ui->getElementById("shoot"));

                    if (!up_el || !down_el || !left_el || !right_el || !shoot_el) return;

                    std::string result = up_el->getText() + " " + down_el->getText() + " " +
                                        left_el->getText() + " " + right_el->getText() + " " +
                                        shoot_el->getText();
        
                    auto dashboard_ui = UIManager::getInstance().getUI("Dashboardpage");
                    if (!dashboard_ui) return;
                    auto username_text_el = std::dynamic_pointer_cast<TextElement>(dashboard_ui->getElementById("username_left"));
                    if (!username_text_el) return;
                    std::string username_text = username_text_el->getContent();

                    std::string message = "SAVE " + username_text + " " + result + "\n";
                    client->write(message);
                }
            });
        
            registerAction("LaunchGame", [weak_client = std::weak_ptr<ClientTCP>(client)]() {
                if (auto client = weak_client.lock()) {
                    auto ui = UIManager::getInstance().getCurrentUI();
                    if (!ui) return;
                    auto code_el = std::dynamic_pointer_cast<TextElement>(ui->getElementById("code_value"));
                    if (!code_el) return;
                    std::string code = code_el->getContent();
                    std::string message = "LAUNCH_GAME " + code + "\n";
                    client->write(message);
                }
            });
        }
        

        void setInstanceActions(NetworkManager& networkManager, std::shared_ptr<ClientTCP> client) {
        
            registerAction("CreateParty", [&networkManager, weak_client = std::weak_ptr<ClientTCP>(client)]() {
                if (auto client = weak_client.lock()) {
                    std::string message = "CREATE_GAME ";
                    std::string custom = "";
                    std::string udp_endpoint = endpoint_to_string(networkManager.getEndpoint());

                    message += udp_endpoint;
                    message += " ";

                    auto ui = UIManager::getInstance().getCurrentUI();

                    auto elem = ui->getElementById("mode_choice");
                    auto cast = std::dynamic_pointer_cast<ChoiceFieldElement>(elem);

                    if (cast->getSelectedIndex() == 0) {
                        custom += "NORMAL ";
                        auto diff_choice = ui->getElementById("difficulty_choice");
                        auto diff_choice_cast = std::dynamic_pointer_cast<ChoiceFieldElement>(diff_choice);
                        switch(diff_choice_cast->getSelectedIndex()) {
                            case 0:
                                custom += "EASY";
                                break;
                            case 1:
                                custom += "MEDIUM";
                                break;
                            case 2:
                                custom += "DIFFICULT";
                                break;
                        }
                    } else if (cast->getSelectedIndex() == 1) {
                        custom += "PVP";
                    } else if (cast->getSelectedIndex() == 2) {
                        custom += "CUSTOM ";
                        auto input = ui->getElementById("file_path");
                        auto input_cast = std::dynamic_pointer_cast<InputFieldElement>(input);

                        custom += input_cast->getText();
                        if (input_cast->getText().size() == 0)
                            return;
                    }

                    std::string final = message + custom + "\n";
                    client->write(final);
                } else {
                    std::cerr << "[CreateParty] ClientTCP instance expired, cannot send CREATE_GAME\n";
                }
            });
        
            registerAction("JoinMe", [&networkManager, weak_client = std::weak_ptr<ClientTCP>(client)]() {
                if (auto client = weak_client.lock()) {
                    auto ui = UIManager::getInstance().getCurrentUI();
                    auto value_elem = std::dynamic_pointer_cast<InputFieldElement>(ui->getElementById("join_code"));
                    if (!value_elem) {
                        std::cerr << "[JoinMe] join_code field not found\n";
                        return;
                    }
        
                    std::string value = value_elem->getText();
                    std::string udp_endpoint = endpoint_to_string(networkManager.getEndpoint());
                    std::string final = "JOIN_GAME " + value + " " + udp_endpoint + "\n";
                    if (value.size() == 5)
                        client->write(final);
                } else {
                    std::cerr << "[JoinMe] ClientTCP instance expired, cannot send JOIN_GAME\n";
                }
            });
        }
        

        void clear() {
            for (auto &entry : actions) {
                entry.second = nullptr;
            }
            actions.clear();
        }
    
};
#endif