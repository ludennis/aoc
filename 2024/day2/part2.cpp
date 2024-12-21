#include <fstream>
#include <string>
#include <iostream>
#include <sstream>
#include <vector>

template <typename T>
bool IsSafeReport(std::vector<T> v_)
{
    auto v = std::move(v_);
    bool decreasing = (v[1] - v[0]) > 0; 

    for (auto i = 0u; i < v.size() - 1; ++i)
    {
        int diff = v[i + 1] - v[i];
        if (diff == 0 || (decreasing && diff < 0) ||
            (!decreasing && diff > 0) || abs(diff) < 1 || abs(diff) > 3)
            return false;
    }

    return true;
}

template <typename T>
bool IsDampenerSafe(std::vector<T> v_)
{
    auto v = std::move(v_);

    for (auto i = 0u; i < v.size(); ++i)
    {
        auto vv = v;
        vv.erase(vv.begin() + i);
        if (IsSafeReport(std::move(vv)))
            return true;
    }

    return false;
}


int main(int argc, char** argv)
{
    std::ifstream file("input.txt");
    std::string line;
    int sum_of_safe_report = 0;

    if (file.is_open())
    {
        while (getline(file, line) && !file.eof())
        {          
            std::stringstream ss(line);

            std::vector<int> v;
            while (!ss.eof())
            {
                int i;
                ss >> i;
                v.emplace_back(i);
            }

            if (IsSafeReport(v))
                sum_of_safe_report++;
            else
                if (IsDampenerSafe(std::move(v)))
                    sum_of_safe_report++;
        }
    }

    std::cout << sum_of_safe_report << std::endl;
}