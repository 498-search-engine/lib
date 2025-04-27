#ifndef LIB_PRIORITY_QUEUE_H
#define LIB_PRIORITY_QUEUE_H

#include <cstddef>
#include <utility>
#include "vector.h"
#include <initializer_list>

namespace core {

template<typename T, typename Container = vector<T>, typename COMP = std::less<T>>
struct PriorityQueue {
    //binary heap impl of priority queue
    public:
    PriorityQueue() = default;
    PriorityQueue(const std::initializer_list<T>& il){
        for (auto i : il){
            arr.push_back(i);
        }
        heapify();
    }
    void pop() {
        if (back == front) return; //need some sort of logging w this
        front += (front+1)%arr.size();
        --sz;
    }

    void push(const T& element) {
        auto new_b = (back +1) % arr.size();
        (not arr.size()) or (new_b == front) ? arr.push_back(element) : arr[new_b] = elem;
        back = (back +1) % arr.size();
        ++sz;
    }

    void push(T&& element) {
        auto new_b = (back +1) % arr.size();
        (not arr.size()) or (new_b == front) ? arr.push_back(std::move(elem)) : arr[new_b] = std::move(elem);
        back = (back +1) % arr.size();
        ++sz;
    }

    const T& top() {
        return arr[front];
    }
    
    bool empty() {
        return sz == 0;
    }
    size_t size(){
        return sz;
    }
    private:
    Container arr;
    COMP compare;
    uint32_t sz;

    uint32_t getIndex(const uint32 i) { return i-1; }

    void fixUp(uint32 idx) {
        while (idx > 0)
    }


    void heapify(){

    }
};

}  // namespace core

#endif
