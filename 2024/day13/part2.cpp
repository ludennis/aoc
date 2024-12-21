#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <limits>

using Pair = std::pair<long int, long int>;

bool has_no_remainder(long int a, long int b)
{
    a = labs(a);
    b = labs(b);

    return a % b == 0;
}

bool is_prize_reachable(const Pair& a, const Pair& b, const Pair& prize)
{
    std::cout << "a = " << a.first << "," << a.second << ", b = " << b.first << "," << b.second <<
            ", prize = " << prize.first << "," << prize.second << std::endl;

    return has_no_remainder(prize.first * b.second - b.first * prize.second,
            b.second * a.first - a.second * b.first);
}


long int win_prize_with_minimum_token(const Pair& a, const Pair& b, const Pair& prize)
{
    if (!is_prize_reachable(a, b, prize))
        return 0;

    long int an = (prize.first * b.second - b.first * prize.second) /
            (b.second * a.first - a.second * b.first);
    long int bn = (prize.first - a.first * an) / b.first;

    std::cout << "an = " << an << ", bn = " << bn << std::endl;

    return an * 3 + bn;   
}

int main(int argc, char** argv)
{
    std::ifstream f(argv[1]);
    std::string line;
    std::vector<Pair> A;
    std::vector<Pair> B;
    std::vector<Pair> prizes;

    if (f.is_open())
    {
        while (std::getline(f, line))
        {
            if (line.find("A") != std::string::npos)
            {
                std::cout << line << std::endl;

                A.emplace_back(Pair(stoi(line.substr(12, 2)), stoi(line.substr(18, 2))));
            }
            else if (line.find("B") != std::string::npos)
            {
                B.emplace_back(Pair(stoi(line.substr(12, 2)), stoi(line.substr(18, 2))));
            }
            else if (line.find("Prize") != std::string::npos)
            {
                std::cout << line << std::endl;

                auto left = line.find("X=") + 2;
                auto right = left;
                while (line[right] != ',')
                {
                    ++right;
                }

                prizes.emplace_back(Pair(stoi(line.substr(line.find("X=") + 2, right - left)),
                        stoi(line.substr(line.find("Y=") + 2))));
                prizes.back().first += 10000000000000;
                prizes.back().second += 10000000000000;
            }
        }
    }

    std::cout << std::endl;
    long int sum = 0;
    for (auto i = 0u; i < A.size(); ++i)
    {
        sum += win_prize_with_minimum_token(A[i], B[i], prizes[i]);
    }

    std::cout << sum << std::endl;

    return 0;
}