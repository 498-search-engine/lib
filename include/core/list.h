#ifndef LIB_LIST_H
#define LIB_LIST_H

#include <stdio.h>
#include <utility>
#include <functional>

namespace core {
    template<typename T>
    class List {
        private:

        struct Node {
            T data;
            Node* next;
            Node* prev;

            Node(const T& d) : data(d), prev(nullptr), next(nullptr) {}
            Node(T&& d) : data(std::move(d)), prev(nullptr), next(nullptr) {}
        };

        public:

        List() = default;

        List(const List& other) {
            for (const T& item : other){
                this->push_back(item);
            }
        }

        List& operator=(const List& other){
            if (this != &other){
                clear();
                for (const T& item : other){
                    this->push_back(item);
                } 
            }
            return *this;
        }

        List(List&& other) : head(other.head), tail(other.tail), sz(other.sz){
            other.head = nullptr;
            other.tail = nullptr;
            other.sz = 0;
        }

        List& operator=(List&& other) noexcept {
            if (this != &other) {
                clear();
                head = other.head;
                tail = other.tail;
                sz = other.sz;
                other.head = nullptr;
                other.tail = nullptr;
                other.sz = 0;
            }
            return *this;
        }

        ~List() {
            clear();
        }

        void push_back(const T& value) {
            Node* n = new Node(value);
            if (!tail) {
                head = tail = n;
            } else {
                tail->next = n;
                n->prev = tail;
                tail = n;
            }
            sz++;
        }

        void push_front(const T& value) {
            Node* n = new Node(value);
            if (!head) {
                head = tail = n;
            } else {
                n->next = head;
                head->prev = n;
                head = n;
            }
            sz++;
        }
    
        void pop_back() {
            if (!tail) return;
            Node* to_delete = tail;
            tail = tail->prev;
            if (tail) tail->next = nullptr;
            else head = nullptr;
            delete to_delete;
            sz--;
        }
    
        void pop_front() {
            if (!head) return;
            Node* to_delete = head;
            head = head->next;
            if (head) head->prev = nullptr;
            else tail = nullptr;
            delete to_delete;
            sz--;
        }
    
        size_t size() const {
            return sz;
        }
    
        bool empty() const {
            return sz == 0;
        }
    
        void clear() {
            while (head) {
                Node* next = head->next;
                delete head;
                head = next;
            }
            head = tail = nullptr;
            sz = 0;
        }
    
        struct iterator {
            Node* curr;
    
            iterator(Node* node) : curr(node) {}
    
            T& operator*() const { return curr->data; }
            T* operator->() const { return &(curr->data); }
    
            iterator& operator++() { curr = curr->next; return *this; }
            iterator operator++(int) { iterator tmp = *this; ++(*this); return tmp; }
    
            iterator& operator--() { curr = curr->prev; return *this; }
            iterator operator--(int) { iterator tmp = *this; --(*this); return tmp; }
    
            bool operator==(const iterator& other) const { return curr == other.curr; }
            bool operator!=(const iterator& other) const { return curr != other.curr; }
        };
    
        iterator begin() const { return iterator(head); }
        iterator end() const { return iterator(nullptr); }
    
        iterator insert(iterator pos, const T& value) {
            if (pos.curr == nullptr) {
                push_back(value);
                return iterator(tail);
            }
            Node* n = new Node(value);
            Node* curr = pos.curr;
            n->next = curr;
            n->prev = curr->prev;
            if (curr->prev)
                curr->prev->next = n;
            else
                head = n;
            curr->prev = n;
            sz++;
            return iterator(n);
        }
    
        iterator erase(iterator pos) {
            if (!pos.curr) return end();
    
            Node* curr = pos.curr;
            Node* next = curr->next;
    
            if (curr->prev)
                curr->prev->next = curr->next;
            else
                head = curr->next;
    
            if (curr->next)
                curr->next->prev = curr->prev;
            else
                tail = curr->prev;
    
            delete curr;
            sz--;
            return iterator(next);
        }

        private:

        Node* head = nullptr;
        Node* tail = nullptr;

        size_t sz = 0;

        
    };
}

#endif