#include <fstream>
#include <atomic>
#include <iostream>
#include <string>
#include <deque>

int FindX_MAS(const std::deque<std::string>& deq)
{
    if (deq.size() < 3)
        return 0;

    int n = 0;
    for (auto i = 1u; i < deq[1].size() - 1; ++i)
    {
        if (deq[1][i] == 'A')
        {
            if (((deq[0][i - 1] == 'M' && deq[2][i + 1] == 'S') ||
                (deq[0][i - 1] == 'S' && deq[2][i + 1] == 'M')) &&
                ((deq[0][i + 1] == 'M' && deq[2][i - 1] == 'S') ||
                (deq[0][i + 1] == 'S' && deq[2][i - 1] == 'M')))
                n++;
        }
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
                if (deq.size() < 3)
                    continue;
                else if (deq.size() > 3)
                    deq.pop_front();

                if (deq.size() == 3)
                    total += FindX_MAS(deq); 
            }
        }
        f.close();
    }

    std::cout << total << std::endl;

    return 0;
}