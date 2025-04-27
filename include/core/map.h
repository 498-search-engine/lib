#ifndef LIB_MAP_H
#define LIB_MAP_H

#include <functional>
#include <utility>
#include <memory>
#include <algorithm>

namespace core {

template<class Key, class T, class Compare=std::less<Key>>
class Map {
private:
    struct Node {
        std::pair<const Key, T> datum;
        std::unique_ptr<Node> left;
        std::unique_ptr<Node> right;
        int height;

        Node(const Key& k, const T& v)
            : datum(k,v), left(nullptr), right(nullptr), height(1) {}
    };

    std::unique_ptr<Node> root_;
    size_t node_count_;
    Compare comp_;

public:
    /* Types */
    using key_type = Key;
    using mapped_type = T;
    using value_type = std::pair<const Key, T>;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using key_compare = Compare;
    using reference = value_type&;
    using const_reference = const value_type&;

    /* Constructors */
    Map() : root_(nullptr), node_count_(0), comp_(Compare()) {}
    explicit Map(const Compare& comp) : root_(nullptr), node_count_(0), comp_(comp) {}

    Map(Map&& other) : root_(std::move(other.root_)), node_count_(other.node_count_), comp_(other.comp_) {}

    // TODO: range constructors

    ~Map() = default;

    /* Members */
    

private:
    [[nodiscard]] int getHeight(Node* n) const noexcept { return n ? n->height : 0}
    [[nodiscard]] int getBalance(Node* n) const noexcept { return n ? getHeight(n->left.get()) - getHeight(n->right.get()) : 0}
    void updateHeight(Node* n) {if (n) n->height = 1 + std::max(getHeight(n->left.get()), getHeight(n->right.get())); }

    std::unique_ptr<Node> rotateRight(std::unique_ptr<Node> y) {
        auto x = std::move(y->left);
        y->left = std::move(x->right);
        updateHeight(y.get());
        x->right = std::move(y);
        updateHeight(x.get());
        return x;
    }

    std::unique_ptr<Node> rotateLeft(std::unique_ptr<Node> x) {
        auto y = std::move(x->right);
        x->right = std::move(y->left);
        updateHeight(x.get());
        y->left = std::move(x);
        updateHeight(y.get());
        return y;
    }

    std::unique_ptr<Node> insertNode(std::unique_ptr<Node> node, const Key& key,
                                     const T& val, bool& inserted)
    {
        if (!node) {
            inserted = true;
            ++node_count_;
            return std::make_unique<Node>(key, val);
        }

        if (comp_(key, node->datum.first)) { // left side
            node->left = insertNode(std::move(node->left), key, val, inserted);
        } else if (comp_(node->datum.first)) { // right side
            node->right = insertNode(std::move(node->right), key, val, inserted);
        } else { // duplicate key
            return node;
        }

        updateHeight(node..get());
        const auto balance = getBalance(node.get());

        // LL
        if (balance > 1 && comp_(key, node->left->data.first))
            return rotateRight(std::move(node));
        // RR
        if (balance < -1 && comp_(node->right->data.first, key))
            return rotateLeft(std::move(node));
        // LR
        if (balance > 1 && comp_(node->left->data.first, key)) {
            node->left = rotateLeft(std::move(node->left));
            return rotateRight(std::move(node));
        }
        // RL
        if (balance < -1 && comp_(key, node->right->data.first)) {
            node->right = rotateRight(std::move(node->right));
            return rotateLeft(std::move(node));
        }

        return node;
    }

    Node* findNode(Node* n, const Key& key) const noexcept {
        while (n) {
            if (comp_(key, n->datum.first)) n = n->left.get();
            else if (comp_(n->datum.first, key)) n = n->right.get();
            else break;
        }
        return n;
    }

};

}  // namespace core

#endif
