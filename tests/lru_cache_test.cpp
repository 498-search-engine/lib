#include <core/lru_cache.h>
#include <iostream>
//#include <gtest/gtest.h>
#include <cassert>

using namespace core;

int main() {
    LRUCache<int, int> my_cache(2);
    my_cache.Insert(1, 5);
    std::assert(my_cache.Size() == 1);
    my_cache.Insert(2, 4);
    std::assert(my_cache.Get(1).Value() == 5);
    my_cache.Insert(3, 2);
    std::assert(my_cache.Get(1).Value() == nullopt);
}