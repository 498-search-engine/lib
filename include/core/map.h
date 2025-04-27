/**
 * @file map.h
 * @author Christopher Davis
 * @brief A (mostly) C++20 compliant std::map implementation
 * @version 1.0
 * @date 2025-04-27
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#ifndef LIB_MAP_H
#define LIB_MAP_H

#include <functional>
#include <utility>
#include <memory>
#include <algorithm>
#include <concepts>
#include <limits>
#include <stdexcept>
#include <cassert>
#include <vector>

namespace core {

template<class Key, class T, class Compare=std::less<Key>>
class Map {
private:
    struct Node {
        std::pair<const Key, T> data;
        std::unique_ptr<Node> left;
        std::unique_ptr<Node> right;
        Node* parent;
        int height;

        Node(const Key& k, const T& v, Node* p=nullptr)
            : data(k,v), left(nullptr), right(nullptr), parent(p), height(1) {}
    };

    std::unique_ptr<Node> root_;
    size_t node_count_;
    Compare comp_;

public:
    class iterator;

    /* Types */
    using key_type = Key;
    using mapped_type = T;
    using value_type = std::pair<const Key, T>;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using key_compare = Compare;
    struct value_compare {
        Compare comp;
        using is_transparent = void;
        value_compare(Compare c = Compare()) : comp(c) {}
        bool operator()(const value_type& a, const value_type& b) const {
            return comp(a.first, b.first);
        }
    };
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = value_type*;
    using const_pointer = const value_type*;
    using const_iterator = iterator;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = reverse_iterator;

    /* Constructors */
    Map() noexcept : root_(nullptr), node_count_(0), comp_(Compare()) {}
    explicit Map(const Compare& comp) : root_(nullptr), node_count_(0), comp_(comp) {}

    Map(const Map& other)
        : root_(clone(other.root_.get(), nullptr)),
          node_count_(other.node_count_), comp_(other.comp_) {}

    Map& operator=(const Map& other) {
        if (this != &other) {
            Map tmp(other);
            swap(tmp);
        }
        return *this;
    }

    Map(Map&&) noexcept = default;
    Map& operator=(Map&&) noexcept = default;

    // TODO: range constructors, etc.

    ~Map() = default;

    /* Element Access */
    T& at(const Key& key) {
        const auto it = find(key);
        if (it == this->end())
            throw std::out_of_range("Map::at: key not found");
        return it->second;
    }

    const T& at(const Key& key) const {
        const auto it = find(key);
        if (it == this->end())
            throw std::out_of_range("Map::at: key not found");
        return it->second;
    }

    T& operator[](const Key& key) {
        auto [it, _] = insert(key, T{});
        return it->second;
    }

    T& operator[](Key&& key) {
        auto [it, _] = insert(key, T{});
        return it->second;
    }

    /* Capacity */
    bool empty() const noexcept { return node_count_ == 0; }
    size_type size() const noexcept { return node_count_; }
    size_type max_size() const noexcept { return std::numeric_limits<size_type>::max(); }

    /* Modifiers */
    void clear() noexcept {
        root_.reset();
        node_count_ = 0;
    }

    void swap(Map& other) noexcept {
        std::swap(root_, other.root_);
        std::swap(node_count_, other.node_count_);
        std::swap(comp_, other.comp_);
    }

    friend void swap(Map& a, Map& b) noexcept { a.swap(b); }

    std::pair<typename Map::iterator, bool> insert(const Key& key, const T& value) {
        bool inserted = false;
        root_ = insertNode(std::move(root_), nullptr, key, value, inserted);
        Node* n = findNode(root_.get(), key);
        return { iterator(n, this), inserted };
    }

    template<typename... Args>
    std::pair<iterator, bool> emplace(Args&&... args) {
        value_type val(std::forward<Args>(args)...);
        return insert(val.first, val.second);
    }

    // Returns number of elements erased (0 or 1)
    size_type erase(const Key& k) {
        bool rem=false;
        root_ = removeNode(std::move(root_), k, rem);
        return rem ? 1 : 0;
    }

    iterator erase(const_iterator pos) {
        if (!pos.node) return end();
        auto nxt = pos; ++nxt;
        erase(pos.node->data.first);
        return nxt;
    }

    // TODO: erase range

    /* Lookup */
    size_type count(const Key& key) const noexcept { return findNode(root_.get(), key) ? 1 : 0; }
    iterator find(const Key& key) const noexcept { return iterator(findNode(root_.get(), key), this); }
    bool contains(const Key& key) const { return count(key) > 0; }

    iterator lower_bound(const Key& key) const noexcept {
        Node* n = root_.get();
        Node* candidate = nullptr;
        while (n) {
            if (!comp_(n->data.first, key)) {
                candidate = n;
                n = n->left.get();
            } else {
                n = n->right.get();
            }
        }
        return iterator(candidate, this);
    }

    iterator upper_bound(const Key& key) const noexcept {
        Node* n = root_.get();
        Node* candidate = nullptr;
        while (n) {
            if (comp_(key, n->data.first)) {
                candidate = n;
                n = n->left.get();
            } else {
                n = n->right.get();
            }
        }
        return iterator(candidate, this);
    }

    std::pair<iterator,iterator> equal_range(const Key& key) const noexcept {
        return { lower_bound(key), upper_bound(key) };
    }

    /* Observers */
    key_compare key_comp() const { return comp_; }
    value_compare value_comp() const { return value_compare(comp_); }

    /* Iterators */
    iterator begin()          const noexcept { return iterator(leftmost(root_.get()), this); }
    iterator end()            const noexcept { return iterator(nullptr, this); }
    const_iterator cbegin()   const noexcept { return begin(); }
    const_iterator cend()     const noexcept { return end(); }
    reverse_iterator rbegin() const noexcept { return reverse_iterator(end()); }
    reverse_iterator rend()   const noexcept { return reverse_iterator(begin()); }

