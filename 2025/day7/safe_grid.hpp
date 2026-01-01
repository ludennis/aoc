#include <atomic>
#include <vector>
#include <mutex>

using pair_type = std::pair<int, int>;

struct AtomicGrid
{
    using atomic_char_v_t = std::vector<std::atomic<char>>;
    using grid_type = std::vector<atomic_char_v_t>;

    AtomicGrid() = delete;
    AtomicGrid(int x_size, int y_size)
    {
        grid = std::make_unique<grid_type>(y_size);
        for (auto i = 0u; i < grid->size(); ++i)
            (*grid)[i] = atomic_char_v_t(x_size);
    }

    std::atomic<char>& get(pair_type& p) { return (*grid)[p.second][p.first]; }
    std::atomic<char>& get(int x, int y) { return (*grid)[y][x]; }
    void set(int x, int y, char c) { (*grid)[y][x] = c; }
    void set(pair_type& p, char c) { (*grid)[p.second][p.first] = c; }
    std::vector<std::atomic<char>>& operator[](size_t y) { return (*grid)[y]; }
    std::vector<std::atomic<char>>& back() { return grid->back(); }

    void print()
    {
        for (auto const& v : *grid)
        {
            for (auto const& c : v)
            {
                std::cout << c.load();
            }
            std::cout << std::endl;
        }
    }

    unsigned int update_splits() { splits++; return splits; }
    size_t get_y_size() { return grid->size(); }
    size_t get_x_size() { return (grid->size() > 0) ? (*grid)[0].size() : 0; }

    std::atomic<unsigned int> splits{0u};
    std::atomic<unsigned int> num_timelines{0u};
    std::unique_ptr<grid_type> grid{nullptr};
};

struct SafeGrid
{
    using grid_type = std::vector<std::string>;

    SafeGrid() = default;

    char& get(int x, int y) { std::lock_guard<std::mutex> lck(m); return grid[y][x]; }
    char& get(pair_type& p) { std::lock_guard<std::mutex> lck(m); return grid[p.second][p.first]; }
    std::string& get_row(int y) { std::lock_guard<std::mutex> lck(m); return grid[y]; }
    void set(int x, int y, char c) { std::lock_guard<std::mutex> lck(m); grid[y][x] = c; }
    void set(pair_type& p, char c) { std::lock_guard<std::mutex> lck(m); grid[p.second][p.first] = c; }
    void push_row(const std::string& line)
    {
           std::lock_guard<std::mutex> lck(m); grid.emplace_back(line);
    }

    void print()
    {
        std::lock_guard<std::mutex> lck(m);
        for (auto const s : grid)
            std::cout << s << std::endl;
    }

    unsigned int update_splits() { splits++; return splits; }

    size_t get_y_size() { std::lock_guard<std::mutex> lck(m); return grid.size(); }

    size_t get_x_size()
    {
        std::lock_guard<std::mutex> lck(m); return (grid.size() > 0) ? grid[0].size() : 0;
    }

    std::atomic<unsigned int> splits{0u};
    std::atomic<unsigned int> num_timelines{0u};
    grid_type grid;
    std::mutex m;
};

