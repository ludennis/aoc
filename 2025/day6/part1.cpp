#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

#ifdef DEBUG
#define LOG(string, args...) printf(string, ##args)
#else
#define LOG(string, args...)
#endif

unsigned long long int calculate(char op, std::vector<int>&& v)
{
    auto sum{0ull};

    switch (op)
    {
        case '*':
            sum = 1;
            for (auto const& i : v)
                sum *= i;
            break;
        case '+':
            for (auto const& i : v)
                sum += i;
        default:
            break;
    }

    return sum;
}

int main(int argc, char** argv)
{
    std::fstream file;
    if (argc > 1)
        file = std::fstream(argv[1]);
    else
        file = std::fstream("input.txt");

    std::string line;

    std::vector<std::vector<int>> vv;

    while (std::getline(file, line))
    {
        std::istringstream ss(line);
        vv.push_back(std::vector<int>());

        if (file.eof())
        {
            char c;
            while (ss >> c)
                vv.back().emplace_back(c - '0');
        }
        else
        {
            int i;
            while (ss >> i)
                vv.back().emplace_back(i);
        }
    }

    for (auto i{0u}; i < vv.size() - 1; ++i)
    {
        LOG("Size of vv[%d] = %ld\n", i, vv[i].size());

        for (auto const& x : vv[i])
            LOG("%d, ", x);
        LOG("\n");
    }

    for (auto const& x : vv.back())
        LOG("%c, ", x + '0');
    LOG("\n");

    auto total{0ull};

    for (auto x{0ul}; x < vv[0].size(); ++x)
    {
        char op = vv.back()[x] + '0';

        auto sum{0ull};
        switch (op)
        {
            case '*':
                sum = 1ull;
                for (auto y{0ul}; y < vv.size() - 1; ++y)
                    sum *= vv[y][x];
                break;
            case '+':
                for (auto y{0ul}; y < vv.size() - 1; ++y)
                    sum += vv[y][x];
                break;
            default:
                break;
        }

        total += sum;
        LOG("Total = %lld, sum = %lld\n", total, sum);
    }

    std::cout << "Total = " << total << std::endl;
    return 0;
}
