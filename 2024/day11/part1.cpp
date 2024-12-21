#include <fstream>
#include <string>
#include <iostream>
#include <sstream>
#include <vector>
#include <memory>
#include <thread>
#include <mutex>
#include <functional>

struct SafeThreadPool
{
    template <typename F, typename... Args>
    void push(F&& f, Args&& ...args)
    {
        std::lock_guard<std::mutex> l(lock);
        threads.emplace_back(std::unique_ptr<std::thread>(
                std::forward<F>(f)(std::forward<Args>(args))...));
    }

    void join()
    {
        for (auto& t : threads)
            t->join();
    }

    std::vector<std::unique_ptr<std::thread>> threads;
    std::mutex lock;
};

template <typename T>
void blink(std::vector<T>& v)
{
    auto orig_size = v.size();
    for (auto i = 0u; i < orig_size; ++i)
    {
        std::string s = std::to_string(v[i]);
        unsigned long digits = s.size();

        if (v[i] == 0)
        {
            v[i] = 1;
        }
        else if (digits % 2 == 0)
        {
            v.push_back(stoull(s.substr(0, digits / 2)));
            v[i] = stoull(s.substr(digits / 2));
            // v[i] = v[i] / pow(10, digits / 2);
        }
        else
        {
            v[i] = v[i] * 2024;
        }
    }
}

template <typename T>
void print(std::vector<T>& v)
{
    for (auto &x : v)
    {
        std::cout << x << " ";
    }
    std::cout << std::endl;
}

int main(int argc, char** argv)
{
    std::ifstream f(argv[1]);
    std::string line;

    std::vector<unsigned long int> v;

    if (f.is_open())
    {
        std::getline(f, line);

        std::stringstream ss(line);
        int i;
        while (!ss.eof())
        {
            ss >> i;
            v.push_back(i);
        }
    }


    int blink_time = std::stoi(std::string(argv[2]));
    print(v);
    for (auto i = 0u; i < blink_time; ++i)
    {
        blink(v);
        // blink_t(v);
        // print(v);
        // std::cout << std::endl;
    }

    print(v);
    std::cout << std::endl;

    // std::cout << "stones count = " << v.size() << std::endl;

    std::cout << stoull(std::string("00")) << std::endl;

    return 0;
}