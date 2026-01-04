#include <fstream>
#include <iostream>
#include <limits>
#include <map>
#include <queue>
#include <set>
#include <sstream>
#include <vector>

using Point = std::pair<unsigned long, unsigned long>;

auto to_string(const Point& p) -> std::string
{
    return "(" + std::to_string(p.first) + "," + std::to_string(p.second) + ")";
};

void step_one_toward(Point& p1, Point& p2)
{
    if (p1.first == p2.first)
    {
        if (p1.second > p2.second)
            p1.second--;
        else
            p1.second++;
    }
    else // p1.second == p2.second
    {
        if (p1.first > p2.first)
            p1.first--;
        else
            p1.first++;
    }
}


bool is_area_valid(const std::set<Point>& s, const Point& p1, const Point& p2)
{
    auto start_x = std::min(p1.first, p2.first);
    auto start_y = std::min(p1.second, p2.second);

    auto end_x = std::max(p1.first, p2.first);
    auto end_y = std::max(p1.second, p2.second);

    for (auto x{start_x}; x <= end_x; ++x)
    {
        for (auto y{start_y}; y <= end_y; ++y)
        {
            if (s.find({x, y}) == s.end())
                return false;
        }
    }

    return true;
}

bool is_point_within(const Point& p, const std::set<Point>& s, unsigned long max_x, unsigned long max_y)
{
    auto i = 0ull;
    auto pp = p;
    while (true)
    {
        pp.first++;
        if (s.find(pp) != s.end())
            break;
        else if (pp.first >= max_x)
            return false;
    }

    pp = p;
    while (true)
    {
        pp.first--;
        if (s.find(pp) != s.end())
            break;
        else if (pp.first <= 0ul)
            return false;
    }

    pp = p;
    while (true)
    {
        pp.second++;
        if (s.find(pp) != s.end())
            break;
        else if (pp.second >= max_y)
            return false;
    }

    pp = p;
    while (true)
    {
        pp.second--;
        if (s.find(pp) != s.end())
            break;
        else if (pp.second <= 0ul)
            return false;
    }

    return true;
}

Point get_point_within(const Point& p, const std::set<Point>& s, unsigned long max_x, unsigned long max_y)
{
    Point pp{p.first + 1, p.second + 1}; // br
    if (is_point_within(pp, s, max_x, max_y)) return pp;
    pp.second -= 2; // tr
    if (is_point_within(pp, s, max_x, max_y)) return pp;
    pp.first -= 2; // tl
    if (is_point_within(pp, s, max_x, max_y)) return pp;
    pp.second += 2;
    if (is_point_within(pp, s, max_x, max_y)) return pp;

    return pp;
}

void populate_green(std::queue<Point>& points, std::set<Point>& s)
{
    while (!points.empty())
    {
        auto p = points.front();
        //std::cout << "Populating green at: " << to_string(p) << std::endl;
        //std::cout << "s.size(): " << s.size() << std::endl;
        //std::cout << "points.size(): " << points.size() << std::endl;

        if (s.find(p) != s.end())
        {}
        else
        {
            s.insert(p);
            Point pp{p.first + 1, p.second}; // right
            if (s.find(pp) == s.end()) points.push(pp);
            pp = Point{p.first - 1, p.second};
            if (s.find(pp) == s.end()) points.push(pp);
            pp = Point{p.first, p.second + 1};
            if (s.find(pp) == s.end()) points.push(pp);
            pp = Point{p.first, p.second - 1};
            if (s.find(pp) == s.end()) points.push(pp);
        }

        points.pop();
    }
}

//void populate_green(const Point& p, std::set<Point>& s)
//{
//    std::queue<Point> points;
//    points.push(p);
//    populate_green(points, s);
//}

int main (int argc, char** argv)
{
    std::ifstream file;

    if (argc > 1)
        file = std::ifstream{argv[1]};
    else
        file = std::ifstream{"input.txt"};

    std::string line;
    std::vector<Point> points;

    auto max_x = std::numeric_limits<unsigned long>::min();
    auto max_y = std::numeric_limits<unsigned long>::min();
    while (std::getline(file, line))
    {
        std::cout << line << std::endl;
        std::istringstream ss(line);
        unsigned long x, y;
        char c;
        ss >> x;
        ss >> c;
        ss >> y;
        points.emplace_back(x, y);

        max_x = (x > max_x) ? x : max_x;
        max_y = (y > max_y) ? y : max_y;
    }

    std::set<Point> green_and_red;

    for (auto itr{points.begin()}; itr != points.end(); ++itr)
    {
        Point next_p;
        if (itr == (points.end() - 1))
            next_p = *(points.begin());
        else
            next_p = *(itr + 1);

        auto p = *itr;

        green_and_red.insert(p);

        step_one_toward(p, next_p);

        while (p != next_p)
        {
            green_and_red.insert(p);

            step_one_toward(p, next_p);
        }
    }

    auto is_corner = [](const Point& p, const std::set<Point>& s) -> bool
    {
        std::vector<Point> v; // stores adjacent points of point p

        auto p_ = p;

        p_.first++;
        if (s.find(p_) != s.end())
            v.push_back(p_);
        p_.first--;
        p_.second++;
        if (s.find(p_) != s.end())
            v.push_back(p_);
        p_.second -= 2;
        if (s.find(p_) != s.end())
            v.push_back(p_);
        p_.second++;
        p_.first--;
        if (s.find(p_) != s.end())
            v.push_back(p_);

        // only two points will be added to vector
        if (v[0].first == v[1].first || v[0].second == v[1].second)
            return false;

        return true;
    };

    for (auto const& p : points)
    {
        if (is_corner(p, green_and_red))
        {
            // determine if it's a corner, if so
            // check each diagonal point if one of them is inside the encapsulated area
            // then populate 'X' from the point that's inside
            std::cout << "point " << to_string(p) << " is a corner!" << std::endl;

            auto p_within = get_point_within(p, green_and_red, max_x, max_y);
            // populate_green(p_within, green_and_red);
        }
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

            if (area >= max_area)
            {
                // check if area is valid
                if (is_area_valid(green_and_red, p1, p2))
                {
                    max_area = (area > max_area) ? area : max_area;
                }

            }
        }
    }

    std::cout << "Max area: " << max_area << std::endl;

    return 0;
}
