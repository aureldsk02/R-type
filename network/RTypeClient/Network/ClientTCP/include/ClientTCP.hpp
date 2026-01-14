#ifndef CLIENT_TCP
    #define CLIENT_TCP
    #include "../../UI/include/librairies.hpp"
    #include "./ThreadSafeQueue.hpp"

class ClientTCP : public std::enable_shared_from_this<ClientTCP> {
public:
    ClientTCP(asio::io_context& io_context, asio::ip::tcp::resolver::results_type endpoints,
        std::shared_ptr<ThreadSafeQueue> queue);

    void write(const std::string& msg);

    void start(asio::ip::tcp::resolver::results_type endpoints);

private:
    void do_connect(asio::ip::tcp::resolver::results_type endpoints);

    void do_read();

    void do_write();

    asio::ip::tcp::socket socket_;
    asio::streambuf read_buffer_;
    std::deque<std::string> write_queue_;
    std::shared_ptr<ThreadSafeQueue> _eventQueue;
};
#endif