private:
    /* Helpers*/
    [[nodiscard]] int getHeight(Node* n) const noexcept { return n ? n->height : 0; }
    [[nodiscard]] int getBalance(Node* n) const noexcept { return n ? getHeight(n->left.get()) - getHeight(n->right.get()) : 0; }
    void updateHeight(Node* n) {if (n) n->height = 1 + std::max(getHeight(n->left.get()), getHeight(n->right.get())); }

    static Node* leftmost(Node* n) noexcept {
        while (n && n->left) n = n->left.get();
        return n;
    }

    static Node* rightmost(Node* n) noexcept {
        while (n && n->right) n = n->right.get();
        return n;
    }

    std::unique_ptr<Node> rotateRight(std::unique_ptr<Node> y) {
        auto x = std::move(y->left);
        x->parent = y->parent;
        y->left = std::move(x->right);
        if (y->left) y->left->parent = y.get();
        updateHeight(y.get());
        x->right = std::move(y);
        x->right->parent = x.get();
        updateHeight(x.get());
        return x;
    }

    std::unique_ptr<Node> rotateLeft(std::unique_ptr<Node> x) {
        auto y = std::move(x->right);
        y->parent = x->parent;
        x->right = std::move(y->left);
        if (x->right) x->right->parent = x.get();
        updateHeight(x.get());
        y->left = std::move(x);
        y->left->parent = y.get();
        updateHeight(y.get());
        return y;
    }

    [[nodiscard]]
    std::unique_ptr<Node> insertNode(std::unique_ptr<Node> node, Node* parent,
                                     const Key& key, const T& value, bool& inserted)
    {
        if (!node) {
            inserted = true;
            ++node_count_;
            return std::make_unique<Node>(key, value, parent);
        }
        if (comp_(key, node->data.first)) {
            node->left = insertNode(std::move(node->left), node.get(), key, value, inserted);
        } else if (comp_(node->data.first, key)) {
            node->right = insertNode(std::move(node->right), node.get(), key, value, inserted);
        } else {
            return node;
        }

        updateHeight(node.get());
        const auto balance = getBalance(node.get());

        // Balancing cases (RR, LL, RL, LR)
        if (balance > 1 && comp_(key, node->left->data.first))
            return rotateRight(std::move(node));
        if (balance < -1 && comp_(node->right->data.first, key))
            return rotateLeft(std::move(node));
        if (balance > 1 && comp_(node->left->data.first, key)) {
            node->left = rotateLeft(std::move(node->left));
            return rotateRight(std::move(node));
        }
        if (balance < -1 && comp_(key, node->right->data.first)) {
            node->right = rotateRight(std::move(node->right));
            return rotateLeft(std::move(node));
        }
        return node;
    }

    [[nodiscard]]
    Node* findNode(Node* n, const Key& key) const noexcept {
        while (n) {
            if (comp_(key, n->data.first)) n = n->left.get();
            else if (comp_(n->data.first, key)) n = n->right.get();
            else break;
        }
        return n;
    }

    [[nodiscard]]
    static std::unique_ptr<Node> clone(Node* src, Node* parent) {
        if (!src) return nullptr;
        auto n = std::make_unique<Node>(src->data.first, src->data.second, parent);
        n->height = src->height;
        n->left = clone(src->left.get(), n.get());
        n->right = clone(src->right.get(), n.get());
        return n;
    }

    [[nodiscard]]
    std::unique_ptr<Node> removeNode(std::unique_ptr<Node> node, const Key& key, bool& removed) {
        if (!node) return nullptr;
        if (comp_(key, node->data.first)) {
            node->left = removeNode(std::move(node->left), key, removed);
            if (node->left) node->left->parent = node.get();
        } else if (comp_(node->data.first, key)) {
            node->right = removeNode(std::move(node->right), key, removed);
            if (node->right) node->right->parent = node.get();
        } else {
            removed = true;
            --node_count_;
            // Node with one or no child
            if (!node->left) {
                auto r = std::move(node->right);
                if (r) r->parent = node->parent;
                return r;
            } else if (!node->right) {
                auto l = std::move(node->left);
                if (l) l->parent = node->parent;
                return l;
            }
            // Node with two children: rotate to reduce
            if (getHeight(node->left.get()) > getHeight(node->right.get())) {
                node = rotateRight(std::move(node));
                node->right = removeNode(std::move(node->right), key, removed);
                if (node->right) node->right->parent = node.get();
            } else {
                node = rotateLeft(std::move(node));
                node->left = removeNode(std::move(node->left), key, removed);
                if (node->left) node->left->parent = node.get();
            }
        }
        // Rebalance
        updateHeight(node.get());
        int balance = getBalance(node.get());
        if (balance > 1 && getBalance(node->left.get()) >= 0)
            return rotateRight(std::move(node));
        if (balance > 1 && getBalance(node->left.get()) < 0) {
            node->left = rotateLeft(std::move(node->left));
            return rotateRight(std::move(node));
        }
        if (balance < -1 && getBalance(node->right.get()) <= 0)
            return rotateLeft(std::move(node));
        if (balance < -1 && getBalance(node->right.get()) > 0) {
            node->right = rotateRight(std::move(node->right));
            return rotateLeft(std::move(node));
        }
        return node;
    }

