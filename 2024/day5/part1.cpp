#include <fstream>
#include <string>
#include <iostream>
#include <vector>
#include <unordered_map>
#include <memory>
#include <sstream>
#include <unordered_set>
#include <queue>

int main(int argc, char** argv)
{
    std::ifstream f("input.txt");
    std::string line;
    std::queue<int> q;

    std::unordered_map<int, std::unordered_set<int>> map;
    int sum = 0;

    if (f.is_open())
    {
        while(std::getline(f, line))
        {
            if (line.find("|") != std::string::npos)
            {
                // std::cout << line << std::endl;

                // std::cout << line.substr(0,2) << ", " << line.substr(3,2) << std::endl;
                int before = std::stoi(line.substr(0,2));
                int after = std::stoi(line.substr(3,2));       

                // construct map and graph
                if (map.find(before) == map.end())
                {
                    map[after] = std::unordered_set<int>({before});
                }
                else
                {
                    map[after].emplace(before);
                }   
            }
            else
            {
                // std::cout << line << std::endl;
                std::stringstream ss(line);

                std::vector<int> pages;
                while (!ss.eof())
                {
                    int n;
                    ss >> n;
                    pages.push_back(n);
                    if (!ss.eof())
                    {
                        char c;
                        ss >> c;
                    }
                }

                bool correct_order = true;
                for (auto i = 1u; i < pages.size(); ++i)
                {
                    auto page = pages[i];
                    
                    for (auto j = 0u; j < i; ++j)
                    {
                        auto s = map[pages[j]];
                        if (s.find(page) != s.end())
                            correct_order = false;
                    }
                }

                if (correct_order)
                {
                    std::cout << "FOUND! " << line << std::endl;
                    std::cout << "mid = " << pages[pages.size() / 2] << std::endl;

                    sum = sum + pages[pages.size() / 2];
                    std::cout << sum << std::endl;
                }
            }
        }
    }

    for (auto& pair : map)
    {

        std::cout << "node " << pair.first << " => [";

        for (const auto& i : pair.second)
        {
            if (i)
                std::cout << i << ",";
        }

        std::cout << "]" << std::endl;
    }

    std::cout << sum << std::endl;

    f.close();
    return 0;
}