#pragma once

#include <cstddef>
#include <queue>
#include <mutex>

template <typename Type>
class AsynchSafelyQueue
{
public:
    AsynchSafelyQueue() = default;

    bool empty() const
    {
        std::lock_guard<std::mutex> lg(mut_);
        return queue_.empty();
    }

    std::size_t size() const
    {
        std::lock_guard<std::mutex> lg(mut_);
        return queue_.size();
    }

    void push(const Type &value)
    {
        std::unique_lock<std::mutex> lg(mut_);
        queue_.push(value);
        lg.unlock();
        cv_.notify_one();
    }

    Type pop()
    {
        std::unique_lock<std::mutex> lg(mut_);

        cv_.wait(lg, [&]
                 { return !queue_.empty(); });

        Type result = queue_.front();
        queue_.pop();

        return result;
    }

    std::queue<Type>& getStdQueue()
    {
        return queue_;
    }
    const std::queue<Type>& getStdQueue() const
    {
        return queue_;
    }

private:
    std::queue<Type> queue_;

    mutable std::mutex mut_;
    std::condition_variable cv_;
};