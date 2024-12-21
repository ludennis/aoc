#include <algorithm>
#include <fstream>
#include <string>
#include <iostream>
#include <vector>
#include <sstream>

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
    
    std::vector<int> v1;
    std::vector<int> v2;

    in_file.open("input.txt");

    if (in_file.is_open())
    {
        while (getline(in_file, line) && !in_file.eof())
            // std::cout << line << std::endl;
            // std::cout << line.substr(0, 5) << ", " << line.substr(8, 5) << std::endl;
            if (line != "")
            {
                v1.emplace_back(std::stoi(line.substr(0, 5)));
                v2.emplace_back(std::stoi(line.substr(8, 5)));
            }
    }
    in_file.close();

    std::cout << v1.size() << ", " << v2.size() << std::endl;

    int d_sum = 0;

    std::make_heap(v1.begin(), v1.end(), greater);
    std::make_heap(v2.begin(), v2.end(), greater);
    while (v1.size() > 0 && v2.size() > 0)
    {
        d_sum += std::abs(v1.front() - v2.front());
        std::pop_heap(v1.begin(), v1.end(), greater);
        v1.pop_back();

        std::pop_heap(v2.begin(), v2.end(), greater);
        v2.pop_back();
    }

    std::cout << d_sum << std::endl;

    return 0;
}