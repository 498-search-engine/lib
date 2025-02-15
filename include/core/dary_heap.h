// d-ary heap
// authors: @mdvsh

#ifndef LIB_DARY_HEAP_H
#define LIB_DARY_HEAP_H

#include <vector>
#include <unordered_map>
#include <functional>
#include <stdexcept>
#include <utility>
#include <concepts>
#include <cassert>

namespace core {

// core type reqs
template <typename T>
concept HeapItem = std::movable<T> && std::equality_comparable<T> && requires(T a, T b) { std::swap(a, b); };
template <typename T, typename Priority, size_t D = 4, typename Compare = std::less<Priority>> requires HeapItem<T>

class dary_heap {
    static_assert(D >= 2, "D must be atleast 2");

    // packed struct for better cache efficiency - priority first to avoid padding
    struct alignas(16) HeapElement {
        Priority priority;
        T value;
        HeapElement(Priority p, T v) : priority(std::move(p)), value(std::move(v)) {}
        bool operator<(const HeapElement& other) const {
            return Compare()(priority, other.priority);
        }
    };

    std::vector<HeapElement> heap_;
    std::unordered_map<T, size_t> index_map_;
    Compare comp_;

    // helpers for parent/child idx calculations
    [[nodiscard]] constexpr size_t parent(size_t i) const noexcept { return (i - 1) / D; }
    [[nodiscard]] constexpr size_t firstChild(size_t i) const noexcept { return i * D + 1; }
    
    // Maintains heap property by floating elements up to their correct position
    // Updates index_map_ to track element positions
    void heapifyUp(size_t i) noexcept(std::is_nothrow_swappable_v<HeapElement>) {
        while (i > 0 && heap_[i] < heap_[parent(i)]) {
            std::swap(heap_[i], heap_[parent(i)]);
            index_map_[heap_[i].value] = i;
            i = parent(i);
        }
        index_map_[heap_[i].value] = i;
    }

    // Maintains heap property by sinking elements down to their correct position
    // Updates index_map_ for O(1) lookups later
    void heapifyDown(size_t i) noexcept(std::is_nothrow_swappable_v<HeapElement>) {
        while (true) {
            size_t min_index = i;
            const size_t first = firstChild(i);
            const size_t last = std::min(first + D, heap_.size());

            for (size_t j = first; j < last; ++j) {
                if (heap_[j] < heap_[min_index])
                    min_index = j;
            }

            if (min_index == i) break;

            std::swap(heap_[i], heap_[min_index]);
            index_map_[heap_[i].value] = i;
            i = min_index;
        }
        index_map_[heap_[i].value] = i;
    }

    // [debug] verify heap property, returns true valid heap
    [[nodiscard]] bool verifyHeap() const {
        for (size_t i = 1; i < heap_.size(); ++i) {
            if (heap_[i] < heap_[parent(i)]) return false;
        }
        return true;
    }

public:
    // create empty default heap - not thread safe, like most STL ctrs
    dary_heap() = default;
    
    // pre-allocate space for faster insertions - good for when one knows rough size
    explicit dary_heap(size_t reservedSize) {
        heap_.reserve(reservedSize);
        index_map_.reserve(reservedSize);
    }

    // O(1) helpers
    [[nodiscard]] bool contains(const T& value) const { return index_map_.contains(value); }
    [[nodiscard]] bool empty() const noexcept { return heap_.empty(); }
    [[nodiscard]] size_t size() const noexcept { return heap_.size(); }

    template<typename U = T, typename V = Priority>
    void push(U&& value, V&& priority) {
        if (index_map_.count(value))
            throw std::logic_error("duplicate element being pushed");
        
        heap_.emplace_back(std::forward<V>(priority), std::forward<U>(value));
        index_map_[heap_.back().value] = heap_.size() - 1;
        heapifyUp(heap_.size() - 1);
        assert(verifyHeap()); // can get away w/out it ?
    }

    void pop() {
        if (empty())
            throw std::out_of_range("heap is empty");
        
        index_map_.erase(heap_.front().value); // remove from the cache
        if (heap_.size() > 1) {
            heap_.front() = std::move(heap_.back()); // send to the back
            index_map_[heap_.front().value] = 0; // reset priority
        }
        // actually, pop
        heap_.pop_back();
        // propagate changes down
        if (!empty())
            heapifyDown(0);

        assert(verifyHeap()); // again, can get away w/out it ?
    }

    void updatePriority(const T& value, Priority newPriority) {
        auto it = index_map_.find(value);
        if (it == index_map_.end())
            throw std::invalid_argument("value to update priority of not found in heap");

        const size_t idx = it->second;
        const Priority oldPriority = heap_[idx].priority;
        heap_[idx].priority = newPriority;
        if (comp_(newPriority, oldPriority))
            heapifyUp(idx);
        else if (comp_(oldPriority, newPriority))
            heapifyDown(idx);
            
        assert(verifyHeap());
    }

    [[nodiscard]] const T& top() const {
        if (empty())
            throw std::out_of_range("heap is empty");
        return heap_.front().value;
    }

    [[nodiscard]] T& top() {
        if (empty())
            throw std::out_of_range("heap is empty");
        return heap_.front().value;
    }

    dary_heap<T, Priority>(const char* filename){
        
    }

    void serialize(const char* filename){

    }

};

} // namespace core

#endif 