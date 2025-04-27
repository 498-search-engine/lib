#ifndef LIB_QUEUE_H
#define LIB_QUEUE_H

#include <cstddef>
#include <utility>
#include "vector.h"

namespace core {

template<typename T>
struct Queue {
    //ring buffer queue
    public:
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
    T& front() {
        return arr[front];
    }
    T& back(){
        return arr[back];
    }
    bool empty() {
        return front == back;
    }
    size_t size(){
        return sz;
    }
    private:
    vector<T> arr;
    size_t front = 0, back = 0, sz = 0;
};

}  // namespace core

#endif
