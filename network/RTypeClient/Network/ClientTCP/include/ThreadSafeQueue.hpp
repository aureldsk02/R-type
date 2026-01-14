#pragma once
#include "../../UI/include/librairies.hpp"

class ThreadSafeQueue {
public:
    void push(const std::string& value) {
        std::lock_guard<std::mutex> lock(mtx);
        queue.push(value);
    }

    bool try_pop(std::string& result) {
        std::lock_guard<std::mutex> lock(mtx);
        if (queue.empty())
            return false;
        result = queue.front();
        queue.pop();
        return true;
    }

private:
    std::queue<std::string> queue;
    std::mutex mtx;
};