public:
    class iterator {
        friend class Map;
        Node* node;
        const Map* container;
        iterator(Node* n, const Map* c) noexcept : node(n), container(c) {}
    public:
        using value_type        = typename Map::value_type;
        using difference_type   = typename Map::difference_type;
        using pointer           = typename Map::pointer;
        using reference         = typename Map::reference;

        iterator() noexcept : node(nullptr), container(nullptr) {}

        reference operator*()  const noexcept { assert(node); return node->data; }
        pointer   operator->() const noexcept { assert(node); return &node->data; }

        iterator& operator++() noexcept {
            assert(container);
            if (node->right) {
                node = leftmost(node->right.get());
            } else {
                Node* p = node->parent;
                while (p && node == p->right.get()) {
                    node = p;
                    p = p->parent;
                }
                node = p;
            }
            return *this;
        }
        iterator operator++(int) noexcept { auto tmp=*this; ++*this; return tmp; }

        iterator& operator--() noexcept {
            assert(container);
            if (!node) {
                node = rightmost(container->root_.get());
            } else if (node->left) {
                node = rightmost(node->left.get());
            } else {
                Node* p = node->parent;
                while (p && node == p->left.get()) {
                    node = p;
                    p = p->parent;
                }
                node = p;
            }
            return *this;
        }
        iterator operator--(int) noexcept { auto tmp=*this; --*this; return tmp; }

        friend bool operator==(const iterator& a, const iterator& b) noexcept { return a.node == b.node; }
        friend bool operator!=(const iterator& a, const iterator& b) noexcept { return a.node != b.node; }
    };
    static_assert(std::bidirectional_iterator<iterator>);
};

}  // namespace core

#endif
