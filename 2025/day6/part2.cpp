#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

#ifdef DEBUG
#define LOG(string, args...) printf(string, ##args)
#else
#define LOG(string, args...)
#endif

int main(int argc, char** argv)
{
    std::fstream file;
    if (argc > 1)
        file = std::fstream(argv[1]);
    else
        file = std::fstream("input.txt");

    std::string line;

    std::vector<std::string> vv;

    while (std::getline(file, line))
    {
        vv.emplace_back(line);
    }

    for (auto const& s : vv)
        LOG("%s ------> size = %lld\n", s, s.size());

    auto total{0ull};
    std::vector<unsigned long long int> operands;
    char op{'.'};
    for (long long int x{static_cast<long long int>(vv[0].size()) - 1}; x >= 0ll; --x)
    {
        std::string operand;
        for (auto y{0ull}; y < vv.size(); ++y)
        {
            if (vv[y][x] == '+' || vv[y][x] == '*')
            {
                LOG("vv[y][x] == %c\n", vv[y][x]);

                op = vv[y][x];

            }
            else if (vv[y][x] >= '0' && vv[y][x] <= '9')
            {
                operand.push_back(vv[y][x]);
            }
        }

        if (!operand.empty())
        {
            operands.emplace_back(std::stoull(operand));
            LOG("Operand pushed: %lld\n", operands.back());
        }

        if (op != '.')
        {
            // calculate sum
            auto sum{0ull};
            if (op == '+')
            {
                for (auto const& n : operands)
                    sum += n;
            }
            else
            {
                sum = 1ull;
                for (auto const& n : operands)
                    sum *= n;
            }

            total += sum;
            LOG("Total = %lld, sum = %lld\n", total, sum);

            operands.clear();
            operands.shrink_to_fit();
            op = '.';
        }
    }

    std::cout << "Total = " << total << std::endl;

    return 0;
}
