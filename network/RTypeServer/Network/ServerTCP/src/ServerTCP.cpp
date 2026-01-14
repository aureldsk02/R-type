#include "../include/SessionManager.hpp"

//-----------------------SESSION-------------------------------

std::string generate_five_digit_random() {
    static std::random_device rd;
    static std::mt19937 generator(rd());

    const int min_value = 10000;
    const int max_value = 99999;
    
    std::uniform_int_distribution<int> distribution(min_value, max_value);

    int randomNumberInt = distribution(generator);
    return std::to_string(randomNumberInt);
}

std::string generateUniqueSessionId()
{
    static std::mt19937_64 rng(std::random_device{}());
    static std::uniform_int_distribution<uint64_t> dist;

    std::ostringstream oss;
    oss << std::hex << std::setw(16) << std::setfill('0') << dist(rng);
    return oss.str();
}

asio::ip::udp::endpoint string_to_endpoint(const std::string& ep_str)
{
    size_t pos = ep_str.find_last_of(':');

    if (pos == std::string::npos) {
        throw std::runtime_error("Format d'endpoint invalide (doit contenir ':').");
    }

    std::string address_str = ep_str.substr(0, pos);
    
    std::string port_str = ep_str.substr(pos + 1);

    unsigned short port = 0;
    try {
        port = static_cast<unsigned short>(std::stoi(port_str));
    } catch (const std::exception& e) {
        throw std::runtime_error("Port invalide: " + port_str);
    }
    
    asio::ip::address address = asio::ip::make_address(address_str);

    return asio::ip::udp::endpoint(address, port);
}

static std::string interpret_command(const std::string& line, const std::string& sessionId)
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

    if (keyword == "REGISTER") {
        if (args.size() < 2)
            return "ERROR REGISTER\n"; //Missing username or password

        const std::string& username = args[0];
        const std::string& password = args[1];

        if (username.empty() || password.empty()) {
            return "ERROR REGISTER\n"; //Missing username or password
        }

        UserManager::getInstance().createUser(username, password, DatabaseManager::getInstance().getDB());
        return "REGISTER_OK " + username + "\n";
    } else if (keyword == "LOGIN") {
        if (args.size() < 2)
            return "ERROR LOGIN\n"; //Missing username or password

        const std::string& username = args[0];
        const std::string& password = args[1]; 

        if (username.empty() || password.empty())
            return "ERROR LOGIN\n"; //Missing username or password

        bool valid = UserManager::getInstance().authenticate(username, password, DatabaseManager::getInstance().getServerKey());

        if (valid) {
            auto userOpt = UserManager::getInstance().getUserByUsername(username);
            if (userOpt) {
                User& user = userOpt.value().get();
                if (user.isUserBanned())
                    return "BANNED\n";
                if (user.getUserRole() == "ADMIN")
                    return "ADMIN_OK\n";
                std::vector<std::string> keyMap = user.getUserControl().getKeys();
                std::string final_result = "";
                for (auto value : keyMap) {
                    final_result += value;
                    final_result += " ";
                }
                final_result.pop_back();
                std::string result = std::to_string(user.getId()) + " " + username + " " +
                    std::to_string(user.getUserStats().getNbGamesPlayed()) 
                    + " " + std::to_string(user.getUserStats().getNbGamesWon()) +
                    " " + std::to_string(user.getUserStats().getLevel()) + " " + final_result;
                return "LOGIN_OK " + result + "\n";
            }
        } else
            return "LOGIN_FAIL\n";
    } else if (keyword == "SAVE") {
        auto userOpt = UserManager::getInstance().getUserByUsername(args[0]);
        if (userOpt) {
            User& user = userOpt.value().get();
            std::vector<std::string> _keyMap = {args[1], args[2], args[3], args[4], args[5]};
            user.bind_keys(_keyMap);
            user.save(DatabaseManager::getInstance().getDB());
            std::string response = "";
            for (auto value : _keyMap) {
                response += value;
                response += " ";
            }
            response.pop_back();
            return "SAVE_OK " + response + "\n";
        }
    } else if (keyword == "CREATE_GAME") {
        std::string code = generate_five_digit_random();
        asio::ip::udp::endpoint end = string_to_endpoint(args[0]);
        GameSettings::GameMode gameMode = GameSettings::GameMode::NORMAL;
        GameSettings::Difficulty diff = GameSettings::Difficulty::EASY;
        std::string filepath = "";

        std::string last = "";

        for (auto value : args)
            last += value;
        std::cout << last << std::endl;

        if (args[1] == "NORMAL") {
            gameMode = GameSettings::GameMode::NORMAL;
            std::string level = args[2];
            if (level == "EASY")
                diff = GameSettings::Difficulty::EASY;
            else if (level == "MEDIUM")
                diff = GameSettings::Difficulty::MEDIUM;
            else if (level == "DIFFICULT")
                diff = GameSettings::Difficulty::DIFFICULT;
        } else if (args[1] == "PVP") {
            gameMode = GameSettings::GameMode::PVP;
        } else if (args[1] == "CUSTOM") {
            gameMode = GameSettings::GameMode::CUSTOM;
            filepath = args[2];
        }

        GameSettings gameSettings(gameMode, diff, filepath);

        //Tu passes ici la variable à ta gameInstance

        GameManager::getInstance().create_game(code, gameSettings);
        GameManager::getInstance().addClientToGame(code, end, sessionId);
        std::string message = "CODE " + code + "\n";
        std::cout << message << std::endl;
        return message;
    } else if (keyword == "JOIN_GAME") {
        std::string code = args[0];
        asio::ip::udp::endpoint end = string_to_endpoint(args[1]);
        
        GameManager::getInstance().addClientToGame(code, end, sessionId);
        return "JOIN_OK\n";
    } else if (keyword == "LAUNCH_GAME") {
        std::string code = args[0];

        GameManager::getInstance().start_game(code);
    } else if (keyword == "LIST") {
        std::vector<std::string> _users = UserManager::getInstance().getUsersList();
        std::string begin = "List of users : \n";
        int i = 1;

        for (auto user : _users) {
            begin += std::to_string(i) + ". " + user + "\n";
            i++;
        }
        return begin;
    } else if (keyword == "BAN") {
        auto userOpt = UserManager::getInstance().getUserByUsername(args[0]);
        if (userOpt) {
            User& user = userOpt.value().get();
            user.setBanned(true);
            user.save(DatabaseManager::getInstance().getDB());
            std::string value = "User " + args[0] + " has been successfully banned from the server.\n";
            return value;
        }
        return "No user with username " + args[0] + " has been found in the database.\n";
    }  else if (keyword == "RELEASE") {
        auto userOpt = UserManager::getInstance().getUserByUsername(args[0]);
        if (userOpt) {
            User& user = userOpt.value().get();
            user.setBanned(false);
            user.save(DatabaseManager::getInstance().getDB());
            std::string value = "User " + args[0] + " has been successfully released from the server.\n";
            return value;
        }
        return "No user with username " + args[0] + " has been found in the database.\n";
    } else if (keyword == "LEADERBOARD") {
        std::string value = UserManager::getInstance().getTopThreeUsersString();
        std::string response = "LEADERBOARD " + value + "\n";

        return response;
    }
    else if (keyword == "QUIT") {
        return "BYE\n";
    }
    return "ERROR Unknown command\n";
}


