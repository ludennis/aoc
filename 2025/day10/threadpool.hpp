#ifndef __THREADPOOL_HPP__
#define __THREADPOOL_HPP__

#include <functional>
#include <queue>
#include <thread>

#define LOG_E(string, args...) printf(string, ##args)

class ThreadPool
{
    using task_t = std::function<void()>;

    template <typename T>
    struct SafeStealableDeque
    {
        void push(T t)
        {
            std::lock_guard<std::mutex> lk{m};
            deq.push_back(std::make_unique<T>(std::move(t)));
        }

        std::unique_ptr<T> pop()
        {
            std::lock_guard<std::mutex> lk{m};
            if (deq.empty())
                return nullptr;
            else
            {
                std::unique_ptr<T> t = std::move(deq.front());
                deq.pop_front();

                return t;
            }
        }

        std::unique_ptr<T> steal()
        {
            std::lock_guard<std::mutex> lk{m};
            if (deq.empty())
                return nullptr;
            else
            {
                std::unique_ptr<T> t = std::move(deq.back());
                deq.pop_back();

                return t;
            }
        }

        void clear() { std::lock_guard<std::mutex> lk{m}; deq.clear(); }

        std::mutex m;
        std::deque<std::unique_ptr<T>> deq;
    };

  public:

    ThreadPool(unsigned int s) : pool_size(s)
    {
        for (auto i{0u}; i < pool_size; ++i)
            task_queues.emplace_back(std::make_unique<SafeStealableDeque<task_t>>());

        for (auto i{0u}; i < pool_size; ++i)
            workers.emplace_back(std::thread(&ThreadPool::worker_run, this, i));
    }

    template <typename F, typename ...Args>
    auto execute_task(F&& f, Args&& ...args) -> void
    {
        auto index = (n++ % pool_size);

        using ret_t = std::invoke_result_t<F, Args...>;

        if constexpr (std::is_void<ret_t>())
        {
            task_t task = std::bind(std::forward<F>(f), std::forward<Args>(args)...);
            task_queues[index]->push(task);
        }
    }

    ~ThreadPool()
    {
        is_stopped = true;

        for (auto& q : task_queues)
            q->clear();

        for (auto& t : workers)
            t.join();
    }

    static thread_local int thread_index;

  private:
    void worker_run(int i)
    {
        thread_index = i;

        while (!is_stopped)
        {
            std::unique_ptr<task_t> task = this->task_queues[thread_index]->pop();

            if (!task)
            {
                for (auto& q : task_queues)
                {
                    task = q->steal();

                    if (task)
                        break;
                }
            }

            if (task)
            {
                try
                {
                    (*task)();
                }
                catch (std::exception& e)
                {
                    std::rethrow_exception(std::current_exception());

                    LOG_E("%s: exception caught: %s\n", __FUNCTION__, e.what());
                }
            }
            else
            {
                std::this_thread::yield();
            }
        }
    }

    std::vector<std::unique_ptr<SafeStealableDeque<task_t>>> task_queues;
    std::vector<std::thread> workers;
    std::atomic<bool> is_stopped{false};
    unsigned int pool_size;
    unsigned int n{0u};
};

thread_local int ThreadPool::thread_index = -1;

#endif // __THREADPOOL_HPP__
