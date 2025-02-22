#include <list>
#include <unordered_map>
#include "optional.h"

namespace core {
template<typename Key, typename Value>
class LRUCache {
    public:
    LRUCache(uint32_t sz) : max_sz(sz) {}

    uint32_t Size() const {
        return map.size();
    }

    bool Insert(const Key& key, const Value& value) {
        if (map.contains(key)) return true;

        if (Size() == max_sz) {
            //remove the oldest element
            auto first = recents.front();
            recents.pop_front();
            map.erase(first);
        }

        //insert new element
        recents.push_back(key);
        map[key] = value;
        return true;

        //possible error checking??
    }

    const optional<Value> Get(const Key& key) const {
        if (not map.contains(key)) return nullopt;
        return map[key];
    }
    
    private:
    std::list<Key> recents;
    std::unordered_map<Key> map;
    uint32_t max_sz = 0;
}
}