#include <stdio.h>

#include <algorithm>
#include <fstream>
#include <iostream>
#include <limits>
#include <sstream>
#include <string>
#include <vector>

#ifdef DEBUG
#define LOG(string, args...) printf(string, ##args)
#else
#define LOG(string, ...)
#endif

using ulint = unsigned long long int;
using ulintPair = std::pair<ulint, ulint>;

bool is_fresh(const std::vector<ulintPair>& v, ulint n)
{
    for (auto const& p : v)
    {
        if (n >= p.first && n <= p.second)
            return true;
    }

    return false;
}

void consolidate_ranges(std::vector<ulintPair>& v)
{
    //sort according to left
    auto sort_min_range_f = [](ulintPair& a, ulintPair& b) { return a.first < b.first; };
    std::sort(v.begin(), v.end(), sort_min_range_f);
}

ulint get_total_fresh(const std::vector<ulintPair>& v)
{
    ulint total_fresh{0u};

    for (auto i{0u}; i < v.size(); ++i)
    {
        if (i == v.size() - 1)
        {
            total_fresh += (v[i].second - v[i].first + 1ull);
            break;
        }

        if (v[i].second < v[i + 1].first)
        {
            total_fresh += (v[i].second - v[i].first + 1ull);

            LOG("Fresh total added from [%lld, %lld]\n", v[i].first, v[i].second);
        }
        else // v[i] either overlapp or contain the next coming ranges
        {
            auto left = v[i].first;
            auto right = v[i].second;

            for (auto j{i + 1}; j < v.size() + 1; ++j)
            {
                LOG("i, j = [%u, %u], left, right = [%lld, %lld]\n", i, j, left, right);

                // seeks until no overlap nor contain
                if (j == v.size())
                {
                    total_fresh += (right - left + 1);

                    LOG("Fresh total added from [%lld, %lld]\n", left, right);

                    i = j - 1;
                    break;
                }

                if (right >= v[j].second) {}
                else if (right >= v[j].first && right <= v[j].second) right = v[j].second;
                else
                {
                    // no overlapp nor contain
                    total_fresh += (right - left + 1);

                    LOG("Fresh total added from [%lld, %lld]\n", left, right);

                    i = j - 1;
                    break;
                }
            }
        }
    }

    return total_fresh;
}

int main (int argc, char** argv)
{
    std::fstream file;

    if (argc > 1)
        file = std::fstream(argv[1]);
    else
        file = std::fstream("input.txt");

    std::string line;

    std::vector<ulintPair> fresh_vector;
    ulint fresh_count{0u};

    while (std::getline(file, line))
    {
        if (auto index = line.find('-') != std::string::npos)
        {
            ulint left, right;
            char c;
            std::istringstream ss(line);

            ss >> left;
            ss >> c;
            ss >> right;

            fresh_vector.emplace_back(left, right);

            LOG("Fresh range: [%lld, %lld]\n", left, right);
        }
        else
        {
            if (!line.empty())
            {
                LOG("Check if fresh: %s\n", line.c_str());

                if (is_fresh(fresh_vector, std::stoull(line)))
                {
                    fresh_count++;

                    LOG(" is Fresh!\n");
                }
            }
        }
    }

    std::cout << "Fresh count = " << fresh_count << std::endl;

    consolidate_ranges(fresh_vector);

    auto print_pair_f = [](const ulintPair& p)
    {
        LOG("[%lld, %lld]\n", p.first, p.second);
    };

    std::for_each(fresh_vector.cbegin(), fresh_vector.cend(), print_pair_f);

    auto total_fresh = get_total_fresh(fresh_vector);

    std::cout << "Total fresh = " << total_fresh << std::endl;

    return 0;
}
