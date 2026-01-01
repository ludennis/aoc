#include <thread>

template <typename T>
struct SafeQueue
{
    void push(const T&& t) { std::lock_guard<std::mutex> lk(m); q.push(std::move(t)); }
    void pop() { std::lock_guard<std::mutex> lk(m); q.pop(); }
    T& front() { std::lock_guard<std::mutex> lk(m); return q.front(); }
    bool empty() { std::lock_guard<std::mutex> lk(m); return q.empty(); }
    size_t size() { std::lock_guard<std::mutex> lk(m); return q.size(); }

    std::mutex m;
    std::queue<T> q;
};

struct AsyncThreadPool
{
    AsyncThreadPool(unsigned int s) : max_pool_size(s) { run(); }
    ~AsyncThreadPool() { this->stop(); }

    void stop() { is_stopped = true;}
    void run()
    {
        std::cout << "Starting thread pool with max pool size: " << max_pool_size << std::endl;

        t = std::thread([this]()
        {
            while (!this->is_stopped)
            {
                if (this->pool_size < this->max_pool_size && !(this->tasks.empty()))
                {
                    this->pool_size++;

                    auto task_t = std::thread([f = std::move(tasks.front()), this]()
                    {
                        f();
                        this->pool_size--;
                    });

                    task_t.detach();

                    tasks.pop();
                }
            }
        });

        t.detach();

        std::cout << "Thread pool running!" << std::endl;
    }

    void push_task(std::function<void()>&& f)
    {
        tasks.push(std::move(f));
    }

    size_t queue_size() { return tasks.size(); }
    bool overloaded() { return (tasks.size() >= max_pool_size); }

    std::atomic<unsigned int> max_pool_size{0u};
    std::atomic<unsigned int> pool_size{0u};
    std::atomic<bool> is_stopped{false};
    SafeQueue<std::function<void()>> tasks;
    std::thread t;
};

