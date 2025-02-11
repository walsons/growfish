#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <queue>
#include <functional>
#include <thread>
#include <future>
#include <condition_variable>
#include <atomic>

// currently only works well on amd CPU, while on intel CPU, multiple thread perform worse than single thread
constexpr size_t THREAD_NUM = 1;

class ThreadPool {
public:
    ThreadPool(size_t num_threads);
    
    template <typename Func, typename... Args>
    auto Submit(Func&& func, Args&&... args) -> std::future<std::invoke_result_t<Func, Args...>>;

    ~ThreadPool();

private:
    size_t num_threads_;
    std::vector<std::thread> threads_;
    std::condition_variable cv_;
    std::mutex mutex_;
    std::atomic<bool> stop_;
    std::queue<std::function<void()>> tasks_;
};

inline ThreadPool::ThreadPool(size_t num_threads) : num_threads_(num_threads)
{
    for (size_t i = 0; i < num_threads_; ++i)
    {
        threads_.emplace_back([this] {
            while (true)
            {
                std::function<void()> task;
                {
                    std::unique_lock<std::mutex> lock(this->mutex_);
                    this->cv_.wait(lock, [this] {
                        return this->stop_ || !this->tasks_.empty();
                    });
                    if (this->stop_ && this->tasks_.empty())
                        return;
                    task = tasks_.front();
                    tasks_.pop();
                }
                task();
            }
        });
    }
}

template <typename Func, typename... Args>
auto ThreadPool::Submit(Func&& func, Args&&... args) -> std::future<std::invoke_result_t<Func, Args...>>
{
    auto task = std::bind(std::forward<Func>(func), std::forward<Args>(args)...);
    auto task_ptr = std::make_shared<std::packaged_task<std::invoke_result_t<Func, Args...>()>>(task);
    auto future = task_ptr->get_future();
    {
        std::lock_guard<std::mutex> lock(mutex_);
        tasks_.emplace([task_ptr]() {
            (*task_ptr)();
        });
    }
    cv_.notify_one();
    return future;
}

inline ThreadPool::~ThreadPool()
{
    {
        std::lock_guard<std::mutex> lock(mutex_);
        stop_ = true;
    }
    cv_.notify_all();
    for (auto& thread : threads_)
        thread.join();
}

#endif