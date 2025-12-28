#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using Grid = std::vector<std::vector<char>>;

void print_grid(const Grid& g)
{
    for (auto const& v : g)
    {
        for (auto const& c : v)
        {
            std::cout << c;
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

int num_around(const Grid& g, int x, int y)
{
    int n{0};

    if ((y - 1) >= 0 && g[y - 1][x] == '@') n++; // check up
    if ((y + 1) < g.size() && g[y + 1][x] == '@') n++;  // check down
    if ((x - 1) >= 0 && g[y][x - 1] == '@') n++; // check left
    if ((x + 1) < g[0].size() && g[y][x + 1] == '@') n++; // check right
    if ((y - 1) >= 0 && (x - 1) >= 0 && g[y - 1][x - 1] == '@') n++; // check top-left
    if ((y - 1) >= 0 && (x + 1) < g[0].size() && g[y - 1][x + 1] == '@') n++; // check top-right
    if ((y + 1) < g.size() && (x - 1) >= 0 && g[y + 1][x - 1] == '@') n++; // check bottom-left
    if ((y + 1) < g.size() && (x + 1) < g[0].size() && g[y + 1][x + 1] == '@') n++; // check bottom-right

    return n;
}

int remove_rolls_once(Grid& grid)
{
    Grid answer;

    int removed{0};

    for (auto y{0}; y < grid.size(); ++y)
    {
        answer.push_back(std::vector<char>());

        for (auto x{0}; x < grid[0].size(); ++x)
        {

            if (grid[y][x] == '@' && num_around(grid, x, y) < 4)
            {
                removed++;

                answer.back().emplace_back('x');
            }
            else
                answer.back().emplace_back('.');
        }
    }

    for (auto y{0}; y < answer.size(); ++y)
        for (auto x{0}; x < answer[0].size(); ++x)
            if (answer[y][x] == 'x')
                grid[y][x] = '.';

    print_grid(answer);
    std::cout << "Rolls removed: " << removed << std::endl;

    return removed;
}

int main (int argc, char** argv)
{
    std::fstream file;
    if (argc > 1)
        file = std::fstream(argv[1]);
    else
        file = std::fstream("input.txt");

    std::string line;
    Grid grid;

    while (std::getline(file, line))
    {
        grid.push_back(std::vector<char>());

        for (auto const& c : line)
        {
            grid.back().emplace_back(c);
        }
    }

    print_grid(grid);

    auto total_rolls_removed{0};

    for (;;)
    {
        auto removed = remove_rolls_once(grid);
        if (removed <= 0)
            break;
        total_rolls_removed += removed;
    }

    print_grid(grid);

    std::cout << "Total rolls removed: " << total_rolls_removed << std::endl;

    return 0;
}
