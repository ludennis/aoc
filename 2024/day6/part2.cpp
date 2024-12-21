#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <atomic>
#include <thread>
#include <chrono>
#include <queue>
#include <memory>
#include <mutex>
#include <map>
#include <set>

using Matrix = std::vector<std::vector<char>>;
using Pair = std::pair<int,int>;

void Print(const Matrix& m)
{
    for (auto& v : m)
    {
        for (auto& i : v)
            std::cout << i;
        std::cout << std::endl;
    }
}

bool IsIn(const Matrix& m, const Pair& p)
{
    if (p.first >= 0 && p.first < m.size() && p.second >= 0 && p.second < m[0].size())
        return true;
    return false;
}

char Get(const Matrix& m, const Pair& p)
{
    if (IsIn(m, p))
        return m[p.first][p.second];
    return ' ';
}

void Set(Matrix& m, const Pair& p, char c)
{
    if (IsIn(m, p))
        m[p.first][p.second] = c;
}

Pair Peek(const Matrix& m, const Pair& p)
{
    switch(Get(m, p))
    {
        case '^':
            return Pair(p.first - 1, p.second);
        case '<':
            return Pair(p.first, p.second - 1);
        case 'v':
            return Pair(p.first + 1, p.second);
        case '>':
            return Pair(p.first, p.second + 1);
    }

    return Pair(-1, -1);
}

bool Step(Matrix& m, Pair& p)
{
    auto peek = Peek(m, p);

    if (!IsIn(m, peek))
        return false;

    if (m[peek.first][peek.second] == '#')
    {
        // std::cout << "peeked an obstacle, turning" << std::endl;
        switch (Get(m, p))
        {
            case '^':
                m[p.first][p.second] = '>';
                break;
            case '<':
                m[p.first][p.second] = '^';
                break;
            case 'v':
                m[p.first][p.second] = '<';
                break;
            case '>':
                m[p.first][p.second] = 'v';
                break;
        }
    }
    else
    {
        m[peek.first][peek.second] = m[p.first][p.second];
        p = peek;
    }

    return true;
}

struct SafeSet
{
    std::mutex lock;
    std::set<Pair> s;
};

// m has added obstacle already
bool CheckPatrollable(Matrix m, Pair p)
{
    std::map<Pair, std::set<char>> visited;

    while(IsIn(m, p))
    {
        if (visited.find(p) == visited.end())
            visited[p].insert(Get(m, p));
        else
        {
            if (visited[p].find(Get(m, p)) == visited[p].end())
                visited[p].insert(Get(m, p));
            else
                return false;
        }

        if (!Step(m, p))
            return true;
    }

    return true;
}

bool PlaceObstacle(Matrix& m, Pair& p)
{
    if (Get(m, p) == '#')
        return false;

    char c = m[p.first][p.second];
    Set(m, p, '#');

    return true;
}

void RestoreObstacle(Matrix& m, Pair& p, char c)
{
    if (!IsIn(m, p))
        return;

    m[p.first][p.second] = c;
}

void Patrol(Matrix& m, Pair& pos, std::set<Pair>& visited, SafeSet& obstacles)
{
    std::atomic<int> thread_count = 0;

    std::vector<std::unique_ptr<std::thread>> threads;
    int max_thread = 50000;

    while (IsIn(m, pos))
    {
        if (visited.find(pos) == visited.end())
            visited.insert(pos);

        std::cout << "Patrol pos(x,y) = " << pos.first << "," << pos.second << ", total visited = " << visited.size() << std::endl;

        while (thread_count >= max_thread)
        {
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }

        thread_count+= 1;
        auto obstacle = Peek(m, pos);
        if (IsIn(m, obstacle) && visited.find(obstacle) == visited.end())
        {
            // std::cout << "PEEEEEEEEEEKKKKK" << std::endl;
            auto restore = m[obstacle.first][obstacle.second];

            if (PlaceObstacle(m, obstacle))
            {
                threads.push_back(std::make_unique<std::thread>([m, pos, obstacle, &thread_count, &obstacles]()
                {
                    if (!CheckPatrollable(std::move(m), std::move(pos)))
                    {
                        std::cout << "NOT PATROLLABLE!" << std::endl;
                        std::lock_guard<std::mutex> m(obstacles.lock);
                        obstacles.s.insert(obstacle);
                        std::cout << "thread pos(x,y) = ("<< pos.first << "," << pos.second << "), obstruction_count = " << obstacles.s.size() << std::endl;
                    }

                    thread_count -= 1;
                }));
                RestoreObstacle(m, obstacle, restore);
            }
        }

        if(!Step(m, pos))
            break;
    }

    for (auto& t : threads)
        t->join();
}

int main(int argc, char** argv)
{
    std::ifstream f("input.txt");
    std::string line;

    Matrix m; 

    if (f.is_open())
    {
        while (std::getline(f, line))
        {
            m.push_back(std::vector<char>());
            
            for (auto& c : line)
                m.back().push_back(c);
        }
    }

    Pair start;

    for (auto i = 0u; i < m.size(); ++i)
    {
        for (auto j = 0u; j < m[i].size(); ++j)
        {
            auto& c = m[i][j];
            if (c == '<' || c == '^' || c == '>' || c == 'v')
            {
                start = std::make_pair<int,int>(i, j);
            }
        }
    }

    Matrix origin_m = m;

    auto pos = Pair(start);
    SafeSet obstacles;
    std::set<Pair> visited;

    Patrol(m, pos, visited, obstacles);


    m[start.first][start.second] = 'G';
    Print(m);
    // for (auto i = 0u; i < m.size(); ++i)
    // {
    //     for (auto j = 0u; j < m[0].size(); ++j)
    //     {
    //         std::cout << m[i][j];
    //     }
    //     std::cout << std::endl;
    // }
    std::cout << std::endl << std::endl;

    for (auto& p : obstacles.s)
    {
        Set(origin_m, p, 'O');
    }

    Print(origin_m);
    // for (auto i = 0u; i < origin_m.size(); ++i)
    // {
    //     for (auto j = 0u; j < origin_m[0].size(); ++j)
    //     {
    //         std::cout << origin_m[i][j];
    //     }
    //     std::cout << std::endl;
    // }
    std::cout << std::endl << std::endl;


    std::cout << "visited = " << visited.size() << ", obstacles =  " << obstacles.s.size() << std::endl;

    return 0;
}