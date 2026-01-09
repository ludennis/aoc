#include "threadpool.hpp"
#include "async_threadpool.hpp"

#include <iostream>

int main (int argc, char** argv)
{
    ThreadPool tp{10u};

    std::atomic<int> num{0};
    for (auto i{0u}; i < 5000; ++i)
    {
        tp.execute_task([&num](){
            num++;
        });
    }

    std::cout << "Num is: " << num << std::endl;


    AsyncThreadPool atp{10u};
    std::future<int> fut = atp.execute_task(std::packaged_task<int()>([](){
        return 15;
    }));

    std::cout << "fut: " << fut.get() << std::endl;

    sleep(1);

    std::cout << "Num is: " << num << std::endl;

    return 0;
}
