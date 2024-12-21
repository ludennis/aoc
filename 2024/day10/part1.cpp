#include <fstream>
#include <string>
#include <iostream>
#include <map>
#include <set>
#include <vector>
#include <queue>

using Pair = std::pair<int,int>;
using Matrix = std::vector<std::vector<int>>;

void print(const Matrix& m)
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

bool is_in(const Matrix& m, const Pair& p)
{
    if (p.first >= 0 && p.first < m.size() && p.second >= 0 && p.second < m[0].size())
        return true;
    return false;
}

int get(const Matrix& m, const Pair& p)
{
    return m[p.first][p.second];
}

void tr(const Matrix& m, const Pair p, int height, int& count, std::set<Pair>& visited)
{
    if (!is_in(m, p) || get(m, p) != height || visited.find(p) != visited.end())
        return;

    visited.insert(p);

    if (get(m, p) == 9)
    {
        count++;
        return;
    }

    // go up
    tr(m, Pair(p.first - 1, p.second), height + 1, count, visited);

    // go down
    tr(m, Pair(p.first + 1, p.second), height + 1, count, visited);

    // go left
    tr(m, Pair(p.first, p.second - 1), height + 1, count, visited);

    // go right
    tr(m, Pair(p.first, p.second + 1), height + 1, count, visited);
}

int tops_reachable(Matrix& m, Pair& start)
{
    std::set<Pair> visited;
    int count = 0;

    tr(m, start, 0, count, visited);

    return count;
}

int main(int argc, char** argv)
{
    std::ifstream f(argv[1]);
    std::string line;

    std::set<Pair> heads;
    Matrix map;

    if (f.is_open())
    {
        for (auto i = 0u; std::getline(f, line); ++i)
        {
            map.push_back(std::vector<int>());
            for (auto j = 0u; j < line.size(); ++j)
            {
                auto n = int(line[j] - '0');

                map.back().push_back(n);

                if (n == 0)
                {
                    auto p = Pair(i, j);
                    if (heads.find(p) == heads.end())
                    {
                        heads.insert(p);
                    }
                }
            }
        }
    }

    int sum = 0;

    for (auto start : heads)
    {
        sum += tops_reachable(map, start);
    }

    print(map);
    std::cout << std::endl;

    std::cout << sum << std::endl;
    
    return 0;
}