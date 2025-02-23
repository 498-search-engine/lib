#ifndef CORE_ORDERED_MAP_FILE_H
#define CORE_ORDERED_MAP_FILE_H

#include "core/array.h"
#include "core/optional.h"
#include "core/vector_file.h"

#include <concepts>

namespace core {

template<typename T, typename A, typename B>
concept TotalOrderComparator = requires(T t, A a, B b) {
    { t(a, b) } -> std::same_as<int>;
};

/**
 * @brief OrderedMapFile is a implementation of a insert-only B+ tree that is
 * backed by a memory-mapped file.
 *
 * @tparam K Key type
 * @tparam V Value type
 * @tparam N Number of entries per node in tree
 * @tparam Compare Comparator
 */
template<typename K, typename V, size_t N, TotalOrderComparator<K, K> Compare>
    requires std::is_trivially_copyable_v<K> && std::is_trivially_copyable_v<V>
class OrderedMapFile {
    using index_t = uint32_t;

    struct Metadata {
        uint32_t size;
    };

    struct KV {
        K key;
        union {
            index_t childIndex;  // Internal node
            V value;             // Leaf node
        };
    };

    struct Node {
        uint32_t isLeaf;
        uint32_t keyCount;
        core::Array<KV, N> entries;
    };

    struct Split {
        index_t left;
        index_t right;
        K key;
    };

    struct InsertionResult {
        bool inserted;
        core::Optional<Split> split;
    };

    struct FindResult {
        index_t node;
        uint32_t entry;
    };

public:
    struct Pair {
        const K* key;
        V* value;
    };

    struct ConstPair {
        const K* key;
        const V* value;
    };

    /**
     * @brief Creates or opens a OrderedMapFile at the given path.
     *
     * @param path Path of backing file
     * @param compare Comparator to use for maintaining ordering
     */
    OrderedMapFile(const char* path, Compare compare) : nodes_(path), compare_(compare) {
        if (nodes_.Empty()) {
            nodes_.PushBack(Node{
                .isLeaf = 1,
                .keyCount = 0,
            });
        }
    }

    size_t Size() const { return nodes_.CustomData()->size; }
    bool Empty() const { return Size() == 0; }

    /**
     * @brief Attempts to insert a key-value pair into the map.
     *
     * @param key Key to insert
     * @param val Value to insert
     * @return true Insertion occurred
     * @return false Insertion did not occur -- key already present
     */
    bool Insert(K key, V val) {
        auto result = InsertInto(0, key, val);
        if (!result.inserted) {
            return false;
        }

        if (result.split) {
            // Root node was split, update with new root
            assert(result.split->left == 0);
            Node old_root = nodes_[0];
            Node new_root = Node{
                .isLeaf = 0,
                .keyCount = 2,
            };
            new_root.entries[0].key = old_root.entries[0].key;
            new_root.entries[0].childIndex = nodes_.Size();
            new_root.entries[1].key = result.split->key;
            new_root.entries[1].childIndex = result.split->right;
            nodes_.PushBack(new_root);
            std::swap(nodes_.Front(), nodes_.Back());
        }

        // Increment number of elements in map
        ++nodes_.CustomData()->size;

        return true;
    }

    /**
     * @brief Finds a key-value pair in the map.
     *
     * @param key Key to look up
     * @return Found pair, if key is in map.
     */
    template<typename ComparableKey>
        requires TotalOrderComparator<Compare, K, ComparableKey>
    core::Optional<Pair> Find(const ComparableKey& key) {
        if (auto found = FindImpl(key)) {
            return Pair{
                .key = &nodes_[found->node].entries[found->entry].key,
                .value = &nodes_[found->node].entries[found->entry].value,
            };
        }
        return core::nullopt;
    }

