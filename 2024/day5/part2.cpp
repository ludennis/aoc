#include <fstream>
#include <string>
#include <iostream>
#include <vector>
#include <unordered_map>
#include <memory>
#include <sstream>
#include <unordered_set>
#include <queue>

void fix_order(std::vector<int>& pages,
    const std::unordered_map<int, std::unordered_set<int>>& map)
{
    for (auto i = 1u; i < pages.size(); ++i)
    {
        auto page = pages[i];

        for (auto j = 0u; j < i; ++j)
        {
            auto& s = map.at(pages[j]);
            if (s.find(page) != s.end())
            {
                auto temp = std::move(pages[i]);
                pages[i] = std::move(pages[j]);
                pages[j] = std::move(temp);

                return;
            }
        }
    }
}

bool is_correct_order(const std::vector<int>& pages,
        const std::unordered_map<int, std::unordered_set<int>>& map)
{
    for (auto i = 1u; i < pages.size(); ++i)
    {
        auto page = pages[i];
        
        for (auto j = 0u; j < i; ++j)
        {
            auto& s = map.at(pages[j]);
            if (s.find(page) != s.end())
                return false;
        }
    }

    return true;
}

int main(int argc, char** argv)
{
    std::ifstream f("input.txt");
    std::string line;
    std::queue<int> q;

    std::unordered_map<int, std::unordered_set<int>> map;

    {
        std::ifstream f("input.txt");
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
                    if (map.find(after) == map.end())
                    {
                        map[after] = std::unordered_set<int>({before});
                    }
                    else
                    {
                        map[after].emplace(before);
                    }   
                }
            }
        }
    }

    int sum = 0;

    {
        std::ifstream f("input.txt");
        if (f.is_open())
        {
            while(std::getline(f, line))
            {
                if (line.find(",") != std::string::npos)
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

                    if (!is_correct_order(pages, map))
                    {
                        std::cout << "BAD! " << line << std::endl;

                        // do a sort
                        while (!is_correct_order(pages, map))
                        {
                            fix_order(pages, map);
                        }

                        std::cout << "GOOD! ";
                        for (auto& p : pages)
                            std::cout << p << ",";
                        std::cout << std::endl;

                        sum += pages[pages.size() / 2];
                    }
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