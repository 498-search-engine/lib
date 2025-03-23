#include "../include/core/unordered_map.h"
#include <cassert>
#include <iostream>
using namespace core;
int main(){
    UnorderedMap<int, int> my_map;
    std::cout << "1\n";
    my_map[5] = 20;
    assert(my_map[5] == 20);
    std::cout << "2\n";
    my_map[5] = 5;
    assert(my_map[5] == 5);
    std::cout << "3\n";
    my_map.erase(5);
    assert(my_map.find(5) == my_map.end());\
    std::cout << "4\n";
    return 0;
}