#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <iostream>

namespace {

bool greater(const int a, const int b)
{
    return a >= b;
}

} // namespace

int main(int argc, char** argv)
{
    std::ifstream in_file;
    std::string line;
    std::string number;
    
    std::vector<int> numbers;
    std::unordered_map<int, int> map;

    in_file.open("input.txt");

    if (in_file.is_open())
    {
        while (getline(in_file, line) && !in_file.eof())
            // std::cout << line << std::endl;
            // std::cout << line.substr(0, 5) << ", " << line.substr(8, 5) << std::endl;
            if (line != "")
            {
                int n1 = std::stoi(line.substr(0, 5));
                int n2 = std::stoi(line.substr(8, 5));

                numbers.emplace_back(n1);

                if (map.find(n2) == map.end())
                    map[n2] = 1;
                else
                    map[n2]++;
            }
    }
    in_file.close();

    std::cout << numbers.size() << ", " << map.size() << std::endl;

    int sum = 0;
    for (auto itr = numbers.begin(); itr != numbers.end(); ++itr)
    {
        if (map.find(*itr) != map.end())
            sum += *itr * map[*itr];
    }

    std::cout << sum << std::endl;

    return 0;
}