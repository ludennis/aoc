#include <fstream>
#include <string>
#include <iostream>
#include <vector>
#include <sstream>
#include <mutex>
#include <thread>
#include <memory>
#include <atomic>

#define CLEAR_SCREEN "\033[2J\033[1;1H"

#define RESET "\033[0m"
#define BLACK "\033[30m"
#define GREEN "\033[42m"

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
            if (x == 1)
                std::cout << GREEN << x << RESET;
            else
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

void step(Pair& p, Pair& v, Pair& grid_size, int step=1)
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

void write_to_file(const Matrix<int>& m, const std::string& name)
{
    std::ofstream f(name);

    for (auto& v : m)
    {
        for (auto& x : v)
        {
            if (x == 1)
                f << '1';
            else
                f << '0';
        }
    }
}

// bool has_vertical_line(const Matrix<int>& m)
// {

// }

// bool has_horizontal_line(const Matrix<int>& m)
// {

// }

int which_quadrant(const Pair& pose, const Pair& grid_size)
{
    if (pose.first >= 0 && pose.first < (grid_size.first / 2) &&
    pose.second >= 0 && pose.second < (grid_size.second / 2))
    {
        return 0;
    }
    // top-right
    else if (pose.first > (grid_size.first / 2) && pose.first < grid_size.first &&
        pose.second >= 0 && pose.second < grid_size.second / 2)
    {
        return 1;
    }
    // bottom-left
    else if (pose.first >= 0 && pose.first < (grid_size.first / 2) &&
        pose.second > (grid_size.second / 2) && pose.second < grid_size.second)
    {
        return 2;
    }
    // bottom-right
    else if (pose.first > (grid_size.first / 2) && pose.first < grid_size.first &&
        pose.second > (grid_size.second / 2) && pose.second < grid_size.second)
    {
        return 3;
    }

    return -1;
}

int main(int argc, char** argv)
{
    std::ifstream f(argv[1]);
    std::string line;

    auto grid_size = Pair(std::stoi(argv[2]), std::stoi(argv[3]));
    std::vector<std::pair<Pair, Pair>> robot_poses;
    // SafeVector<Pair> robot_poses;
    int t = std::stoi(argv[4]);
    // int start_steps = std::stoi(argv[5]);

    // std::cout << start_steps << std::endl;
    // size_t max_thread_num = 10;
    // std::vector<std::unique_ptr<std::thread>> threads;
    // std::atomic<size_t> thread_num = 0;


    if (f.is_open())
    {
        while (std::getline(f, line))
        {
            Pair p;
            Pair v;

            parse_line(line, p, v);

            robot_poses.emplace_back(std::pair<Pair,Pair>(std::move(p), std::move(v)));
        }
    }


    // for (auto& t : threads)
    //     t->join();


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


    // for (auto& pose : robot_poses)
    //     step(pose.first, pose.second, grid_size, start_steps);

    for (auto& pose : robot_poses)
    {
        m[pose.first.second][pose.first.first]++;
    }

    for (auto steps = 0u; steps < t; ++steps)
    {
        quadrants = std::vector<int>(4, 0);
        for (auto& pose : robot_poses)
        {
            m[pose.first.second][pose.first.first]--;

            step(pose.first, pose.second, grid_size);

            auto q = which_quadrant(pose.first, grid_size);
            if (q >= 0)
                quadrants[q]++;
            // quadrants[which_quadrant(pose.first, grid_size)]++;

            m[pose.first.second][pose.first.first]++;        
        }

        for (auto& q : quadrants)
        {
            if (q > robot_poses.size() / 2)
            {
                print(m);
                std::cout << "Step = " << steps << std::endl;
            }
        }
    }


    return 0;
}