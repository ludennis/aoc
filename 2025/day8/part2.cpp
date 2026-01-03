#include <array>
#include <cmath>
#include <fstream>
#include <functional>
#include <iostream>
#include <map>
#include <optional>
#include <queue>
#include <set>
#include <string>
#include <sstream>
#include <stack>
#include <vector>

#ifdef DEBUG
#define LOG(string, args...) printf(string, ##args)
#else
#define LOG(string, args...)
#endif

struct Point
{
    Point(int x, int y, int z) : p{x, y, z} {}
    int x() const { return std::get<0>(p); }
    int y() const { return std::get<1>(p); }
    int z() const { return std::get<2>(p); }

    bool operator==(const Point& other) const
    {
        return (this->x() == other.x() && this->y() == other.y() && this->z() == other.z()) ? true : false;
    }

    auto operator<=>(const Point&) const = default;

    std::string to_string() const
    {
        std::stringstream ss;
        ss << "(" << this->x() << "," << this->y() << "," << this->z() << ")";

        return ss.str();
    }

    std::tuple<int, int, int> p;
};

auto exist_in_circuits(std::vector<std::set<Point>>& circuits, const Point& p)
        -> std::optional<decltype(circuits.begin())>
{
    for (auto c_itr{circuits.begin()}; c_itr != circuits.end(); ++c_itr)
        if (c_itr->find(p) != c_itr->end())
            return {c_itr};
    return {};
}

void add_to_circuits(std::vector<std::set<Point>>& circuits, const Point& p1, const Point& p2)
{
    if (!circuits.empty())
    {
        auto c1 = exist_in_circuits(circuits, p1);
        auto c2 = exist_in_circuits(circuits, p2);
        if (c1 && c2 && c1.value() != c2.value())
        {
            // combine two circuits
            (c1.value())->merge(*(c2.value()));
            circuits.erase(c2.value());
            return;
        }
        for (auto& c : circuits)
        {
            if (c.find(p1) != c.end() || c.find(p2) != c.end())
            {
                c.insert(p1);
                c.insert(p2);
                return;
            }
        }

        circuits.emplace_back(std::set<Point>{p1, p2});
    }
    else
        circuits.emplace_back(std::set<Point>{p1, p2});
}

unsigned long long add_until_one_big_circuit(std::vector<std::set<Point>>& circuits,
        const std::vector<Point>& points)
{
    for (auto const& p1 : points)
    {
        for (auto const& p2 : points)
        {
            if (p1 == p2)
                continue;

            add_to_circuits(circuits, p1, p2);

            if (circuits.size() == 1)
            {
                LOG("Junction boxes forming 1 circuit: %s, %s\n",
                        p1.to_string().c_str(), p2.to_string().c_str());
                return p1.x() * p2.x();
            }
        }
    }
    return 0ull;
}


int main(int argc, char** argv)
{
    std::ifstream file;

    auto kMaxPriorityQueueSize = 10;
    if (argc > 1)
    {
        file = std::ifstream(argv[1]);
    }
    else
    {
        file = std::ifstream("input.txt");
        kMaxPriorityQueueSize = 1000;
    }

    std::vector<Point> points;

    std::string line;
    while (std::getline(file, line))
    {
        std::istringstream ss(line);

        char c;
        int x, y, z;

        ss >> x;
        ss >> c;
        ss >> y;
        ss >> c;
        ss >> z;

        points.emplace_back(Point{x, y, z});
    }


    using t_type = std::tuple<double, Point, Point>;
    auto cmp = [](t_type& dp1, t_type& dp2)
    {
        return std::get<double>(dp1) < std::get<double>(dp2);
    };
    using priority_queue_type =
            std::priority_queue<t_type, std::vector<t_type>, decltype(cmp)>;
    priority_queue_type pq{cmp};


    std::unordered_map<std::string, double> edges;

    for (auto const& p : points)
    {
        for (auto const& other : points)
        {
            if (p == other)
                continue;

            auto distance = std::sqrt(std::pow(std::abs(p.x() - other.x()), 2) +
                    std::pow(std::abs(p.y() - other.y()), 2) +
                    std::pow(std::abs(p.z() - other.z()), 2));

            auto p_str = p.to_string();
            auto other_str = other.to_string();

            if (pq.empty())
            {
                pq.push(std::tuple{distance, p, other});
                edges[p_str + "," + other_str] = distance;
                edges[other_str + "," + p_str] = distance;
            }
            else if (distance < std::get<double>(pq.top()))
            {
                if (edges.find(p_str + "," + other_str) != edges.end())
                    continue;

                pq.push(std::tuple{distance, p, other});
                edges[p_str + "," + other_str] = distance;
                edges[other_str + "," + p_str] = distance;

//                if (pq.size() > kMaxPriorityQueueSize)
//                {
//                    auto [d, p1, p2] = pq.top();
//                    auto p1_str = p1.to_string();
//                    auto p2_str = p2.to_string();
//                    edges.erase(edges.find(p1_str + "," + p2_str));
//                    edges.erase(edges.find(p2_str + "," + p1_str));
//                    pq.pop();
//                }
            }
        }
    }

    std::stack<std::array<Point, 2>> shortest_distance_points;
    while (!pq.empty())
    {
        shortest_distance_points.push({std::get<1>(pq.top()), std::get<2>(pq.top())});
        pq.pop();
    }

    std::vector<std::set<Point>> circuits;
    for (auto const& p : points)
        circuits.push_back(std::set<Point>{p});

    auto sum{1ull};
    bool stopped{false};

    while (!shortest_distance_points.empty() || stopped)
    {
        auto p1 = shortest_distance_points.top()[0];
        auto p2 = shortest_distance_points.top()[1];

        add_to_circuits(circuits, p1, p2);


        LOG("p1 %s, p2 %s\n", p1.to_string().c_str(), p2.to_string().c_str());
        for (auto const& c : circuits)
        {
            LOG("Circuit:\n");

            for (auto const& p : c)
            {
                LOG("    -> %s\n", p.to_string().c_str());
            }
        }

        if (circuits.size() == 1)
        {
            sum = p1.x() * p2.x();
            stopped = true;
            break;
        }
        shortest_distance_points.pop();
    }

    LOG("X multiples when connected as 1 large circuit: %lld\n", sum);

    return 0;
}
