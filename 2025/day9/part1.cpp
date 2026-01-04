#include <fstream>
#include <iostream>
#include <limits>
#include <sstream>
#include <vector>

using Point = std::pair<unsigned long, unsigned long>;

int main (int argc, char** argv)
{
    std::ifstream file;

    if (argc > 1)
        file = std::ifstream{argv[1]};
    else
        file = std::ifstream{"input.txt"};

    std::string line;
    std::vector<Point> points;

    while (std::getline(file, line))
    {
        std::cout << line << std::endl;
        std::istringstream ss(line);
        int x, y;
        char c;
        ss >> x;
        ss >> c;
        ss >> y;
        points.emplace_back(x, y);
    }

    auto max_area = std::numeric_limits<unsigned long>::min();
    for (auto const& p1 : points)
    {
        for (auto const& p2 : points)
        {
            if (p1 == p2)
                continue;

            auto x = (p1.first > p2.first) ? p1.first - p2.first : p2.first - p1.first;
            auto y = (p1.second > p2.second) ? p1.second - p2.second : p2.second - p1.second;
            auto area = (x + 1) * (y + 1);
            max_area = (area > max_area) ? area : max_area;
        }
    }

    std::cout << "Max area: " << max_area << std::endl;

    return 0;
}
