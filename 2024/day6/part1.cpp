#include <fstream>
#include <iostream>
#include <string>
#include <vector>

enum class Direction : char
{
    UP = '^',
    DOWN = 'v',
    LEFT = '<',
    RIGHT = '>'
};

void Step(std::vector<std::vector<char>>& m, std::pair<int, int>& p)
{
    Direction dir = static_cast<Direction>(m[p.first][p.second]);

    m[p.first][p.second] = 'X';

    if (dir == Direction::UP)
    {
        if (p.first - 1 >= 0)
        {
            if (m[p.first - 1][p.second] != '#')
            {
                m[p.first - 1][p.second] = '^';
                p.first = p.first - 1;
            }
            else
            {
                if (p.second + 1 < m[0].size())
                {
                    m[p.first][p.second + 1] = '>'; 
                    p.second = p.second + 1;
                }
                
            }
        }
    }
    else if (dir == Direction::LEFT)
    {
        if (p.second - 1 >= 0)
        {
            if (m[p.first][p.second - 1] != '#')
            {
                m[p.first][p.second - 1] = '<';
                p.second = p.second - 1;
            }
            else
            {
                if (p.first - 1 >= 0)
                {
                    m[p.first - 1][p.second] = '^';
                    p.first = p.first - 1;
                }
            }
        }
    }
    else if (dir == Direction::DOWN)
    {
        if (p.first + 1 < m.size())
        {
            if (m[p.first + 1][p.second] != '#')
            {
                m[p.first + 1][p.second] = 'v';
                p.first = p.first + 1;
            }
            else
            {
                if (p.second - 1 >= 0)
                {
                    m[p.first][p.second - 1] = '<';
                    p.second = p.second - 1;
                }
            }
        }
    }
    else // dir == Direction::RIGHT
    {
        if (p.second + 1 < m[0].size())
        {
            if (m[p.first][p.second + 1] != '#')
            {
                m[p.first][p.second + 1] = '>';
                p.second = p.second + 1;
            }
            else
            {
                if (p.first + 1 < m.size())
                {
                    m[p.first + 1][p.second] = 'v';
                    p.first = p.first + 1;
                }
            }
        }
    }
}

int main(int argc, char** argv)
{
    std::ifstream f("input.txt");
    std::string line;

    std::vector<std::vector<char>> m; 

    if (f.is_open())
    {
        while (std::getline(f, line))
        {
            m.push_back(std::vector<char>());
            
            for (auto& c : line)
                m.back().push_back(c);
        }
    }

    std::pair<int, int> start;
    Direction dir;

    for (auto i = 0u; i < m.size(); ++i)
    {
        for (auto j = 0u; j < m[i].size(); ++j)
        {
            auto& c = m[i][j];
            if (c == '<' || c == '^' || c == '>' || c == 'v')
            {
                start = std::make_pair<int,int>(i, j);
                dir = static_cast<Direction>(c);
            }
        }
    }




    auto pos = std::pair<int,int>(start);
    while (m[pos.first][pos.second] != 'X')
    {
        Step(m, pos);
    }

    int num_x = 0;
    for (auto i = 0u; i < m.size(); ++i)
    {
        for (auto j = 0u; j < m[i].size(); ++j)
        {
            auto& c = m[i][j];
            if (c == 'X')
                num_x++;
            std::cout << c;
        }
        std::cout << std::endl;
    }

    std::cout << "starting point = (" << start.first << "," << start.second << ")" << std::endl;
    std::cout << "direction = " << static_cast<char>(dir) << std::endl;

    std::cout << "end point = (" << pos.first << "," << pos.second << ")" << std::endl;
    std::cout << "num of X = " << num_x << std::endl;
    
    return 0;
}