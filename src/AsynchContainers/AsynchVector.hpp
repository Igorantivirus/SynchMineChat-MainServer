#pragma once

#include <cstddef>
#include <vector>
#include <mutex>

template <typename Type>
class AsynchSafelyVector
{
public:
    AsynchSafelyVector() = default;

    std::vector<Type>& getStdVector()
    {
        return vector_;
    }
    const std::vector<Type>& getStdVector() const
    {
        return vector_;
    }

    #pragma region memory

    void resize(const size_t newSize, const Type& newValue = Type{})
    {
        std::unique_lock<std::mutex> lg(mut_);
        vector_.resize(newSize, newValue);
    }
    void reserve(const size_t newCapasity)
    {
        std::unique_lock<std::mutex> lg(mut_);
        vector_.reserve(newCapasity);
    }

    std::size_t size() const
    {
        std::lock_guard<std::mutex> lg(mut_);
        return vector_.size();
    }
    std::size_t capacity() const
    {
        std::lock_guard<std::mutex> lg(mut_);
        return vector_.capacity();
    }

    bool empty() const
    {
        std::lock_guard lg(mut_);
        return vector_.empty();
    }

    #pragma endregion

    #pragma region push pop

    void push_back(const Type &value)
    {
        std::unique_lock<std::mutex> lg(mut_);
        vector_.push_back(value);
    }
    void push_front(const Type &value)
    {
        std::unique_lock<std::mutex> lg(mut_);
        vector_.insert(vector_.begin(), value);
    }
    void insert(const size_t ind, const Type &value)
    {
        std::unique_lock<std::mutex> lg(mut_);
        vector_.insert(vector_.begin() + ind, value);
    }

    void pop_back()
    {
        std::unique_lock<std::mutex> lg(mut_);
        vector_.pop_back();
    }
    void pop_front()
    {
        std::unique_lock<std::mutex> lg(mut_);
        vector_.erase(vector_.begin());
    }
    void erase(const size_t ind, const size_t count)
    {
        std::unique_lock<std::mutex> lg(mut_);
        vector_.erase(vector_.begin() + ind, vector_.begin() + ind + count);
    }

    #pragma endregion

    #pragma region get operator

    const Type& operator[](const size_t ind) const
    {
        std::lock_guard<std::mutex> lg(mut_);
        return vector_[ind];
    }
    Type& operator[](const size_t ind)
    {
        std::unique_lock<std::mutex> lg(mut_);
        return vector_[ind];
    }

    #pragma endregion

    #pragma region begin end

    std::vector<Type>::const_iterator begin() const
    {
        std::lock_guard<std::mutex> lg(mut_);
        return vector_.begin();
    }
    std::vector<Type>::const_iterator end() const
    {
        std::lock_guard<std::mutex> lg(mut_);
        return vector_.end();
    }

    std::vector<Type>::iterator begin()
    {
        std::unique_lock<std::mutex> lg(mut_);
        return vector_.begin();
    }
    std::vector<Type>::iterator end()
    {
        std::unique_lock<std::mutex> lg(mut_);
        return vector_.end();
    }

    #pragma endregion

private:

    std::vector<Type> vector_;
    mutable std::mutex mut_;

};