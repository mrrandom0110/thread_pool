#include "thread_pool.h"
#include "BlockQueue.h"
#include "joiner.h"
#include <iostream>
#include <chrono>
#include <thread>
using namespace std;
int main() {
    /*auto handle = [](){
        cout << "QQQ\n";
    };
    auto p = std::bind(handle);
    //cout << typeid(p).name() << '\n';
    auto f = [p]{
        p();
    };
    std::packaged_task<void()> task(f);
    auto m = std::make_shared<packaged_task<void()>> (p);
    std::future<void> fut = m->get_future();
    auto pp = [m](){
        (*m)();
    };
    std::thread(pp).detach();
    fut.get();*/
    thread_pool pool;
    auto f1 = pool.submit([](){
        std::this_thread::sleep_for(std::chrono::seconds(1));
    });
    auto f2 = pool.submit([](){
        std::this_thread::sleep_for(std::chrono::seconds(1));
    });
    auto f3 = pool.submit([](){
        std::this_thread::sleep_for(std::chrono::seconds(1));
    });
    f1.get(), f2.get(), f3.get();
    
}