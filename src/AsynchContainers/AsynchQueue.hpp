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

    void push(const T &value)
    {
        std::unique_lock<std::mutex> lg(mut_);
        q.push_back(value);
        lg.unlock();
        cv.notify_one();
    }

    T pop()
    {
        std::unique_lock<std::mutex> lg(mut_);

        cv_.wait(lg, [&]
                 { return !q.empty(); });

        T result = q.front();
        q.pop_front();

        return result;
    }

private:
    std::queue<Type> queue_;

    mutable std::mutex mut_;
    std::condition_variable cv_;
};