Session::Session(asio::ip::tcp::socket socket, asio::io_context& ioc)
        : socket_(std::move(socket)),
          strand_(asio::make_strand(ioc))
{}

std::string Session::getId() const { return id_; }
void Session::setId(const std::string& id) { id_ = id; }

void Session::start() {
    do_read();
}

void Session::stop() {
    asio::post(strand_, [self = shared_from_this()]() {
        std::error_code ec;
        self->socket_.close(ec);
    });
}

void Session::do_read() {
    auto self = shared_from_this();
    asio::async_read_until(socket_, streambuf_, '\n', 
        asio::bind_executor(strand_,
            [this, self](std::error_code ec, std::size_t bytes_transferred) {
                if (!ec) {
                    std::string line;
                    std::istream is(&streambuf_);
                    std::getline(is, line);
                    std::string response = interpret_command(line, id_);

                    if (!response.empty())
                        enqueue_write(response);
                    else
                        do_read();
                } else
                    do_close();
            }
        )
    );
}

void Session::enqueue_write(const std::string& msg) {
    auto self = shared_from_this();
    asio::post(strand_, [this, self, msg]() {
        bool write_in_progress = !write_queue_.empty();
        write_queue_.push_back(msg);
        if (!write_in_progress) {
            do_write();
        }
    });
}

void Session::do_write() {
    auto self = shared_from_this();
    asio::async_write(socket_,
        asio::buffer(write_queue_.front()),
        asio::bind_executor(strand_,
            [this, self](std::error_code ec, std::size_t bytes_transferred) {
                if (!ec) {
                    write_queue_.pop_front();
                    if (!write_queue_.empty())
                        do_write();
                    else
                        do_read();
                } else
                    do_close();
            }
        )
    );
}

void Session::do_close() {
    std::error_code ec;
    socket_.shutdown(asio::ip::tcp::socket::shutdown_both, ec);
    socket_.close(ec);
}

//----------------------SERVER-----------------------------------------------

ServerTCP::ServerTCP(asio::io_context& ioc, int port)
    : ioc_(ioc), acceptor_(ioc, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port))
{
    init_database();
    do_accept();
}

ServerTCP::~ServerTCP() {
    DatabaseManager::getInstance().close();
}

void ServerTCP::do_accept() {
    acceptor_.async_accept(
        asio::make_strand(ioc_),
        [this](std::error_code ec, asio::ip::tcp::socket socket) {
            if (!ec) {
                std::cout << "New connection from " << socket.remote_endpoint() << "\n";
                auto session = std::make_shared<Session>(std::move(socket), ioc_);
                session->setId(generateUniqueSessionId());
                SessionManager::getInstance().addSession(session->getId(), session);
                session->start();
            } else {
                std::cerr << "Accept error: " << ec.message() << "\n";
            }
            do_accept();
        } 
    );
}

void ServerTCP::init_database()
{
    DatabaseManager::getInstance().open("Database/r-type.db");
    DatabaseManager::getInstance().initialize();
    DatabaseManager::getInstance().generateServerKey();
    UserManager::getInstance().loadAllUsers(DatabaseManager::getInstance().getDB());
    UserManager::getInstance().updateId();
    return;
}