#pragma once
#include "joiner.h"
#include "BlockQueue.h"
#include <iostream>
#include <functional>
#include <mutex>
#include <thread>
#include <memory>
#include <future>
#include <stdexcept>
#include <atomic>
#include <vector>

class thread_pool {

public:
    thread_pool():done(false), joiner(threads)
    {
        unsigned long long count = std::thread::hardware_concurrency();
        try {
            for (unsigned long long i = 0; i < count; ++i)
            {
                threads.push_back(
                    std::thread(&thread_pool::worker_thread, this));
            }
        } catch(...) {
            done = true;
            throw;
        }
    }

    ~thread_pool()
    {
        done = true;
    }

    template<typename Callable, typename... Args>
    auto submit(Callable&& Function, Args&&... args) 
    -> std::future<typename std::result_of<Callable(Args...)>::type>
    {
        using result_type = typename std::result_of<Callable(Args...)>::type;
        auto task = std::make_shared<std::packaged_task<result_type()>>(std::bind(std::forward<Callable> (Function),
            std::forward<Args> (args)...));
        std::future<result_type> res = task->get_future();
        {
            if (done) {
                throw std::runtime_error("stop thread pool");
            }
            work.push([task](){
                (*task)();
            });
        }
        return res;
    }

private:
    std::atomic_bool done;
    BlockQueue<std::function<void()>> work;
    std::vector<std::thread> threads;
    joiner_thread joiner;
    void worker_thread() {
        while(!done) {
            std::function<void()> task;
            if (work.try_pop(task)) {
                task();
            } else {
                std::this_thread::yield();
            }
        }
    }
};