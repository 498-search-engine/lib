#ifndef CORE_UNORDERED_MAP_H
#define CORE_UNORDERED_MAP_H

#include "vector.h"
#include <functional>
#include <iostream>

namespace core {

    enum class Status : uint8_t {
        Empty,
        Occupied,
        Deleted
    };
    
    template<typename K, typename V, typename Hasher = std::hash<K>>
    class UnorderedMap {
    public:
        // A bucket has a status, a key, and a value.
        struct Bucket {
            // Do not modify Bucket.
            Status status = Status::Empty;
            K key;
            V val;
        };
    
        UnorderedMap() {
            // TODO: a default constructor (possibly empty).
            buckets.resize(20);
            num_elements = 0;
            // You can use the following to avoid implementing rehash_and_grow().
            // However, you will only pass the AG test cases ending in _C.
            // To pass the rest, start with at most 20 buckets and implement rehash_and_grow().
            //    buckets.resize(10000);
        }
    
        size_t size() const {
            return num_elements;
        }
    
        // returns a reference to the value in the bucket with the key, if it
        // already exists. Otherwise, insert it with a default value, and return
        // a reference to the resulting bucket.
        V& operator[](const K& key) {
            //std::cout << "[] before grow\n";
           if (num_elements >= buckets.size() / 2) rehash_and_grow();
           //std::cout << "[] after grow\n";
            size_t key_hash = my_hash(key);
            //std::cout << "[] before search\n";
            size_t res = search(key, key_hash);
            //std::cout << "[] after search\n";
            if (buckets[res].status == Status::Occupied ) {
                return buckets[res].val;
            }
            else {
                //buckets[res].status = Status::Occupied;
                //buckets[res].val = V();
                insert(key, V());
                //std::cout << "[] after insert\n";
                return buckets[res].val;
            }
        }
    
        // insert returns whether inserted successfully
        // (if the key already exists in the table, do nothing and return false).
        bool insert(const K& key, const V& val) {
            //std::cout << "insert start\n";
            if (num_elements >= buckets.size() / 2) rehash_and_grow();
            //std::cout << "insert after grow\n";
            size_t key_hash = my_hash(key);
            size_t res = search(key, key_hash);
            //std::cout << "insert after search\n";
            if (buckets[res].status == Status::Occupied) return false;
            else {
                buckets[res].status = Status::Occupied;
                buckets[res].key = key;
                buckets[res].val = val;
                num_elements++;
                return true;
            }
        }
        // erase returns the number of items remove (0 or 1)
        size_t erase(const K& key) {
            size_t key_hash = my_hash(key);
            size_t res = search(key, key_hash);
            if (buckets[res].status == Status::Occupied) {
                buckets[res].status = Status::Deleted;
                num_elements--;
                num_deleted++;
                return 1;
            }
            return 0;
        }

        class Iterator {
            private:
            typename vector<Bucket>::Iterator iter;
            
            public:
            explicit Iterator(typename vector<Bucket>::Iterator it) : iter(it) {}
            
            std::pair<K, V>& operator*() { 
                const auto& buck = reinterpret_cast<Bucket&>(*iter); 
                return {buck.key, buck.val};
            }
            Iterator& operator++() { ++iter; return *this; }
            bool operator!=(const Iterator& other) const { return iter != other.iter; }
            bool operator==(const Iterator& other) const {return not (*this != other); }
        };

        Iterator begin() { return Iterator(buckets.begin()); }
        Iterator end() { return Iterator(buckets.end()); }

        Iterator find(const K& key){
            auto idx = search(key, my_hash(key));
            return buckets[idx].status == Status::Occupied ? Iterator(buckets.begin()+idx) : end();
        }

    
    private:
        size_t num_elements = 0;
        size_t num_deleted = 0; // OPTIONAL: you don't need to use num_deleted to pass
        vector<Bucket> buckets;
    
        void rehash_and_grow() {
            // You can avoid implementing rehash_and_grow() by calling
            //    buckets.resize(10000);
            // in the constructor. However, you will only pass the AG test cases ending in _C.
            // To pass the rest, start with at most 20 buckets and implement rehash_and_grow().
    
            vector<Bucket> temp;
            //std::cout << "grow after making vec\n";
            temp.resize(buckets.size() * 2);
            //std::cout << "grow after resize vec\n";
            std::swap(temp, buckets);
            //std::cout << "grow after swap vec\n";
    
            num_elements = 0;
            for (size_t i = 0; i < temp.size(); ++i) {
                //std::cout << i << " ";
                if (temp[i].status == Status::Occupied) {
                    insert(temp[i].key, temp[i].val);
                }
            }
            //std::cout << "\n";
        }
    
        // You can add methods here if you like.
        size_t my_hash (const K &key){
            //std::cout << "in my_hash " << key << " " << buckets.size() << "\n";
            Hasher hasher;
            auto val = hasher(key) % buckets.size();
            //std::cout << "my_hash made the hash " << val << "\n";
            return val;
        }
    
        size_t search(const K& key, size_t key_search) {
            bool del_found = false;
            size_t d_ind = 0;
            for (int i = 0; i < buckets.size(); ++i) {
                
                size_t alt_ind = (i + key_search) % buckets.size();
                if (buckets[alt_ind].status == Status::Occupied && key == buckets[alt_ind].key) {
                    return alt_ind;
                }
                if (buckets[alt_ind].status == Status::Deleted && !del_found) {
                    d_ind = alt_ind;
                    del_found = true;
                }
                if (buckets[alt_ind].status == Status::Empty) {
                    if (del_found) return d_ind;
                    else return alt_ind;
                }
            }
            return d_ind;
        }
    
    };    

}
#endif