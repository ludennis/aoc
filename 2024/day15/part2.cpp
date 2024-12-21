#include <fstream>
#include <string>
#include <iostream>
#include <vector>
#include <set>
#include <sstream>
#include <thread>
#include <chrono>

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

void reset(Matrix<char>& m, Pair& robot, std::set<Pair>& walls, std::set<Pair>& l_boxes, std::set<Pair>& r_boxes)
{
    reset(m);
    m[robot.first][robot.second] = '@';
    for (auto& p : walls)
        m[p.first][p.second] = '#';
    for (auto& p : l_boxes)
        m[p.first][p.second] = '[';
    for (auto& p : r_boxes)
        m[p.first][p.second] = ']';
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

// no more boxes in next row
bool is_final_row(const std::vector<Pair>& row, char dir, std::set<Pair>& l_boxes, std::set<Pair>& r_boxes)
{
    for (auto& p : row)
    {
        auto peeked = peek(p, dir);

        if (l_boxes.find(peeked) != l_boxes.end() || r_boxes.find(peeked) != r_boxes.end())
            return false;
    }

    return true;
}

bool has_space_to_move(const std::vector<Pair>& row, char dir, std::set<Pair>& walls)
{
    for (auto& p : row)
    {
        auto peeked = peek(p, dir);

        if (walls.find(peeked) != walls.end())
            return false;
    }

    return true;
}

void move(Pair& robot, char dir, std::set<Pair>& walls, std::set<Pair>& l_boxes, std::set<Pair>& r_boxes)
{
    auto peeked = peek(robot, dir);

    if (walls.find(peeked) != walls.end())
        return;

    if (walls.find(peeked) == walls.end() && l_boxes.find(peeked) == l_boxes.end() &&
            r_boxes.find(peeked) == r_boxes.end())
    {
        step(robot, dir);
        return;
    }

    if ((dir == '<' && r_boxes.find(peeked) != r_boxes.end()) ||
            dir == '>' && l_boxes.find(peeked) != l_boxes.end())
    {
        // pushing box(es) to the left or right
        std::vector<Pair> visited;
        for (auto p = peeked;; step(p, dir))
        {
            std::cout << "peeked " << get_str(p) << std::endl;
            if (l_boxes.find(p) != l_boxes.end() || r_boxes.find(p) != r_boxes.end())
                visited.push_back(p);
            if (walls.find(p) != walls.end())
                return;

            if (walls.find(p) == walls.end() && l_boxes.find(p) == l_boxes.end() && r_boxes.find(p) == r_boxes.end())
            {
                // has place to move all
                step(robot, dir);
                for (int i = visited.size() - 1; i >= 0; --i)
                {
                    std::cout << "moving box" << get_str(visited[i]) << std::endl;
                    if (walls.find(peek(visited[i], dir)) == walls.end())
                    {
                        if (l_boxes.find(visited[i]) != l_boxes.end())
                        {
                            l_boxes.erase(l_boxes.find(visited[i]));
                            l_boxes.insert(peek(visited[i], dir));
                        }

                        if (r_boxes.find(visited[i]) != r_boxes.end())
                        {
                            r_boxes.erase(r_boxes.find(visited[i]));
                            r_boxes.insert(peek(visited[i], dir));
                        }

                    }
                }

                return;
            }
        }

        return;
    }

    if ((dir == 'v' || dir == '^') && (l_boxes.find(peeked) != l_boxes.end() || r_boxes.find(peeked) != r_boxes.end()))
    {
        std::vector<Pair> row;
        std::vector<Pair> final_row;
        std::vector<Pair> visited;

        row.push_back(peeked);
        if (l_boxes.find(peeked) != l_boxes.end())
            row.push_back(Pair(peeked.first, peeked.second + 1));
        else
            row.push_back(Pair(peeked.first, peeked.second - 1));

        while (!row.empty())
        {
            // check each row of boxes
            std::vector<Pair> next_to_visit;

            std::cout << "visiting row = ";
            for (auto& p : row)
            {
                visited.push_back(p);
                std::cout << get_str(p);

                auto p_ = peek(p, dir);

                if (walls.find(p_) != walls.end())
                    return;

                if (l_boxes.find(p_) != l_boxes.end())
                { // peeked is [
                    next_to_visit.push_back(p_);
                    next_to_visit.push_back(Pair(p_.first, p_.second + 1));
                }
                else if (r_boxes.find(p_) != r_boxes.end())
                { // peeked is ]
                    next_to_visit.push_back(p_);
                    next_to_visit.push_back(Pair(p_.first, p_.second - 1));
                }
            } std::cout << std::endl;

            if (!next_to_visit.empty())
            {
                row.clear();
                for (auto& p : next_to_visit)
                    row.push_back(p);
            }
            else
            {
                if (is_final_row(row, dir, l_boxes, r_boxes))
                {
                    if (has_space_to_move(row, dir, walls))
                    {
                        for (int i = visited.size() - 1; i >= 0; --i)
                        {
                            std::cout << "moving box " << get_str(visited[i]) << std::endl;
                            if (l_boxes.find(visited[i]) != l_boxes.end())
                            {
                                l_boxes.erase(l_boxes.find(visited[i]));
                                l_boxes.insert(peek(visited[i], dir));
                            }
                            if (r_boxes.find(visited[i]) != r_boxes.end())
                            {
                                r_boxes.erase(r_boxes.find(visited[i]));
                                r_boxes.insert(peek(visited[i], dir));
                            }
                        }

                        step(robot, dir);
                        return;
                    }
                }

                break;
            }
        }
    }
}

int main(int argc, char** argv)
{
    std::ifstream f(argv[1]);
    std::string line;

    Matrix<char> map;
    std::vector<char> sequence;

    std::set<Pair> l_boxes;
    std::set<Pair> r_boxes;
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
                    // map.back().push_back(c);

                    if (c == 'O')
                    {
                        map.back().push_back('[');
                        l_boxes.insert(Pair(map.size() - 1, map.back().size() - 1));
                        map.back().push_back(']');
                        r_boxes.insert(Pair(map.size() - 1, map.back().size() - 1));
                    }
                    else if (c == '#')
                    {
                        map.back().push_back('#');
                        walls.insert(Pair(map.size() - 1, map.back().size() - 1));
                        map.back().push_back('#');
                        walls.insert(Pair(map.size() - 1, map.back().size() - 1));
                    }
                    else if (c == '@')
                    {
                        map.back().push_back('@');
                        robot = Pair(map.size() - 1, map.back().size() -1 );
                        map.back().push_back('.');
                    }
                    else
                    {
                        map.back().push_back('.');
                        map.back().push_back('.');
                    }
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

    reset(map, robot, walls, l_boxes, r_boxes);
    print(map);

    for (auto& dir : sequence)
    {
        move(robot, dir, walls, l_boxes, r_boxes);

        // reset(map, robot, walls, l_boxes, r_boxes);
        // print(map);

        // std::this_thread::sleep_for(std::chrono::milliseconds(500));
    } std::cout << std::endl;


    reset(map, robot, walls, l_boxes, r_boxes);
    print(map);

    auto sum = 0;
    for (auto& gps : l_boxes)
    {
        sum += gps.first * 100 + gps.second;
    }
    std::cout << "sum = " << sum << std::endl;
}