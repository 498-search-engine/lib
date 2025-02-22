// Simple hash table template.

// Nicole Hamilton  nham@umich.edu

#pragma once

#include <cassert>
#include <iostream>
#include <iomanip>
#include <cstdint>

#include <utility> // std::move

using namespace std;


// You may add additional members or helper functions.


// Compare C-strings, return true if they are the same.

bool CompareEqual( const char *L, const char *R );


template< typename Key, typename Value > class Tuple
   {
   public:
      Key key;
      Value value;

      Tuple( const Key &k, const Value& v ) : key(k), value(v) { }
      Tuple (Key&& k, Value&& v) : key(std::move(k)), value(std::move(v)) {}
      Tuple (Tuple&& t) : Tuple(std::move(t.key), std::move(t.value)) {}
   };


template< typename Key, typename Value > class Bucket
   {
   public:
      Bucket *next;
      uint32_t hashValue;
      Tuple< Key, Value > tuple;

      Bucket( const Key &k, uint32_t h, const Value v ) :
            tuple( k, v ), next( nullptr ), hashValue( h )
         {
         }

      ~Bucket() {
         delete next;
      }
   };


template< typename Key, typename Value > class HashTable
   {
   private:

      // Your code here.
      using HashFn = uint32_t(const Key&);
      using KeyCmpFn = bool(const Key&, const Key&);

      Bucket< Key, Value > **buckets;
      HashFn& rawHashFunc;
      KeyCmpFn& keyCmp;
      size_t capacity;
      size_t size;
      const double max_load_factor;
      const size_t max_bucket_size;
      bool bucket_size_exceeded;

      friend class Iterator;
      friend class HashBlob;

      inline uint32_t GetRawHash(const Key& k) const {
         return rawHashFunc(k);
      }

      inline bool KeyCmp(const Key& a, const Key& b) const {
         return keyCmp(a,b);
      }
   
      void Rehash(double load, double factor = 1.5) {
         size_t new_cap = capacity * factor * load;
         Bucket<Key, Value>** new_buckets = new Bucket<Key,Value>*[new_cap]{};

         // Rehash
         Iterator it = this->begin();
         while (it != this->end()) {
            auto* const bucket_ptr = it.node;
            // NOTE: 'it' must be updated here before it.next field altered
            it++;
            const auto new_hash = bucket_ptr->hashValue % new_cap;
            bucket_ptr->next = new_buckets[new_hash];
            new_buckets[new_hash] = bucket_ptr;
         }

         // NOTE: be cautious of double deletes if changing this
         delete[] buckets;
         buckets = new_buckets;
         capacity = new_cap;

         Iterator i = this->begin();
      }

   public:

      Tuple< Key, Value > *Find( const Key k, const Value initialValue )
         {
         // Search for the key k and return a pointer to the
         // ( key, value ) entry.  If the key is not already
         // in the hash, add it with the initial value.

         const auto raw_hash = GetRawHash(k);
         const auto hash = raw_hash % capacity;
         auto& bucket = buckets[hash];
         if (bucket == nullptr) {
            size++;
            bucket = new Bucket<Key,Value>(k, raw_hash, initialValue);
            return &bucket->tuple;
         }

         size_t bucket_size = 0;
         auto* cur = bucket;
         while (cur->hashValue != raw_hash || !KeyCmp(cur->tuple.key, k)) {
            if (cur->next == nullptr) { // need to insert
               size++;
               cur->next = new Bucket<Key,Value>(k, raw_hash, initialValue);
               return &cur->next->tuple;
            }
            cur = cur->next;
            bucket_size++;
         }

         if (bucket_size > max_bucket_size) // need to rehash
            bucket_size_exceeded = true;

         // Found it
         return &cur->tuple;
         }

      Tuple< Key, Value > *Find( const Key k ) const
         {
         // Search for the key k and return a pointer to the
         // ( key, value ) enty.  If the key is not already
         // in the hash, return nullptr.

         const auto raw_hash = GetRawHash(k);
         const auto hash = raw_hash % capacity;
         auto* bucket = buckets[hash];
         while (bucket) {
            if (bucket->hashValue == raw_hash && KeyCmp(bucket->tuple.key, k))
               return &bucket->tuple;
            bucket = bucket->next;
         }
         return nullptr;
         }

      void Optimize( double loading = 1.5 )
         {
         // Modify or rebuild the hash table as you see fit
         // to improve its performance now that you know
         // nothing more is to be added.

            const double load_factor = static_cast<double>(size) /
                                        static_cast<double>(capacity);
            if (load_factor > max_load_factor || bucket_size_exceeded)
               Rehash(load_factor, loading);
         }

      size_t Size() const noexcept { return size; }

      // Your constructor may take as many default arguments
      // as you like.

      HashTable(HashFn& hash, KeyCmpFn& key_cmp, size_t capacity=5, double max_load_factor=3.0, size_t max_bucket_size=50)
         : rawHashFunc(hash), keyCmp(key_cmp), capacity(capacity), max_load_factor(max_load_factor),
           max_bucket_size(max_bucket_size)
         {
            buckets = new Bucket<Key,Value>*[capacity]{};
            bucket_size_exceeded = false;
            size = 0;
         }


      ~HashTable( )
         {
            for (size_t i = 0; i < capacity; i++) {
               delete buckets[i];
            }
            delete[] buckets;
         }


      class Iterator
         {
         private:

            friend class HashTable;

            HashTable* table;
            Bucket<Key, Value>* node;
            size_t bucket;

            Iterator( HashTable *table, size_t bucket, Bucket<Key, Value> *b )
               : table(table), node(b), bucket(bucket)
               {}

         public:

            Iterator( ) : Iterator( nullptr, 0, nullptr )
               {
               }

            ~Iterator( )
               {
               }

            Tuple< Key, Value > &operator*( )
               {
                  return node->tuple; // not valid for end iterators
               }

            Tuple< Key, Value > *operator->( ) const
               {
                  return node ? &node->tuple : nullptr;
               }

            // Prefix ++
            Iterator &operator++( )
               {
                  if (node->next) {
                     node = node->next;
                     return *this;
                  }

                  do {
                     bucket++;
                  } while (bucket < table->capacity && !table->buckets[bucket]);

                  if (bucket >= table->capacity) { // reached end
                     node = nullptr;
                     return *this;
                  }

                  node = table->buckets[bucket];
                  return *this;
               }

            // Postfix ++
            Iterator operator++( int )
               {
                  return ++(*this);
               }

            bool operator==( const Iterator &rhs ) const
               {
                  return this->node == rhs.node;
               }

            bool operator!=( const Iterator &rhs ) const
               {
                  return this->node != rhs.node;
               }
         };

      Iterator begin( )
         {
            size_t idx = 0;
            while (idx < capacity && !this->buckets[idx])
               idx++;
            return idx == capacity ? 
               Iterator{} : Iterator{this, idx, this->buckets[idx]};
         }

      Iterator end( )
         {
            return Iterator{};
         }
   };
