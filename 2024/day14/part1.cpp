#include <fstream>
#include <string>
#include <iostream>
#include <vector>
#include <sstream>
#include <mutex>
#include <thread>
#include <memory>
#include <atomic>

template <typename T>
using Matrix = std::vector<std::vector<T>>;

using Pair = std::pair<int,int>;

std::string get_str(const Pair& p)
{
    std::stringstream ss;
    ss << "(" << p.first << "," << p.second << ")";
    return ss.str();
}

template <typename T>
void print(const Matrix<T>& m)
{
    for (auto& v : m)
    {
        for (auto& x : v)
        {
            std::cout << x;
        }
        std::cout << std::endl;
    }
}

void parse_line(const std::string& line, Pair& p, Pair& v)
{
    auto left = 2;
    auto right = left;
    while (line[right] != ',')
    {
    right++;
    }

    p.first = std::stoi(line.substr(left, right - left));

    right++;
    left = right;
    while (line[right] != ' ')
    {
    right++;
    }

    p.second = std::stoi(line.substr(left, right - left));

    left = line.find("v=") + 2;
    right = left;
    while (line[right] != ',')
    {
    right++;
    }

    v.first = std::stoi(line.substr(left, right - left));
    v.second = std::stoi(line.substr(right + 1)); 
}

template <typename T>
struct SafeVector
{
    void push_back(T t)
    {
        std::lock_guard<std::mutex> lock(m);
        this->v.push_back(std::move(t));
    }

    T operator[](size_t i)
    {
        std::lock_guard<std::mutex> lock(m);
        return this->v[i];
    }

    size_t size()
    {
        std::lock_guard<std::mutex> lock(m);
        return this->v.size();
    }

    std::vector<T> v;
    std::mutex m;  
};

int main(int argc, char** argv)
{
    std::ifstream f(argv[1]);
    std::string line;

    auto grid_size = Pair(std::stoi(argv[2]), std::stoi(argv[3]));
    SafeVector<Pair> robot_poses;
    int t = 100;

    size_t max_thread_num = 10;
    std::vector<std::unique_ptr<std::thread>> threads;
    std::atomic<size_t> thread_num = 0;


    if (f.is_open())
    {
        while (std::getline(f, line))
        {
            Pair p;
            Pair v;

            parse_line(line, p, v);

            // std::cout << get_str(p) << ", " << get_str(v) << std::endl;

            // spawn robot and make it start moving
            while (thread_num >= max_thread_num)
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            }

            thread_num++;
            threads.emplace_back(std::make_unique<std::thread>([&thread_num, p, v, grid_size, t, &robot_poses]()mutable
            { 
                int step = 0;

                for (auto step = 0; step < t; ++step)
                {
                    p.first = p.first + v.first;
                    if (p.first < 0)
                        p.first = p.first + grid_size.first;
                    else if (p.first >= grid_size.first)
                        p.first = p.first - grid_size.first;
                      
                    p.second = p.second + v.second;
                    if (p.second < 0)
                        p.second = p.second + grid_size.second;
                    else if (p.second >= grid_size.second)
                        p.second = p.second - grid_size.second;
                }

                robot_poses.push_back(std::move(p));

                thread_num--;
            }));
        }
    }


    for (auto& t : threads)
        t->join();


    Matrix<int> m;

    for (auto i = 0u; i < grid_size.second; ++i)
    {
        m.push_back(std::vector<int>());
        for (auto j =0u; j < grid_size.first; ++j)
        {
            m.back().push_back(0);
        }
    }

    std::vector<int> quadrants(4, 0); // top-left, top-right, bottom-left, bottom-right

    for (auto i = 0u; i < robot_poses.size(); ++i)
    {
        auto pose = robot_poses[i];

        std::cout << "robot pose = " << get_str(pose) << std::endl;
        // top-left
        if (pose.first >= 0 && pose.first < (grid_size.first / 2) &&
            pose.second >= 0 && pose.second < (grid_size.second / 2))
        {
            quadrants[0]++;
            std::cout << "in top-left quadrant" << std::endl;
        }
        // top-right
        else if (pose.first > (grid_size.first / 2) && pose.first < grid_size.first &&
                pose.second >= 0 && pose.second < grid_size.second / 2)
        {
            quadrants[1]++;
            std::cout << "in top-right quadrant" << std::endl;
        }
        // bottom-left
        else if (pose.first >= 0 && pose.first < (grid_size.first / 2) &&
                pose.second > (grid_size.second / 2) && pose.second < grid_size.second)
        {
            quadrants[2]++;
            std::cout << "in bottom-left quadrant" << std::endl;   
        }
        // bottom-right
        else if (pose.first > (grid_size.first / 2) && pose.first < grid_size.first &&
                pose.second > (grid_size.second / 2) && pose.second < grid_size.second)
        {
            quadrants[3]++;
            std::cout << "in bottom-right quadrant" << std::endl;
        }


        m[pose.second][pose.first]++;
    }

    print(m);

    int safety_factor = 1;
    for (auto& q : quadrants)
    {
        std::cout << q << std::endl;
        safety_factor = (q > 0) ? safety_factor * q : safety_factor;
    }

    std::cout << "Safety factor = " << safety_factor << std::endl;

    return 0;
}