#ifndef CORE_LRUCACHE_H
#define CORE_LRUCACHE_H

#include <cstddef>
#include <list>
#include <stdexcept>
#include <type_traits>
#include <utility>
#include <unordered_map>

namespace core {

template<typename Key, typename Value>
class LRUCache {
public:
    using value_type = std::pair<const Key, Value>;

private:
    using list_t = std::list<value_type>;
    using map_t = std::unordered_map<Key, typename list_t::iterator>;

public:
    LRUCache(size_t cache_size) : cache_size_(cache_size) {
        if (cache_size == 0) {
            throw std::invalid_argument("cache size must be greater than zero");
        }
    }

    size_t Size() const { return map_.size(); }

    value_type* Find(const Key& k) {
        auto it = map_.find(k);
        if (it == map_.end()) {
            return nullptr;
        }
        MarkUsed(it->second);
        return &(*it->second);
    }

    const value_type* Find(const Key& k) const {
        auto it = map_.find(k);
        if (it == map_.end()) {
            return nullptr;
        }
        MarkUsed(it->second);
        return &(*it->second);
    }

    std::pair<value_type*, bool> Insert(const value_type& value) {
        auto it = map_.find(value.first);
        if (it != map_.end()) {
            return {&(*it->second), false};
        }

        it = InsertNew(value);
        return {&(*it->second), true};
    }

    std::pair<value_type*, bool> Insert(value_type&& value) {
        auto it = map_.find(value.first);
        if (it != map_.end()) {
            return {&(*it->second), false};
        }

        it = InsertNew(std::move(value));
        return {&(*it->second), true};
    }

    Value& operator[](const Key& k)
        requires std::is_default_constructible_v<Value>
    {
        auto it = map_.find(k);
        if (it == map_.end()) {
            it = InsertNew({k, Value{}});
            return it->second->second;
        } else {
            MarkUsed(it->second);
            return it->second->second;
        }
    }

    void Erase(const Key& k) {
        auto it = map_.find(k);
        if (it == map_.end()) {
            return;
        }
        recents_.erase(it->second);
        map_.erase(it);
    }

private:
    void MarkUsed(list_t::iterator it) const { recents_.splice(recents_.begin(), recents_, it); }

    map_t::iterator InsertNew(const value_type& value) {
        if (recents_.size() == cache_size_) {
            // Remove key for space
            PopForSpace();
        }

        recents_.push_front(value);
        auto listIt = recents_.begin();
        auto mapIt = map_.insert({listIt->first, listIt});
        return mapIt.first;
    }

    map_t::iterator InsertNew(value_type&& value) {
        if (recents_.size() == cache_size_) {
            // Remove key for space
            PopForSpace();
        }

        recents_.push_front(std::move(value));
        auto listIt = recents_.begin();
        auto mapIt = map_.insert({listIt->first, listIt});
        return mapIt.first;
    }

    void PopForSpace() {
        auto removeKey = std::move(recents_.back());
        map_.erase(removeKey.first);
        recents_.pop_back();
    }

    size_t cache_size_;
    mutable list_t recents_;
    map_t map_;
};

}  // namespace core

#endif
