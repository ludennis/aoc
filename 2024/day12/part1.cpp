#include <fstream>
#include <string>
#include <iostream>
#include <vector>
#include <set>

template <typename T>
using Matrix = std::vector<std::vector<T>>;

using Pair = std::pair<int,int>;

template <typename T>
void print(const Matrix<T>& m)
{
    for (auto& v : m)
    {
        for (auto& x : v)
            std::cout << x;
        std::cout << std::endl;
    }
}

template <typename T>
T get(const Matrix<T>& m, const Pair& p)
{
    return m[p.first][p.second];
}

template <typename T>
bool is_in(const Matrix<T>& m, const Pair& p)
{
    return (p.first >= 0 && p.first < m.size() && p.second >= 0 && p.second < m[0].size());
}

template <typename T>
void gp (const Matrix<T>& m, Pair p, T& type, std::set<Pair>& visited, unsigned int& area, unsigned int& perimeter)
{

    if (!is_in(m, p) || visited.find(p) != visited.end())
        return;

    area += 1;
    perimeter += 4;
    visited.insert(p);

    #define CHECK_AND_GP \
            if (is_in(m, p) && get(m, p) == type) \
            { \
                perimeter -= 1; gp (m, p, type, visited, area, perimeter); \
            }

    // find nearby neighbors
    // up neighbor
    p.first -= 1;
    CHECK_AND_GP

    // left neighbor
    p.first += 1;
    p.second -= 1;
    CHECK_AND_GP

    // down neighbor
    p.second += 1;
    p.first += 1;
    CHECK_AND_GP

    // right neighbor
    p.first -= 1;
    p.second += 1;
    CHECK_AND_GP
}

template <typename T>
unsigned int get_price(const Matrix<T>& m, const Pair& p, std::set<Pair>& visited)
{
    if (visited.find(p) != visited.end())
        return 0;

    char type = get(m, p);

    unsigned int area = 0;
    unsigned int perimeter = 0;
    gp(m, p, type, visited, area, perimeter);

    std::cout << "p = (" << p.first << "," << p.second << ")" << std::endl;
    std::cout << "type = " << type << ", area = " << area << ", perimeter = " << perimeter << std::endl;

    return area * perimeter;
}

template <typename T>
int get_quote(const Matrix<T>& m)
{
    std::set<std::pair<int, int>> visited;

    int total_price = 0;

    for (auto i = 0u; i < m.size(); ++i)
    {
        for (auto j = 0u; j < m[0].size(); ++j)
        {
            auto p = Pair(i, j);
            if (visited.find(p) == visited.end())
            {
                // get garden price
                total_price += get_price(m, p, visited);
            }

        }
    }

    return total_price;
}

int main(int argc, char** argv)
{
    std::ifstream f(argv[1]);
    std::string line;


    Matrix<char> m;

    if (f.is_open())
    {
        while (std::getline(f, line))
        {
            m.emplace_back(std::vector<char>());
            for (auto& c : line)
                m.back().emplace_back(c);
        }
    }

    auto total_price = get_quote(m);

    print(m);
    std::cout << std::endl;

    std::cout << "total price = " << total_price << std::endl;

    return 0;
}