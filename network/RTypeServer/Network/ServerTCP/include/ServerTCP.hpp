#ifndef SERVER
    #define SERVER
    #include "../../Auth/include/librairies.hpp"
    #include "../../Managers/DatabaseManager.hpp"
    #include "../../Managers/UserManager.hpp"
    #include "../../../../rtype_gameManager/include/gameManager.hpp"

class Session : public std::enable_shared_from_this<Session> {
    public:
        Session(asio::ip::tcp::socket socket, asio::io_context& ioc);
    
        void start();
    
        void stop();
    
        void enqueue_write(const std::string& msg);

        std::string getId() const;
        void setId(const std::string& id);

    private:
        void do_read();
    
    
        void do_write();
    
        void do_close();
    
        asio::ip::tcp::socket socket_;
        asio::strand<asio::io_context::executor_type> strand_;
        asio::streambuf streambuf_;
        std::deque<std::string> write_queue_;
        std::string id_;
};
    
// --- ServerTCPTCP: accepts new connections and spawns sessions ---
class ServerTCP {
    public:
        ServerTCP(asio::io_context& ioc, int port);
        ~ServerTCP();
        void init_database();

    private:
        void do_accept();

        asio::io_context& ioc_;
        asio::ip::tcp::acceptor acceptor_;
};
#endif
