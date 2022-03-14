#include "thread_pool.h"
#include "BlockQueue.h"
#include "joiner.h"
#include <iostream>
#include <chrono>
#include <thread>
using namespace std;
int main() {
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

            /*        real	0m1,003s
                      user	0m0,320s
                      sys	0m0,684s    */
}