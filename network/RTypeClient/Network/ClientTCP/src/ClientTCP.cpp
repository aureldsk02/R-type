#include "../include/ClientTCP.hpp"

ClientTCP::ClientTCP(asio::io_context& io_context, asio::ip::tcp::resolver::results_type endpoints, 
    std::shared_ptr<ThreadSafeQueue> queue)
    : socket_(io_context), _eventQueue(queue) {
}

void ClientTCP::start(asio::ip::tcp::resolver::results_type endpoints) {
    do_connect(endpoints);
}

void ClientTCP::write(const std::string& msg) {
    auto self = shared_from_this();
    asio::post(socket_.get_executor(),
        [this, self, msg]() {
            bool write_in_progress = !write_queue_.empty();
            write_queue_.push_back(msg);
            if (!write_in_progress) {
                do_write();
            }
        });
}

void ClientTCP::do_connect(asio::ip::tcp::resolver::results_type endpoints) {
    auto self = shared_from_this();
    asio::async_connect(socket_, endpoints,
        [this, self](std::error_code ec, asio::ip::tcp::endpoint) {
            if (!ec) {
                do_read();
            } else {
                std::cerr << "Connect failed: " << ec.message() << "\n";
            }
        });
}

void ClientTCP::do_read() {
    auto self = shared_from_this();
    asio::async_read_until(socket_, read_buffer_, '\n',
        [this, self](std::error_code ec, std::size_t) {
            if (!ec) {
                std::istream is(&read_buffer_);
                std::string line;
                std::getline(is, line);
                if (!line.empty())
                    _eventQueue->push(line);
                do_read(); // relancer lecture
            } else {
                std::cerr << "Read error: " << ec.message() << "\n";
                socket_.close();
            }
        });
}

void ClientTCP::do_write() {
    auto self = shared_from_this();
    asio::async_write(socket_,
        asio::buffer(write_queue_.front()),
        [this, self](std::error_code ec, std::size_t) {
            if (!ec) {
                write_queue_.pop_front();
                if (!write_queue_.empty()) {
                    do_write();
                }
            } else {
                socket_.close();
            }
        });
}