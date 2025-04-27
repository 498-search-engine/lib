#ifndef LIB_DEQUE_H
#define LIB_DEQUE_H

#include "vector.h"
#include <stdexcept>
#include <utility>
#include <cstddef>

namespace core {

template<typename T>
class Deque {
private:
    static constexpr size_t block_size = 64; // elements per block

    Vector<T*> blocks;
    size_t start_block = 0;
    size_t start_offset = 0;
    size_t end_block = 0;
    size_t end_offset = 0;
    size_t sz = 0;

    void allocate_block_front() {
        blocks.insert(blocks.begin(), new T[block_size]{});
        ++start_block;
        ++end_block;
    }

    void allocate_block_back() {
        blocks.push_back(new T[block_size]{});
    }

    void grow_if_needed_front() {
        if (start_offset == 0) {
            if (start_block == 0) allocate_block_front();
            --start_block;
            start_offset = block_size;
        }
    }

    void grow_if_needed_back() {
        if (end_block == blocks.size() || end_offset == block_size) {
            allocate_block_back();
        }
    }

public:
    Deque() {
        blocks.push_back(new T[block_size]{});
        start_block = end_block = 0;
        start_offset = end_offset = block_size / 2;
    }

    ~Deque() {
        for (auto block : blocks) delete[] block;
    }

    Deque(const Deque& other) = delete;
    Deque& operator=(const Deque& other) = delete;

    void push_front(const T& val) {
        grow_if_needed_front();
        --start_offset;
        blocks[start_block][start_offset] = val;
        ++sz;
    }

    void push_back(const T& val) {
        grow_if_needed_back();
        blocks[end_block][end_offset] = val;
        ++end_offset;
        if (end_offset == block_size) {
            ++end_block;
            end_offset = 0;
        }
        ++sz;
    }

    void pop_front() {
        if (empty()) throw std::out_of_range("pop_front from empty deque");
        ++start_offset;
        if (start_offset == block_size) {
            ++start_block;
            start_offset = 0;
        }
        --sz;
    }

    void pop_back() {
        if (empty()) throw std::out_of_range("pop_back from empty deque");
        if (end_offset == 0) {
            --end_block;
            end_offset = block_size;
        }
        --end_offset;
        --sz;
    }

    T& front() {
        if (empty()) throw std::out_of_range("front from empty deque");
        return blocks[start_block][start_offset];
    }

    const T& front() const {
        if (empty()) throw std::out_of_range("front from empty deque");
        return blocks[start_block][start_offset];
    }

    T& back() {
        if (empty()) throw std::out_of_range("back from empty deque");
        size_t eb = end_block;
        size_t eo = end_offset;
        if (eo == 0) {
            --eb;
            eo = block_size;
        }
        return blocks[eb][eo-1];
    }

    const T& back() const {
        if (empty()) throw std::out_of_range("back from empty deque");
        size_t eb = end_block;
        size_t eo = end_offset;
        if (eo == 0) {
            --eb;
            eo = block_size;
        }
        return blocks[eb][eo-1];
    }

    T& operator[](size_t idx) {
        if (idx >= sz) throw std::out_of_range("index out of bounds");
        size_t total_offset = start_offset + idx;
        size_t block = start_block + total_offset / block_size;
        size_t offset = total_offset % block_size;
        return blocks[block][offset];
    }

    const T& operator[](size_t idx) const {
        if (idx >= sz) throw std::out_of_range("index out of bounds");
        size_t total_offset = start_offset + idx;
        size_t block = start_block + total_offset / block_size;
        size_t offset = total_offset % block_size;
        return blocks[block][offset];
    }

    bool empty() const {
        return sz == 0;
    }

    size_t size() const {
        return sz;
    }

    void clear() {
        for (auto block : blocks) delete[] block;
        blocks.clear();
        blocks.push_back(new T[block_size]{});
        start_block = end_block = 0;
        start_offset = end_offset = block_size / 2;
        sz = 0;
    }

    void swap(Deque& other){
        std::swap(this->blocks, other.blocks);
        std::swap(this->start_block, other.start_block);
        std::swap(this->start_offset, other.start_offset);
        std::swap(this->end_block, other.end_block);
        std::swap(this->end_offset, other.end_offset);
        std::swap(this->sz, other.sz);
    }

    class iterator {
        public:
            using iterator_category = std::random_access_iterator_tag;
            using difference_type = std::ptrdiff_t;
        
            iterator() = default;
        
            iterator(Vector<T*>::iterator block_it, T* curr)
                : block_it(block_it), curr(curr) {}
        
            T& operator*() const {
                return *curr;
            }
        
            T* operator->() const {
                return curr;
            }
        
            iterator& operator++() {
                ++curr;
                if (curr == *block_it + block_size) {
                    ++block_it;
                    curr = *block_it;
                }
                return *this;
            }
        
            iterator operator++(int) {
                iterator tmp = *this;
                ++(*this);
                return tmp;
            }
        
            iterator& operator--() {
                if (curr == *block_it) {
                    --block_it;
                    curr = *block_it + block_size;
                }
                --curr;
                return *this;
            }
        
            iterator operator--(int) {
                iterator tmp = *this;
                --(*this);
                return tmp;
            }
        
            bool operator==(const iterator& other) const {
                return curr == other.curr;
            }
        
            bool operator!=(const iterator& other) const {
                return not (*this == other);
            }
        
        private:
            Vector<T*>::iterator block_it;
            T* curr;
    };

    iterator begin() {
        return iterator(blocks.begin() + static_cast<ptrdiff_t>(start_block), blocks[start_block] + start_offset);
    }
    
    iterator end() {
        return iterator(blocks.begin() + static_cast<ptrdiff_t>(end_block), blocks[end_block] + end_offset);
    }
};

} // namespace core

#endif // LIB_DEQUE_H
