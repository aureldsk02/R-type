#include "../include/ClientGraphics.hpp"

ClientGraphics::ClientGraphics(std::shared_ptr<ThreadSafeQueue> queue) : _queue(queue) {
    clientUI.init();
}

void ClientGraphics::loadPages() {
    clientUI.loadPages();
}

bool ClientGraphics::inGame()
{
    return _inGame;
}

sf::Keyboard::Key StringtoKey(std::string &my_key)
{
    sf::Keyboard::Key key;

    std::vector<std::string> _keys = {"UP", "DOWN", "LEFT", "RIGHT", "SPACE"};

    if ((std::find(_keys.begin(), _keys.end(), my_key)) != _keys.end()) {
        if (my_key == "UP")
            key = sf::Keyboard::Up;
        if (my_key == "DOWN")
            key = sf::Keyboard::Down;
        if (my_key == "LEFT")
            key = sf::Keyboard::Left;
        if (my_key == "RIGHT")
            key = sf::Keyboard::Right;
        if (my_key == "SPACE")
            key = sf::Keyboard::Space;
        return key;
    }
    char the_key = my_key[0];

    if (the_key >= 'A' && the_key <= 'Z') {
        key = static_cast<sf::Keyboard::Key>(the_key - 'A' + sf::Keyboard::A);
    }
    else if (the_key >= '0' && the_key <= '9') {
        key = static_cast<sf::Keyboard::Key>(the_key - '0' + sf::Keyboard::Num0);
    }
    return key;
}

const std::vector<sf::Keyboard::Key> ClientGraphics::getControllable() const
{
    const std::vector<sf::Keyboard::Key> keyboards = {UP, DOWN, LEFT, RIGHT, SPACE};

    return keyboards;
}

