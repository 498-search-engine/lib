#ifndef LIB_PRIORITY_QUEUE_H
#define LIB_PRIORITY_QUEUE_H

#include <cstddef>
#include <utility>
#include "vector.h"
#include <initializer_list>

namespace core {

template<typename T, typename Container = Vector<T>, typename COMP = std::less<T>>
struct PriorityQueue {
    //binary heap impl of priority queue
    public:
    PriorityQueue() = default;
    PriorityQueue(const std::initializer_list<T>& il){
        for (auto i : il){
            arr.pushBack(i);
        }
        sz = arr.size();
        heapify();
    }

    void printPQ(){
        for (auto i : arr){
            std::cout << i << " ";
        }
        std::cout << std::endl;
    }

    void pop() {
        if (empty()) return;
        std::swap(arr[0], arr[sz-1]);
        --sz;
        fixDown(0);
    }

    void push(const T& element) {
        if (sz == arr.size()) {
            arr.pushBack(element);
        } else {
            arr[sz] = element;
        }
        fixUp(sz);
        ++sz;
    }

    void push(T&& element) {
        if (sz == arr.size()) {
            arr.pushBack(std::move(element));
        } else {
            arr[sz] = std::move(element);
        }
        fixUp(sz);
        ++sz;
    }

    const T& top() {
        return arr[0];
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
    uint32_t sz = 0;

    uint32_t getIndex(const uint32_t i) { return i-1; }

    void fixUp(uint32_t idx) {
        while (idx > 0) {
            uint32_t parent = getIndex(idx) / 2;
            if (!compare(arr[parent], arr[idx])) {
                break;
            }
            std::swap(arr[parent], arr[idx]);
            idx = parent;
        }
    }

    void fixDown(uint32_t idx){
        while (true) {
            uint32_t left = 2 * idx + 1;
            uint32_t right = 2 * idx + 2;
            uint32_t largest = idx;
    
            if (left < sz && compare(arr[largest], arr[left])) {
                largest = left;
            }
            if (right < sz && compare(arr[largest], arr[right])) {
                largest = right;
            }
    
            if (largest == idx) {
                break;
            }
            
            std::swap(arr[idx], arr[largest]);
            idx = largest;
        }
    }


    void heapify(){
        for (int i = sz-1; i >= 0; --i) {
            fixDown(i);
        }
    }
};

}  // namespace core

#endif