    /**
     * @brief Finds a key-value pair in the map.
     *
     * @param key Key to look up
     * @return Found pair, if key is in map.
     */
    template<typename ComparableKey>
        requires TotalOrderComparator<Compare, K, ComparableKey>
    core::Optional<ConstPair> Find(const ComparableKey& key) const {
        if (auto found = FindImpl(key)) {
            return ConstPair{
                .key = &nodes_[found->node].entries[found->entry].key,
                .value = &nodes_[found->node].entries[found->entry].value,
            };
        }
        return core::nullopt;
    }

    /**
     * @brief Checks whether a key is present in the map.
     *
     * @param key Key to look up
     * @return Whether the key is in the map.
     */
    template<typename ComparableKey>
        requires TotalOrderComparator<Compare, K, ComparableKey>
    bool Contains(const ComparableKey& key) const {
        return FindImpl(key).HasValue();
    }

private:
    /**
     * @brief Finds the location of a key in the map, if it exists.
     */
    template<typename ComparableKey>
        requires TotalOrderComparator<Compare, K, ComparableKey>
    core::Optional<FindResult> FindImpl(const ComparableKey& key) const {
        if (Empty()) {
            return core::nullopt;
        }

        uint32_t node_index = 0;
        while (true) {
            const Node& node = nodes_[node_index];
            if (node.keyCount == 0) {
                break;
            }
            auto pos = FindPosition(node, key);
            if (node.isLeaf) {
                if (compare_(node.entries[pos].key, key) == 0) {
                    return {FindResult{.node = node_index, .entry = pos}};
                } else {
                    return core::nullopt;
                }
            } else {
                bool exact_key_match = pos < node.keyCount && compare_(node.entries[pos].key, key) == 0;
                if (!exact_key_match) {
                    // Key should be found be in prior entry slot (if exists)
                    if (pos > 0) {
                        --pos;
                    }
                }
                node_index = node.entries[pos].childIndex;
            }
        }
        return core::nullopt;
    }

    /**
     * @brief Inserts a KV entry into a node at the given position. The node
     * must have enough space for the new entry.
     *
     * @param node_index Index of node
     * @param pos Position within entries array
     * @param kv KV to insert
     */
    void InsertKVAtPosition(index_t node_index, uint32_t pos, KV kv) {
        Node& node = nodes_[node_index];
        assert(node.keyCount < N);
        assert(pos <= node.keyCount);

        // Shift entries right to make space
        for (uint32_t i = node.keyCount; i > pos; --i) {
            node.entries[i] = node.entries[i - 1];
        }

        // Made space to insert
        node.entries[pos] = kv;
        ++node.keyCount;
    }

    /**
     * @brief Inserts a KV entry into a node in the appropriate position. The
     * node must have enough space for the new entry.
     *
     * @param node_index Index of node
     * @param kv KV to insert
     */
    void InsertKVWithSpace(index_t node_index, KV kv) {
        Node& node = nodes_[node_index];
        assert(node.keyCount < N);
        auto pos = FindPosition(node, kv.key);
        InsertKVAtPosition(node_index, pos, kv);
    }

