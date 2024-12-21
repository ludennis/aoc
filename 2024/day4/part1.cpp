#include <fstream>
#include <atomic>
#include <iostream>
#include <string>
#include <deque>

int FindHorizontalXMAS(const std::string& s)
{
    int n = 0;
    size_t pos = 0;

    for (;;)
    {
        size_t found = s.find("XMAS", pos);
        if (found != std::string::npos)
        {
            pos = found + 1;
            n++;
        }
        else
        {
            break;
        }
    }

    pos = 0;
    for (;;)
    {
        size_t found = s.find("SAMX", pos);
        if (found != std::string::npos)
        {
            pos = found + 1;
            n++;
        }
        else
        {
            break;
        }
    }

    return n;
}

int FindVerticalXMAS(const std::deque<std::string>& deq)
{
    if (deq.size() < 4)
        return 0;

    int n = 0;

    // top down search
    for (auto i = 0u; i < deq[0].size(); ++i)
    {
        if (deq[0][i] == 'X' && deq[1][i] == 'M' && deq[2][i] == 'A' && deq[3][i] == 'S')
                n++;
    }

    // bottom up search
    for (auto i = 0u; i < deq[3].size(); ++i)
    {
        if (deq[3][i] == 'X' && deq[2][i] == 'M' && deq[1][i] == 'A' && deq[0][i] == 'S')
                n++;
    }
    return n;
}

int FindDiagonalXMAS(const std::deque<std::string>& deq)
{
    if (deq.size() < 4)
        return 0;

    int n = 0;

    // left down search
    for (auto i = 3u; i < deq[0].size(); ++i)
    {
        if (deq[0][i] == 'X' && deq[1][i - 1] == 'M' && deq[2][i - 2] == 'A' && deq[3][i - 3] == 'S')
            n++;
    }

    // right down search
    for (auto i = 0u; i < deq[0].size() - 3; ++i)
    {
        if (deq[0][i] == 'X' && deq[1][i + 1] == 'M' && deq[2][i + 2] == 'A' && deq[3][i + 3] == 'S')
            n++;
    }

    // left up search
    for (auto i = 3u; i < deq[3].size(); ++i)
    {
        if (deq[3][i] == 'X' && deq[2][i - 1] == 'M' && deq[1][i - 2] == 'A' && deq[0][i - 3] == 'S')
            n++;
    }

    // right up search
    for (auto i = 0u; i < deq[3].size() - 3; ++i)
    {
        if (deq[3][i] == 'X' && deq[2][i + 1] == 'M' && deq[1][i + 2] == 'A' && deq[0][i + 3] == 'S')
            n++;
    }

    return n;
}

int main(int argc, char **argv)
{

    std::string line;
    int total = 0;
    std::atomic<int> count;
    std::deque<std::string> deq;

    {
        std::ifstream f("input.txt");
        if (f.is_open())
        {
            while (getline(f, line))
            {
                deq.push_back(line);
                if (deq.size() < 4)
                    continue;
                else if (deq.size() > 4)
                    deq.pop_front();

                if (deq.size() == 4)
                    total += (FindVerticalXMAS(deq) + FindDiagonalXMAS(deq)); 
            }
        }
        f.close();
    }

    {
        std::ifstream f("input.txt");
        if (f.is_open())
        {
            while (getline(f, line))
            {
                total += FindHorizontalXMAS(line);   
            }
        }
        f.close();
    }

    std::cout << total << std::endl;

    return 0;
}