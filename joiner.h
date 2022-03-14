#pragma once
#include <vector>
#include <thread>

class joiner_thread {
public:
    explicit joiner_thread(std::vector<std::thread>& th):
        joiner(th)
    {}
    ~joiner_thread()
    {
        for (unsigned long i = 0; i < joiner.size(); ++i) {
            if (joiner[i].joinable())
                joiner[i].join();
        }
    }

private:
    std::vector<std::thread>& joiner;
};