    /**
     * @brief Inserts a key-value pair into the tree at the given node index. If
     * the target node splits as a result, the returned result describes the
     * split that occurred.
     *
     * @param node_index Node to insert into
     * @param key Key to insert
     * @param value Value to insert
     * @return InsertionResult Whether the insertion occurred, and split
     * information if any.
     */
    InsertionResult InsertInto(index_t node_index, K key, V value) {
        Node* node = &nodes_[node_index];
        if (node->isLeaf) {
            auto pos = FindPosition(*node, key);
            if (pos < node->keyCount && compare_(node->entries[pos].key, key) == 0) {
                // Key already exists
                return {false, core::nullopt};
            }

            if (node->keyCount == N) {
                // Split into two leafs
                auto split = SplitLeaf(node_index);
                if (compare_(split.key, key) <= 0) {
                    // Belongs to right split child
                    InsertKVWithSpace(split.right, KV{.key = key, .value = value});
                } else {
                    // Belongs to left split child
                    InsertKVWithSpace(split.left, KV{.key = key, .value = value});
                }
                return {true, {split}};
            } else {
                InsertKVAtPosition(node_index, pos, KV{.key = key, .value = value});
                return {true, core::nullopt};
            }
        } else {
            auto pos = FindPosition(*node, key);
            auto target_entry = pos == 0 ? pos : pos - 1;
            auto target_child = node->entries[target_entry].childIndex;

            auto result = InsertInto(target_child, key, value);
            if (!result.inserted) {
                return {false, core::nullopt};
            }

            if (!result.split) {
                // No split occurred
                return {true, core::nullopt};
            }

            // Rebind address of node -- it may have changed due to memory
            // reallocation during call to InsertInto
            node = &nodes_[node_index];

            // Ensure left-most key in child matches our first entry's key (only
            // relevant when the insertion places the new key at the start of
            // the child entry)
            assert(result.split->left == target_child);
            node->entries[target_entry].key = nodes_[target_child].entries[0].key;
            // Insert new split child record
            InsertKVWithSpace(node_index, KV{.key = result.split->key, .childIndex = result.split->right});

            if (node->keyCount == N) {
                // Node is now full, split it up
                auto current_node_split = SplitInternal(node_index);
                return {true, {current_node_split}};
            } else {
                return {true, core::nullopt};
            }
        }
    }

    /**
     * @brief Splits a leaf node into two.
     *
     * @param node_index Index of leaf node
     * @return Split Description of split
     */
    Split SplitLeaf(index_t node_index) {
        Node& old_node = nodes_[node_index];
        assert(old_node.isLeaf);
        assert(old_node.keyCount == N);

        Node new_node = Node{
            .isLeaf = 1,  // Leaf -> Two leafs
            .keyCount = N / 2,
        };

        // Copy right half of entries from old node into new node
        for (uint32_t i = 0; i < N / 2; ++i) {
            new_node.entries[i] = old_node.entries[N - (N / 2) + i];
        }

        // Update old node
        old_node.keyCount -= N / 2;

        // Add new node
        nodes_.PushBack(new_node);
        return Split{
            .left = node_index,
            .right = static_cast<index_t>(nodes_.Size() - 1),
            .key = new_node.entries[0].key,
        };
    }

    /**
     * @brief Splits an internal node into two.
     *
     * @param node_index Index of internal node
     * @return Split Description of split
     */
    Split SplitInternal(index_t node_index) {
        Node& old_node = nodes_[node_index];
        assert(!old_node.isLeaf);
        assert(old_node.keyCount <= N);

        Node new_node = Node{
            .isLeaf = 0,
            .keyCount = N / 2,
        };

        // Copy right half of entries from old node into new node
        for (uint32_t i = 0; i < N / 2; ++i) {
            new_node.entries[i] = old_node.entries[old_node.keyCount - (N / 2) + i];
        }

        // Update old node
        old_node.keyCount -= N / 2;

        // Add new node
        nodes_.PushBack(new_node);
        return Split{
            .left = node_index,
            .right = static_cast<index_t>(nodes_.Size() - 1),
            .key = new_node.entries[0].key,
        };
    }

    /**
     * @brief Finds the insertion position within a node's entries using binary
     * search.
     *
     * @tparam ComparableKey Search key type
     * @param node Node to search within
     * @param key Key to locate position for
     * @return uint32_t Index into entries where key belongs
     */
    template<typename ComparableKey>
        requires TotalOrderComparator<Compare, K, ComparableKey>
    uint32_t FindPosition(const Node& node, const ComparableKey& key) const {
        uint32_t left = 0;
        uint32_t right = node.keyCount;

        while (left < right) {
            uint32_t mid = left + ((right - left) / 2);
            if (compare_(node.entries[mid].key, key) < 0) {
                left = mid + 1;
            } else {
                right = mid;
            }
        }
        return left;
    }

    CustomVectorFile<Node, Metadata> nodes_;
    Compare compare_;
};

}  // namespace core

#endif
