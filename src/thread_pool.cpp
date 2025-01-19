#include "thread_pool.h"

ThreadPool::ThreadPool(size_t num_threads) : num_threads_(num_threads)
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

ThreadPool::~ThreadPool()
{
    {
        std::lock_guard<std::mutex> lock(mutex_);
        stop_ = true;
    }
    cv_.notify_all();
    for (auto& thread : threads_)
        thread.join();
}

ThreadPool THREAD_POOL(THREAD_NUM);