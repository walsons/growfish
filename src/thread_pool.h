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
    auto submit(Func&& func, Args&&... args) -> std::future<std::invoke_result_t<Func, Args...>>;

    ~ThreadPool();

private:
    size_t num_threads_;
    std::vector<std::thread> threads_;
    std::condition_variable cv_;
    std::mutex mutex_;
    std::atomic<bool> stop_;
    std::queue<std::function<void()>> tasks_;
};

template <typename Func, typename... Args>
auto ThreadPool::submit(Func&& func, Args&&... args) -> std::future<std::invoke_result_t<Func, Args...>>
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

#endif