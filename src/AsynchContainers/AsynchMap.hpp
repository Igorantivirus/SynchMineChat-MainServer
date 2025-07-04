#pragma once

#include <cstddef>
#include <map>
#include <mutex>

template <typename KeyT, typename ValueT>
class AsynchSafelyMap
{
public:
    AsynchSafelyMap() = default;

    bool empty() const
    {
        std::lock_guard<std::mutex> lg(mut_);
        return map_.empty();
    }

    std::size_t size() const
    {
        std::lock_guard<std::mutex> lg(mut_);
        return map_.size();
    }

    ValueT& operator[](const KeyT& key)
    {
        return map_[key];
    }
    const ValueT& safelyGet(const KeyT& key, const ValueT& elseValue) const
    {
        const auto found = map_.find(key);
        return found != map_.end() ? found->second : elseValue;
    }

    void remove(const KeyT& key)
    {
        map_.remove(key);
    }

    #pragma region begin end

    std::vector<ValueT>::const_iterator begin() const
    {
        std::lock_guard<std::mutex> lg(mut_);
        return map_.begin();
    }
    std::vector<ValueT>::const_iterator end() const
    {
        std::lock_guard<std::mutex> lg(mut_);
        return map_.end();
    }

    std::vector<ValueT>::iterator begin()
    {
        std::unique_lock<std::mutex> lg(mut_);
        return map_.begin();
    }
    std::vector<ValueT>::iterator end()
    {
        std::unique_lock<std::mutex> lg(mut_);
        return map_.end();
    }

    #pragma endregion

private:

    std::map<KeyT, ValueT> map_;

    mutable std::mutex mut_;
    std::condition_variable cv_;
};