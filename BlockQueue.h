#pragma once
#include <iostream>
#include <atomic>
#include <deque>
#include <future>
#include <functional>
#include <mutex>
#include <chrono>

template<typename T>
class BlockQueue {
public:
    BlockQueue() = default;
    BlockQueue(const BlockQueue&) = delete;
    BlockQueue& operator=(const BlockQueue&) = delete;
    BlockQueue(BlockQueue&& other) = delete;
    BlockQueue& operator=(BlockQueue&&) = delete;

    void push(T val) {
        std::lock_guard<std::mutex> lock(mutex_);
        data.push_back(val);
        condVar.notify_one();
    }

    std::unique_ptr<T> wait_and_pop() {
        std::unique_lock<std::mutex> ulock(mutex_);
        condVar.wait(ulock, [this](){
            return !data.empty();
        });
        std::unique_ptr<T> res = std::make_unique<T> (std::move(data.front()));
        data.pop_front();
        return std::move(res);
    }

    bool try_pop(T& val) {
        std::lock_guard<std::mutex> lock(mutex_);
        if (data.empty()) return false;
        val = std::move(data.front());
        data.pop_front();
        return true;
    }

    std::unique_ptr<T> try_pop() {
        std::lock_guard<std::mutex> lock(mutex_);
        if (data.empty())
            return std::unique_ptr<T> ();
        std::unique_ptr<T> res = std::make_unique<T> (std::move(data.front()));
        data.pop_front();
        return std::move(res);
    }

    template<typename... Args>
    void emplace(Args&&... args) {
        std::lock_guard<std::mutex> lock(mutex_);
        data.push_back(T(std::forward<Args> (args)...));
    }

private:
    mutable std::mutex mutex_;
    std::deque<T> data; // @GUARDED_BY(mutex_)
    std::condition_variable condVar; // c++20 atomic<shared_ptr<type>>
};

