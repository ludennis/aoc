#include <cmath>
#include <fstream>
#include <iostream>
#include <vector>

using ulint = unsigned long int;
int main(int argc, char** argv)
{
    std::fstream file;
    if (argc > 1)
        file = std::fstream(argv[1]);
    else
        file = std::fstream("input.txt");

    std::string line;
    ulint sum{0u};

//    std::pair<int, int> p{1, 1};


    while (std::getline(file, line))
    {
        // 12 batteries to be enabled
        std::vector<int> v(12, 1);
        int index{0};

        for (auto i{0}; i < line.size(); ++i)
        {
            int n = line[i] - '0';

            if (n > v[index] && i < line.size() - (index + 1))
            {
                v[index] = n;

                if (index + 1 < v.size())
                {
                    v[index + 1] = 1;
                    index++;
                }

            }
        }

        ulint joltage{0u};

        for (auto i{0}; i < v.size(); ++i)
        {
            joltage += v[i] * std::pow(10, v.size() - i - 1);
        }

        std::cout << line << std::endl;
        std::cout << "joltage = " << joltage << std::endl;

        sum += joltage;
    }

    std::cout << "Total joltage is: " << sum << std::endl;

    return 0;
}
