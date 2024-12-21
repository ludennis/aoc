#include <fstream>
#include <string>
#include <iostream>
#include <sstream>
#include <vector>
#include <memory>
#include <unordered_set>
#include <unordered_map>


template <typename T>
void print(std::vector<T>& v)
{
    for (auto &x : v)
    {
        std::cout << x << " ";
    }
    std::cout << std::endl;
}

template <typename A, typename B>
void print(const std::unordered_map<A, B>& m)
{
    for (auto& p : m)
    {
        std::cout << "m[" << p.first << "] = " << p.second << std::endl;
    }

    std::cout << std::endl;
}

template <typename T>
unsigned long blink(std::vector<T>& v, int n)
{
    std::unordered_map<unsigned long, unsigned long> m; // stores number and their counts

    for (auto& x : v)
    {
        if (m.find(x) == m.end())
            m[x] = 1;
        else
            m[x] += 1;
    }

 
    // print(m);

    std::vector<unsigned long> to_check;
    std::unordered_map<unsigned long, unsigned long> to_add;

    for (auto i = 0u; i < n; ++i)
    {
        to_add.clear();
        to_check.clear();
        for (const auto& p : m)
        {
            if (p.second > 0)
                to_check.push_back(p.first);
        }

        // print(m);
        
        // for (auto& ss : to_check)
        // {
        //     std::cout << ss << ",";
        // }
        // std::cout << std::endl;


        for (const auto& num : to_check)
        {
            auto count = m[num];
            // std::cout << "num = " << num << ", count = " << count << std::endl;

            std::string s = std::to_string(num);
            unsigned long digits = s.size();

            if (num == 0)
            {
                if (to_add.find(1) == to_add.end())
                    to_add[1] = m[num];
                else
                    to_add[1] += m[num];

                m[num] = 0;
            }
            else if (digits % 2 == 0)
            {
                // num -> left | right
                T left = stoul(s.substr(digits / 2));
                T right = stoul(s.substr(0, digits / 2));

                if (to_add.find(left) == to_add.end())
                    to_add[left] = m[num];
                else
                    to_add[left] += m[num];

                if (to_add.find(right) == to_add.end())
                    to_add[right] = m[num];
                else
                    to_add[right] += m[num];

                m[num] = 0;
            }
            else
            {
                if (to_add.find(num * 2024) == to_add.end())
                    to_add[num * 2024] = m[num];
                else
                    to_add[num * 2024] += m[num];

                m[num] = 0;
            }
        }

        for (const auto& p : to_add)
        {
            // std::cout << "to add num: " << p.first << ", count: " << p.second << std::endl;
            if (m.find(p.first) == m.end())
                m[p.first] = p.second;
            else
                m[p.first] += p.second;
        }

        // std::cout << std::endl << std::endl << std::endl;
    }

    unsigned long sum = 0;
    for (auto& p : m)
    {
        sum += p.second;
    }

    // print(m);

    return sum;
}

int main(int argc, char** argv)
{
    std::ifstream f(argv[1]);
    std::string line;

    std::vector<unsigned long int> v;

    if (f.is_open())
    {
        std::getline(f, line);

        std::stringstream ss(line);
        int i;
        while (!ss.eof())
        {
            ss >> i;
            v.push_back(i);
        }
    }


    int blink_time = std::stoi(std::string(argv[2]));
    print(v);

    auto count = blink(v, blink_time);

    std::cout << "stones count = " << count << std::endl;
    return 0;
}