void ClientGraphics::handleNetworkEvent(const std::string& line)
{
    std::string cmd = line;
    if (!cmd.empty() && (cmd.back() == '\r' || cmd.back() == '\n'))
        cmd.pop_back();

    std::istringstream iss(cmd);
    std::string keyword;
    std::vector<std::string> args;
    iss >> keyword;
    std::string token;

    while (iss >> token)
        args.push_back(token);
    std::cout << keyword << std::endl;

    if (keyword == "ERROR") {
        if (args.size() == 1) {
            std::cout << "ERROR" << std::endl;
            std::string error_message = "Please enter both your username and password to continue.";
            if (args[0] == "LOGIN") {
                auto currentUI = UIManager::getInstance().getUI("Loginpage");
                auto elem = currentUI->getElementById("message_login");
                auto cast = std::dynamic_pointer_cast<TextElement>(elem);
                cast->setText(error_message);
            } else if (args[0] == "REGISTER") {
                auto currentUI = UIManager::getInstance().getUI("Registerpage");
                auto elem = currentUI->getElementById("message_register");
                auto cast = std::dynamic_pointer_cast<TextElement>(elem);
                cast->setText(error_message);
            }
        }
    } else if (keyword == "REGISTER_OK") {
        std::string message = "Your account has been successfully created! You can now log in.";
        auto currentUI = UIManager::getInstance().getUI("Registerpage");
        auto username = currentUI->getElementById("username");
        auto cast_username = std::dynamic_pointer_cast<InputFieldElement>(username);
        cast_username->setText("");
        auto password = currentUI->getElementById("password");
        auto cast_password = std::dynamic_pointer_cast<InputFieldElement>(password);
        cast_password->setText("");
        auto elem = currentUI->getElementById("message_register");
        auto cast = std::dynamic_pointer_cast<TextElement>(elem);
        cast->setText(message);
    } else if (keyword == "LOGIN_FAIL") {
        auto currentUI = UIManager::getInstance().getUI("Loginpage");
        auto elem = currentUI->getElementById("message_login");
        auto cast = std::dynamic_pointer_cast<TextElement>(elem);
        cast->setText("The username or password you entered is not correct. Try again.");
    } else if (keyword == "BANNED") {
        auto currentUI = UIManager::getInstance().getUI("Loginpage");
        auto elem = currentUI->getElementById("message_login");
        auto cast = std::dynamic_pointer_cast<TextElement>(elem);
        cast->setText("You have been banned from the server. Contact the administrator.");
    }
    else if (keyword == "LOGIN_OK") {
        std::string id = args[0];
        std::string username = args[1];
        std::string games_played = args[2];
        std::string games_won = args[3];
        std::string level = args[4];
        std::string up = args[5];
        std::string down = args[6];
        std::string left = args[7];
        std::string right = args[8];
        std::string space = args[9];

        std::cout << "LOLKOOOK " << down << space << std::endl;

        //Le username à gauche
        auto dashboardUI = UIManager::getInstance().getUI("Dashboardpage");
        auto elem_username = dashboardUI->getElementById("username_left");
        auto cast_username = std::dynamic_pointer_cast<TextElement>(elem_username);
        cast_username->setText(username);

        //Le username à droite
        auto right_username = dashboardUI->getElementById("username_right");
        auto cast_right = std::dynamic_pointer_cast<TextElement>(right_username);
        cast_right->setText(username);

        //Nombre de parties jouées
        auto games_playedd = dashboardUI->getElementById("games_played_number");
        auto cast_played = std::dynamic_pointer_cast<TextElement>(games_playedd);
        cast_played->setText(games_played);

        //Nombre de parties gagnées
        auto games_wonn = dashboardUI->getElementById("games_won_number");
        auto cast_won = std::dynamic_pointer_cast<TextElement>(games_wonn);
        cast_won->setText(games_won);

        //Level
        auto levell = dashboardUI->getElementById("level_number");
        auto cast_level = std::dynamic_pointer_cast<TextElement>(levell);
        cast_level->setText(level);

        //Je get l'interface settings
        auto settings_ui = UIManager::getInstance().getUI("SettingsPage");
        
        //Je recupere les elements un a un et je set les input fields
        //UP
        auto input_up = settings_ui->getElementById("input_up");
        auto input_up_cast = std::dynamic_pointer_cast<InputFieldElement>(input_up);
        input_up_cast->setText(up);

        UP = StringtoKey(up);

        //DOWN
        auto input_down = settings_ui->getElementById("input_down");
        auto input_down_cast = std::dynamic_pointer_cast<InputFieldElement>(input_down);
        input_down_cast->setText(down);

        DOWN = StringtoKey(down);

        //LEFT
        auto input_left = settings_ui->getElementById("input_left");
        auto input_left_cast = std::dynamic_pointer_cast<InputFieldElement>(input_left);
        input_left_cast->setText(left);

        LEFT = StringtoKey(left);

        //RIGHT
        auto input_right = settings_ui->getElementById("input_right");
        auto input_right_cast = std::dynamic_pointer_cast<InputFieldElement>(input_right);
        input_right_cast->setText(right);

        RIGHT = StringtoKey(right);

        //SHOOT
        auto input_shoot = settings_ui->getElementById("shoot");
        auto input_shoot_cast = std::dynamic_pointer_cast<InputFieldElement>(input_shoot);
        input_shoot_cast->setText(space);

        SPACE = StringtoKey(space);

        UIManager::getInstance().setUI("Dashboardpage");
        return;
    } else if (keyword == "SAVE_OK") {
        auto settings_ui = UIManager::getInstance().getCurrentUI();
        auto message_element = settings_ui->getElementById("message_settings");
        auto cast_element = std::dynamic_pointer_cast<TextElement>(message_element);

        cast_element->setText("Your informations have successfully been updated.");

        std::cout << "Key : " << args[0];
        std::cout << "Key : " << args[1];
        std::cout << "Key : " << args[2];
        std::cout << "Key : " << args[3];
        std::cout << "Key : " << args[4];

        UP = StringtoKey(args[0]);
        DOWN = StringtoKey(args[1]);
        LEFT = StringtoKey(args[2]);
        RIGHT = StringtoKey(args[3]);
        SPACE = StringtoKey(args[4]);

    } else if (keyword == "CODE") {
        std::string code = args[0];

        std::cout << "Mamamias" << std::endl;

        auto create_ui = UIManager::getInstance().getUI("CreateParty");
        std::cout << "Getter bien getté" << std::endl;
        auto message_element = create_ui->getElementById("code_value");
        auto cast_element = std::dynamic_pointer_cast<TextElement>(message_element);

        cast_element->setText(code);
        std::cout << "Bien set" << std::endl;
        UIManager::getInstance().setUI("CreateParty");
        std::cout << "Perfectly created" << std::endl;
    } else if (keyword == "ADMIN_OK") {
        std::cout << "Administrator authentication successful. Closing the graphical interface.\n";
        
        _isAdmin = true; 
        clientUI.window.close();
        return;
    } else if (keyword == "LAUNCH_OK") {
        _inGame = true;
        clientUI.window.close();
    } else if (keyword == "LEADERBOARD") {
        auto leaderboard_ui = UIManager::getInstance().getUI("Leaderboardpage");
        auto us1 = leaderboard_ui->getElementById("first_username");
        auto us1_cast = std::dynamic_pointer_cast<TextElement>(us1);
        
        us1_cast->setText(args[0]);

        auto us1_points = leaderboard_ui->getElementById("first_points");
        auto us1_points_cast = std::dynamic_pointer_cast<TextElement>(us1_points);
        
        us1_points_cast->setText(args[1]);

        auto us2 = leaderboard_ui->getElementById("second_username");
        auto us2_cast = std::dynamic_pointer_cast<TextElement>(us2);
        
        us2_cast->setText(args[2]);

        auto us2_points = leaderboard_ui->getElementById("second_points");
        auto us2_points_cast = std::dynamic_pointer_cast<TextElement>(us2_points);
        
        us2_points_cast->setText(args[3]);

        auto us3 = leaderboard_ui->getElementById("third_username");
        auto us3_cast = std::dynamic_pointer_cast<TextElement>(us3);
        
        us3_cast->setText(args[4]);

        auto us3_points = leaderboard_ui->getElementById("third_points");
        auto us3_points_cast = std::dynamic_pointer_cast<TextElement>(us3_points);
        
        us3_points_cast->setText(args[5]);
        UIManager::getInstance().setUI("Leaderboardpage");
    }
}

ClientGraphics::~ClientGraphics() = default;

bool ClientGraphics::isAdmin() {
    return _isAdmin;
}

void ClientGraphics::run()
{
    std::string network_result = "";

    while (clientUI.window.isOpen()) {
        clientUI.deltaTime = clientUI.clock.restart().asSeconds();
        clientUI.eventHandler.pollEvents(clientUI.window);
        clientUI.window.clear();
        while (_queue->try_pop(network_result)) {
            handleNetworkEvent(network_result);
        }
        renderer.render(clientUI);
        clientUI.window.display();
    }
    return;
}