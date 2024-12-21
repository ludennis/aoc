#include <fstream>
#include <string>
#include <iostream>
#include <vector>
#include <set>

void print(const std::vector<std::vector<char>>& m)
{
    for (auto& v : m)
    {
        for (auto& c : v)
        {
            std::cout << c;
        }
        std::cout << std::endl;
    }
}

void print(const std::pair<int,int>& p)
{
    std::cout << "(" << p.first << "," << p.second << ")" << std::endl;
}

bool in_bound(const std::vector<std::vector<char>>& m, std::pair<int,int>& p)
{
    if (p.first >= 0 && p.first < m.size() && p.second >= 0 && p.second < m[0].size())
        return true;
    return false;
}

char get(const std::vector<std::vector<char>>& m, std::pair<int,int>& p)
{
    if (in_bound(m, p))
        return m[p.first][p.second];
    return ' ';
}

std::pair<int,int> project(std::pair<int,int>& from, std::pair<int,int>& to)
{
    auto i = to.first - from.first;
    auto j = to.second - from.second;

    return std::make_pair<int,int>(from.first + 2 * i, from.second + 2 * j);
}

void fa_(const std::vector<std::vector<char>>& m, std::vector<std::vector<char>>& a, std::pair<int,int> cur, std::pair<int,int>& start, std::set<std::pair<int,int>>& visited)
{
    // std::cout << "cur = (" << cur.first << "," << cur.second << ")" << std::endl;
    if (!in_bound(m, cur))
        return;

    if (cur != start && get(m, cur) == get(m, start))
    {
        // std::cout << "start = "; print(start);
        // std::cout << "cur = "; print(cur);
        auto a1 = project(start, cur);
        auto a2 = project(cur, start);
        // std::cout << "antinode = "; print(antinode);
        if (in_bound(m, a1))
            a[a1.first][a1.second] = '#';

        if (in_bound(m, a2))
            a[a2.first][a2.second] = '#';
    }
    
    visited.insert(cur);

    #ifndef FA_RECUR
    #define FA_RECUR \
        if (in_bound(m, cur) && visited.find(cur) == visited.end()) \
            { fa_(m, a, cur, start, visited); }

    // top left
    cur.second = cur.second - 1;
    cur.first = cur.first - 1;
    FA_RECUR

    // top
    cur.second = cur.second + 1;
    FA_RECUR

    // top right
    cur.second = cur.second + 1;
    FA_RECUR

    // right
    cur.first = cur.first + 1;
    FA_RECUR

    // down right
    cur.first = cur.first + 1;
    FA_RECUR

    // down
    cur.second = cur.second - 1;
    FA_RECUR

    // down left
    cur.second = cur.second - 1;
    FA_RECUR

    // left
    cur.first = cur.first - 1;
    FA_RECUR
    
    #endif // FA_RECUR
}

void find_antinode(const std::vector<std::vector<char>>& m, std::vector<std::vector<char>>& a, std::pair<int, int> start)
{
    std::set<std::pair<int,int>> visited;
    fa_(m, a, start, start, visited);
}

void find_antinodes(const std::vector<std::vector<char>>& m, std::vector<std::vector<char>>& a)
{
    for (auto i = 0u; i < m.size(); ++i)
    {
        for (auto j = 0u; j < m[0].size(); ++j)
        {
            if (m[i][j] != '.')
                find_antinode(m, a, std::pair<int,int>(i,j));
        }
    }
}

int main(int argc, char ** argv)
{
    std::ifstream f("input.txt");
    std::string line;

    std::vector<std::vector<char>> map;
    std::vector<std::vector<char>> antinodes;

    if (f.is_open())
    {
        while (std::getline(f, line))
        {
            map.emplace_back(std::vector<char>());
            antinodes.emplace_back(std::vector<char>());
            for (auto& c : line)
            {
                map.back().emplace_back(c);
                antinodes.back().emplace_back('.');
            }
        }
    }

    find_antinodes(map, antinodes);

    print(map);
    std::cout << std::endl;
    print(antinodes);

    std::pair<int,int> from(5, 5);
    std::pair<int,int> to(6, 7);

    auto cnt = 0;
    for (auto& v : antinodes)
        for (auto& x : v)
            cnt = (x == '#') ? cnt + 1 : cnt;

    std::cout << cnt << std::endl;
    return 0;
}