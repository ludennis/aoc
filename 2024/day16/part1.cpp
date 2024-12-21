#include <fstream>
#include <string>
#include <iostream>
#include <vector>
#include <set>
#include <limits>
#include <map>
#include <algorithm>
#include <sstream>
#include <optional>
#include <queue>

template <typename T>
using Matrix = std::vector<std::vector<T>>;

using Pair = std::pair<int, int>;

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

void print(const Matrix<std::optional<int>>& m)
{
    for (auto& v : m)
    {
        for (auto& x : v)
        {
            std::cout << x.value_or(0) << ",";
        }
        std::cout << std::endl;
    }
}

void reset(Matrix<char>& m)
{
    m = std::vector<std::vector<char>>(m.size(), std::vector<char>(m[0].size(), '.'));
}

template <typename T>
void init(Matrix<T>& m, int row, int col, T t)
{
    m = std::vector<std::vector<T>>(row, std::vector<T>(col, t));
}

void reset(Matrix<char>& m, const std::set<Pair>& walls, const Pair& s, const Pair& e)
{
    reset(m);
    for (auto& w : walls)
    {
        m[w.first][w.second] = '#';
    }
    m[s.first][s.second] = 'S';
    m[e.first][e.second] = 'E';
}

char turn_clockwise(char dir)
{
    switch (dir)
    {
        case '^':
            return '>';
        case '>':
            return 'v';
        case 'v':
            return '<';
        case '<':
            return '^';
        default:
            return ' ';
    }
}

char turn_counterclockwise(char dir)
{
    switch(dir)
    {
        case '^':
            return '<';
        case '<':
            return 'v';
        case 'v':
            return '>';
        case '>':
            return '^';
        default:
            return ' ';
    }
}

void step(Pair& p, char dir)
{
    switch(dir)
    {
        case '^':
            p.first = p.first - 1;
            break;
        case '>':
            p.second = p.second + 1;
            break;
        case 'v':
            p.first = p.first + 1;
            break;
        case '<':
            p.second = p.second - 1;
            break;
        default:
            break;
    }
}

void step_back(Pair& p, char dir)
{
    switch(dir)
    {
        case '^':
            p.first = p.first + 1;
            break;
        case '>':
            p.second = p.second - 1;
            break;
        case 'v':
            p.first = p.first - 1;
            break;
        case '<':
            p.second = p.second + 1;
            break;
        default:
            break;
    }
}

Pair peek(const Pair& p, char dir)
{
    auto p_ = p;
    step(p_, dir);
    return p_;
}

Pair prev(const Pair& p, char dir)
{
    auto p_ = p;
    step_back(p_, dir);
    return p_;
}

std::string get_str(const Pair& p)
{
    std::stringstream ss;
    ss << "(" << p.first << "," << p.second << ")";
    return ss.str();
}

struct Cell
{
    Pair p;
    char dir;
    int score;
};

int traverse(const Pair& s, const Pair& e, const std::set<Pair>& walls, Matrix<std::optional<int>>& scores)
{
    std::map<Pair, std::set<char>> visited;

    std::queue<Cell> next;

    next.push(Cell{peek(s, '>'), '>', 1});
    next.push(Cell{peek(s, '^'), '^', 1001});
    next.push(Cell{peek(s, 'v'), 'v', 1001});

    while (!next.empty())
    {

        auto p = next.front().p;
        auto dir =  next.front().dir;
        auto score = next.front().score;

        // std::cout << "checking p " << get_str(p) << ", dir = " << dir << std::endl;

        if (walls.find(next.front().p) != walls.end())
        {
            next.pop();
            continue;
        }

        if (!scores[p.first][p.second])
            scores[p.first][p.second] = score;
        else
        {
            if (score < scores[p.first][p.second])
                scores[p.first][p.second] = score;
            else
            {
                next.pop();
                continue;
            }
        }

        next.push(Cell{peek(p, dir), dir, score + 1});
        next.push(Cell{peek(p, turn_clockwise(dir)), turn_clockwise(dir), score + 1001});
        next.push(Cell{peek(p, turn_counterclockwise(dir)), turn_counterclockwise(dir), score + 1001});

        next.pop();
    }
    
    return scores[e.first][e.second].value_or(-1);
}

int main(int argc, char** argv)
{
    std::ifstream f(argv[1]);
    std::string line;

    Matrix<char> map;
    std::set<Pair> walls;
    Pair start;
    Pair end;

    if (f.is_open())
    {
        while (std::getline(f, line))
        {
            map.push_back(std::vector<char>());
            for (auto& c : line)
            {
                map.back().push_back(c);
                if (c == '#')
                {
                    walls.insert(Pair(map.size() - 1, map.back().size() - 1));
                }
                else if (c == 'S')
                {
                    start = Pair(map.size() - 1, map.back().size() - 1);
                }
                else if (c == 'E')
                {
                    end = Pair(map.size() - 1, map.back().size() - 1);
                }
            }
        }
    }


    reset(map, walls, start, end);
    print(map);

    Matrix<std::optional<int>> scores;
    init(scores, map.size(), map[0].size(), std::optional<int>());
    auto lowest_score = traverse(start, end, walls, scores);

    std::cout << "lowest score of all paths = " << lowest_score << std::endl;

    // print(scores);
    return 0;
}