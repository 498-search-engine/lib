#ifndef BLOOMFILTER_H
#define BLOOMFILTER_H

#include <cassert>
#include <cmath>
#include <cstring>
#include <stdexcept>
#include <string>
#include <vector>
#include <openssl/md5.h>

namespace core {

/**
 * A memory-efficient probabilistic data structure for approximate set membership testing.
 *
 * Memory Usage: ~(-1.44 * n * log(ε)) bits, where n = expected items, ε = false positive rate
 * Time Complexity: O(k) for insert/query, where k = number of hash functions
 * Thread Safety: None - external synchronization required
 */
class Bloomfilter {
public:
    /**
     * Initialize Bloom filter with given capacity and error rate.
     *
     * @param num_objects Expected number of unique items
     * @param false_positive_rate Acceptable false positive rate (0 < ε < 1)
     * @throws std::runtime_error on memory allocation failure
     *
     * Memory allocated = ceil(bits_needed / 64) * 8 bytes
     * Actual bits may be slightly higher due to 64-bit alignment
     */
    Bloomfilter(int num_objects, double false_positive_rate) {
        if (num_objects <= 0)
            throw std::runtime_error("Number of objects must be positive");
        if (false_positive_rate <= 0.0 || false_positive_rate >= 1.0)
            throw std::runtime_error("False positive rate must be between 0 and 1");

        // Calculate optimal bits using formula: m = -n*ln(ε)/(ln(2))²
        double bits_needed = -(num_objects * log(false_positive_rate)) / (log(2) * log(2));
        num_bits = (size_t)ceil(bits_needed);
        num_bits = ((num_bits + 63) / 64) * 64;  // Round to multiple of 64 for uint64_t storage

        // Calculate optimal hash function count: k = (m/n)*ln(2)
        num_hashes = (size_t)round((double)num_bits / num_objects * log(2));

        // Allocate bit array. May throw std::bad_alloc if system is low on memory
        try {
            bits.resize(num_bits / 64, 0);
        } catch (const std::bad_alloc&) {
            throw std::runtime_error("Failed to allocate Bloom filter");
        }
    }

    /**
     * Insert item into the filter.
     * Never fails/throws (noexcept), but may have false positives later if filter is overfilled.
     *
     * @param s Item to insert
     */
    void insert(const std::string& s) noexcept {
        auto hash_pair = hash(s);
        uint64_t h1 = hash_pair.first;
        uint64_t h2 = hash_pair.second;

        // Double hashing scheme: idx = (h1 + i*h2) % m
        // Produces k different indices from 2 hash values
        for (size_t i = 0; i < num_hashes; i++) {
            size_t bit_idx = (h1 + i * h2) % num_bits;
            // Fast division/modulo by powers of 2 using bit operations
            size_t block = bit_idx >> 6;   // Equivalent to / 64
            size_t offset = bit_idx & 63;  // Equivalent to % 64
            bits[block] |= (1ULL << offset);
        }
    }

    /**
     * Test for item membership.
     * Never fails/throws (noexcept).
     *
     * @param s Item to test
     * @return false = definitely not in set, true = probably in set
     */
    bool contains(const std::string& s) const noexcept {
        auto hash_pair = hash(s);
        uint64_t h1 = hash_pair.first;
        uint64_t h2 = hash_pair.second;

        for (size_t i = 0; i < num_hashes; i++) {
            size_t bit_idx = (h1 + i * h2) % num_bits;
            size_t block = bit_idx >> 6;
            size_t offset = bit_idx & 63;
            if (!(bits[block] & (1ULL << offset)))
                return false;  // If any bit is 0, item definitely not present
        }
        return true;  // All bits 1 - item probably present
    }

    /**
     * Current memory usage in bytes.
     * Actual process memory may be higher due to memory allocator overhead.
     */
    size_t memory_usage() const { return bits.size() * sizeof(uint64_t); }

private:
    std::vector<uint64_t> bits;  // Bit array storage (64-bit blocks)
    size_t num_hashes;           // Number of hash functions (k)
    size_t num_bits;             // Total number of bits (m)

    /**
     * Hash function using MD5 for uniformity.
     * Splits 128-bit MD5 hash into two 64-bit values for double hashing.
     *
     * Note: MD5 is cryptographically broken but still good for hash tables.
     * Performance: ~20-30 CPU cycles per byte on modern processors.
     */
    std::pair<uint64_t, uint64_t> hash(const std::string& datum) const noexcept {
        unsigned char md[MD5_DIGEST_LENGTH];
        MD5(reinterpret_cast<const unsigned char*>(datum.c_str()), datum.length(), md);

        uint64_t h1, h2;
        memcpy(&h1, md, sizeof(h1));
        memcpy(&h2, md + sizeof(h1), sizeof(h2));

        return std::make_pair(h1, h2);
    }
};

} // namespace core

#endif
