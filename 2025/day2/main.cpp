#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <utility>
#include <vector>

using ul = unsigned long long int;
using Pair = std::pair<ul, ul>;

bool is_typo(ul num)
{
    auto str = std::to_string(num);

    bool found{true};

    for (int i{2}; i <= str.size(); ++i)
    {
        if (str.size() % i == 0)
        {
            int num_of_substr = i;
            int substr_length = str.size() / num_of_substr;

            std::string cmp = str.substr(0, substr_length);

            for (int j{1}; j < num_of_substr; ++j)
            {

#ifdef DEBUG
                std::cout << "cmp = " << cmp << ", substr = " <<
                        str.substr(j * substr_length, substr_length) <<
                        ", num of substr = " << num_of_substr <<
                        ", substr_length = " << substr_length <<
                        ", j = " << j << std::endl;
#endif

                bool substr_equal = (str.substr(j * substr_length, substr_length) == cmp);
                found = found && (str.substr(j * substr_length, substr_length) == cmp);
                if (!found) { break; }
            }

            if (found)
                return true;

            found = true;
        }
    }

    return false;
}

int main (int argc, char** argv)
{
    std::ifstream file("input.txt");
    std::string line;

    std::vector<Pair> numbers;

    if (std::getline(file, line))
    {
        std::string token;
        std::stringstream ss(line);
        while (std::getline(ss, token, ','))
        {
            std::stringstream ss_(token);

            ul a, b;
            char c;
            ss_ >> a;
            ss_ >> c;
            ss_ >> b;

            numbers.emplace_back(a, b);
        }
    }

    ul sum{0};

    for (auto const& num : numbers)
    {
        std::cout << num.first << ", " << num.second << std::endl;

        for (auto n{num.first}; n <= num.second; ++n)
        {
            if (is_typo(n))
            {
                // std::cout << "Found typo: " << n << std::endl;
                sum += n;
            }
        }
    }

    std::cout << std::endl << "Sum of typo is: " << sum << std::endl;

    return 0;
}
