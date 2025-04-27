#ifndef LIB_UNORDERED_SET_H
#define LIB_UNORDERED_SET_H

#include "vector.h"
#include "list.h"
#include <iostream>
#include <utility>

namespace core {
    template<typename T>
    class UnorderedSet {
        private:
        static constexpr size_t defaultBucketCount = 16;
        static constexpr float maxLoadFactor = 0.75f;

        Vector<List<T>> buckets_;
        size_t size_ = 0;

        size_t GetBucketIndex(const T& val) const {
            return std::hash<T>{}(val) % buckets_.size();
        }

        void rehash(){
            size_t new_bucket_count = buckets_.size() * 2;
            Vector<List<T>> new_buckets(new_bucket_count);

            for (const auto& bucket : buckets_) {
                for (const auto& val : bucket) {
                    size_t new_index = std::hash<T>{}(val) % new_bucket_count;
                    new_buckets[new_index].push_back(val);
                }
            }

            buckets_ = std::move(new_buckets);
        }

        public:

        UnorderedSet() : buckets_(defaultBucketCount) {}

        UnorderedSet(const UnorderedSet& other) : buckets_(other.buckets_), size_(other.size_) {}

        UnorderedSet(UnorderedSet&& other) {
            buckets_ = std::move(other.buckets_);
            size_ = other.size_;
            other.size_ = 0;
        }

        UnorderedSet& operator=(UnorderedSet& other){
            if (this != &other){
                clear();
                buckets_ = other.buckets_;
                size_ = other.size_;
            }
            return *this;
        }

        UnorderedSet& operator=(UnorderedSet&& other) {
            if (this != &other){
                clear();
                buckets_ = std::move(other.buckets_);
                size_ = other.size_;
                other.size_ = 0;
            }
            return *this;
        }

        bool insert(const T& value) {
            if (contains(value)) {
                return false;
            }
    
            if ((float)(size_ + 1) / buckets_.size() > maxLoadFactor) {
                rehash();
            }
    
            size_t index = GetBucketIndex(value);
            buckets_[index].push_back(value);
            ++size_;
            return true;
        }
    
        bool contains(const T& value) const {
            size_t index = GetBucketIndex(value);
            for (const auto& val : buckets_[index]) {
                if (val == value) {
                    return true;
                }
            }
            return false;
        }
    
        bool Erase(const T& value) {
            size_t index = GetBucketIndex(value);
            auto& bucket = buckets_[index];
            for (auto it = bucket.begin(); it != bucket.end(); ++it) {
                if (*it == value) {
                    bucket.erase(it);
                    --size_;
                    return true;
                }
            }
            return false;
        }
    
        void clear() {
            for (auto& bucket : buckets_) {
                bucket.clear();
            }
            size_ = 0;
        }
    
        size_t Size() const {
            return size_;
        }
    
        bool Empty() const {
            return size_ == 0;
        }
    
        class UnorderedSetIterator {
        private:
            using outer_iterator = List<T>*;
            using inner_iterator = typename List<T>::ListIterator;
    
            outer_iterator outer_;
            outer_iterator outer_end_;
            inner_iterator inner_;
    
            void AdvancePastEmptyBuckets() {
                while (outer_ != outer_end_ && inner_ == outer_->end()) {
                    ++outer_;
                    if (outer_ != outer_end_)
                        inner_ = outer_->begin();
                }
            }
    
        public:
            UnorderedSetIterator(outer_iterator outer, outer_iterator outer_end)
                : outer_(outer), outer_end_(outer_end), inner_(nullptr) {
                if (outer_ != outer_end_)
                    inner_ = outer_->begin();
                AdvancePastEmptyBuckets();
            }
    
            T& operator*() const {
                return *inner_;
            }
    
            UnorderedSetIterator& operator++() {
                ++inner_;
                AdvancePastEmptyBuckets();
                return *this;
            }
    
            bool operator==(const UnorderedSetIterator& other) const {
                return outer_ == other.outer_ && (outer_ == outer_end_ || inner_ == other.inner_);
            }
    
            bool operator!=(const UnorderedSetIterator& other) const {
                return !(*this == other);
            }
        };
    
        UnorderedSetIterator begin() {
            return UnorderedSetIterator(buckets_.begin(), buckets_.end());
        }
    
        UnorderedSetIterator end() {
            return UnorderedSetIterator(buckets_.end(), buckets_.end());
        }
    
    };
}

#endif