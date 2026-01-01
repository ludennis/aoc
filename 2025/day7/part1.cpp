#include <fstream>
#include <future>
#include <iostream>
#include <string>
#include <thread>
#include <vector>

#ifdef DEBUG
#define LOG(string, args...) printf(string, ##args)
#else
#define LOG(string, args...)
#endif

using grid_type = std::vector<std::string>;
using pair_type = std::pair<int, int>;

std::atomic<unsigned int> splits{0u};

struct SafeGrid
{
    SafeGrid() = default;

    char& get(int x, int y) { std::lock_guard<std::mutex> lck(m); return grid[y][x]; }
    char& get(pair_type& p) { std::lock_guard<std::mutex> lck(m); return grid[p.second][p.first]; }
    std::string& get_row(int y) { std::lock_guard<std::mutex> lck(m); return grid[y]; }
    void set(int x, int y, char c) { std::lock_guard<std::mutex> lck(m); grid[y][x] = c; }
    void set(pair_type& p, char c) { std::lock_guard<std::mutex> lck(m); grid[p.second][p.first] = c; }
    void push_row(const std::string& line)
    {
           std::lock_guard<std::mutex> lck(m); grid.emplace_back(line);
    }

    void print()
    {
        std::lock_guard<std::mutex> lck(m);
        for (auto const s : grid)
            std::cout << s << std::endl;
    }

    unsigned int update_splits() { splits++; return splits; }

    size_t get_y_size() { std::lock_guard<std::mutex> lck(m); return grid.size(); }

    size_t get_x_size()
    {
        std::lock_guard<std::mutex> lck(m); return (grid.size() > 0) ? grid[0].size() : 0;
    }

    std::atomic<unsigned int> splits{0u};
    grid_type grid;
    std::mutex m;
};

void beam_and_split_f(SafeGrid* g, pair_type p)
{
    if (p.first < 0 || p.first >= g->get_x_size())
        return;

    while (p.second < g->get_x_size())
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        p.second += 1;

        if (g->get(p) == '|')
            return;

        if (g->get(p.first, p.second) == '^')
        {
            g->update_splits();

            g->set(p.first - 1, p.second, '|');
            auto ret_left = std::async(std::launch::async,
                    beam_and_split_f, g, pair_type(p.first - 1, p.second));
            g->set(p.first + 1, p.second, '|');
            auto ret_right = std::async(std::launch::async,
                    beam_and_split_f, g, pair_type(p.first + 1, p.second));

            return;
        }
        else
        {
            g->set(p, '|');
        }
    }
};

int main (int argc, char** argv)
{
    std::ifstream file;
    if (argc > 1)
        file = std::ifstream(argv[1]);
    else
        file = std::ifstream("input.txt");

    SafeGrid grid;

    std::string line;
    while (std::getline(file, line))
    {
        grid.push_row(line);
    }

    // spawn first thread to shoot beam from 'S'
    pair_type start;
    start.first = grid.get_row(0).find('S');
    start.second = 0;

    LOG("Starting at (%d, %d)\n", start.first, start.second);

    auto* grid_ptr = &grid;

    auto ret = std::async(std::launch::async, beam_and_split_f, grid_ptr, start);

    while (true)
    {

#ifndef DEBUG
        std::cout << "\033[H\033[2J" << std::flush;
#endif

        grid.print();
        std::cout << std::endl << "Splits: " << grid.splits << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    return 0;
}
