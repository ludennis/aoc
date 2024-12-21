#include <fstream>
#include <string>
#include <iostream>
#include <vector>
#include <set>
#include <sstream>

template <typename T>
using Matrix = std::vector<std::vector<T>>;

using Pair = std::pair<int,int>;

std::string get_str(const Pair& p)
{
    std::stringstream ss;
    ss << "(" << p.first << "," << p.second << ")";
    return ss.str();
}

void reset(Matrix<char>& m)
{
    m = std::vector<std::vector<char>>(m.size(), std::vector<char>(m[0].size(), '.'));
}

void reset(Matrix<char>& m, Pair& robot, std::set<Pair>& walls, std::set<Pair>& boxes)
{
    reset(m);
    m[robot.first][robot.second] = '@';

    for (auto& p : walls)
        m[p.first][p.second] = '#';

    for (auto& p : boxes)
        m[p.first][p.second] = 'O';
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

void step(Pair& p, char dir)
{
    switch (dir)
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
            return;
    }
}

Pair peek(const Pair& p, char dir)
{
    auto p_ = p;
    step(p_, dir);
    return p_;
}

void move(Pair& robot, char dir, std::set<Pair>& walls, std::set<Pair>& boxes)
{
    auto peeked = peek(robot, dir);

    if (walls.find(peeked) != walls.end())
        return;

    if (walls.find(peeked) == walls.end() && boxes.find(peeked) == boxes.end())
    {
        step(robot, dir);
        return;    
    }

    if (boxes.find(peeked) != boxes.end())
    {
        std::cout << "robot"<< get_str(robot) << " pushing box" << get_str(peeked) << ", dir = " << dir << std::endl;
        std::vector<Pair> path;
        for (auto p = peeked;; step(p, dir))
        {
            std::cout << "peeked " << get_str(p) << std::endl;
            if (boxes.find(p) != boxes.end())
                path.push_back(p);
            if (walls.find(p) != walls.end())
                return;

            if (walls.find(p) == walls.end() && boxes.find(p) == boxes.end())
            {
                // has place to move all
                step(robot, dir);
                for (int i = path.size() - 1; i >= 0; --i)
                {
                    std::cout << "moving box" << get_str(path[i]) << std::endl;
                    if (walls.find(peek(path[i], dir)) == walls.end())
                    {
                        boxes.erase(boxes.find(path[i]));
                        boxes.insert(peek(path[i], dir));
                    }
                }

                return;
            }

        }
    }

    // if boxes, push all connecting boxes if no wall
}

int main(int argc, char** argv)
{
    std::ifstream f(argv[1]);
    std::string line;

    Matrix<char> map;
    std::vector<char> sequence;

    std::set<Pair> boxes;
    std::set<Pair> walls;
    Pair robot;

    if (f.is_open())
    {
        while (std::getline(f, line))
        {
            if (line[0] == '#')
            {
                map.push_back(std::vector<char>());
                for (auto& c : line)
                {
                    map.back().push_back(c);

                    if (c == 'O')
                        boxes.insert(Pair(map.size() - 1, map.back().size() - 1));
                    else if (c == '#')
                        walls.insert(Pair(map.size() - 1, map.back().size() - 1));
                    else if (c == '@')
                        robot = Pair(map.size() - 1, map.back().size() -1 );
                }
            }
            else if (line[0] == '<' || line[0] == '^' || line[0] == '>' || line[0] == 'v')
            {
                for (auto& c : line)
                {
                    sequence.push_back(c);
                }
            }      
        }
    }

    // reset(map);
    print(map);

    for (auto& dir : sequence)
    {
        move(robot, dir, walls, boxes);
    } std::cout << std::endl;


    reset(map, robot, walls, boxes);
    print(map);

    auto sum = 0;
    for (auto& gps : boxes)
    {
        sum += gps.first * 100 + gps.second;
    }
    std::cout << "sum = " << sum << std::endl;
}