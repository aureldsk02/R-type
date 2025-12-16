#ifndef EXCEPTIONS_HPP
#define EXCEPTIONS_HPP

#include <iostream>
#include <exception>
#include <string>

class Error: public std::exception
{
    private:
        std::string _message;
    public:
        explicit Error(const std::string &msg): _message(msg) {}
        const char *what() const noexcept override {
            return _message.c_str();
        }
};

#endif // EXCEPTIONS_HPP
