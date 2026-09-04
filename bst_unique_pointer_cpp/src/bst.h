#pragma once

#include <functional>
#include <memory>
#include <optional>
#include <stdexcept>

enum class Traversal
{
    PreOrder,
    InOrder,
    PostOrder
};

template<typename T>
class BinarySearchTree
{
private:
    struct Node
    {
        explicit Node(const T& value);

        T value;
        std::unique_ptr<Node> left{ nullptr };
        std::unique_ptr<Node> right{ nullptr };
    };

    class BSTException : public std::exception {};

public:
    BinarySearchTree() = default;
    BinarySearchTree(const BinarySearchTree& other);
    BinarySearchTree(BinarySearchTree&& other) noexcept;
    ~BinarySearchTree() = default;

    BinarySearchTree& operator=(const BinarySearchTree& other);
    BinarySearchTree& operator=(BinarySearchTree&& other) noexcept;

    bool Empty() const;
    bool Insert(const T& value);
    bool Contains(const T& value) const;
    std::optional<T> Min() const;
    std::optional<T> Max() const;

    template<Traversal traversalType>
    void Traverse(std::function<void(const T&)> func) const;

    size_t Height() const;
    bool Erase(const T& value);

private:
    template<Traversal traversalType>
    void Traverse(Node* node, std::function<void(const T&)> func) const;

    size_t Height(Node* node) const;

private:
    std::unique_ptr<Node> root{ nullptr };
};

template<typename T>
BinarySearchTree<T>::Node::Node(const T& value)
    : value{ value }
{
}

template<typename T>
BinarySearchTree<T>::BinarySearchTree(const BinarySearchTree& other)
{
    other.Traverse<Traversal::PreOrder>([this](const T& item){ this->Insert(item); });
}

template<typename T>
BinarySearchTree<T>::BinarySearchTree(BinarySearchTree&& other) noexcept
    : root{ std::move(other.root) }
{
    other.root = nullptr;
}

template<typename T>
BinarySearchTree<T>& BinarySearchTree<T>::operator=(const BinarySearchTree& other)
{
    if (this == &other) return *this;

    delete root.release();
    other.Traverse<Traversal::PreOrder>([this](const T& item){ this->Insert(item); });
    return *this;
}

template<typename T>
BinarySearchTree<T>& BinarySearchTree<T>::operator=(BinarySearchTree&& other) noexcept
{
    if (this == &other) return *this;

    delete root.release();
    std::swap(root, other.root);
    return *this;
}

template<typename T>
bool BinarySearchTree<T>::Empty() const
{
    return root == nullptr;
}

template<typename T>
bool BinarySearchTree<T>::Insert(const T& value)
{
    if (Empty()) {
        root = std::make_unique<Node>(value);
        return true;
    }

    Node* current = root.get();
    while (true) {
        if (value == current->value) {
            return false;
        }

        if (value < current->value) {
            if (current->left != nullptr) {
                current = current->left.get();
                continue;
            }
            current->left = std::make_unique<Node>(value);
            return true;
        }

        if (current->right != nullptr) {
            current = current->right.get();
            continue;
        }
        current->right = std::make_unique<Node>(value);
        return true;
    }
}

template<typename T>
bool BinarySearchTree<T>::Contains(const T& value) const
{
    Node* current = root.get();

    while (current != nullptr) {
        if (value == current->value) {
            return true;
        }

        if (value < current->value) {
            current = current->left.get();
        } else {
            current = current->right.get();
        }
    }

    return false;
}

template<typename T>
std::optional<T> BinarySearchTree<T>::Min() const
{
    if (Empty()) { 
        return {};
    };

    Node* current = root.get();
    while (current->left != nullptr) {
        current = current->left.get();
    }

    return std::make_optional<T>(current->value);
}

template<typename T>
std::optional<T> BinarySearchTree<T>::Max() const
{
    if (Empty()) { 
        return {};
    };

    Node* current = root.get();
    while (current->right != nullptr) {
        current = current->right.get();
    }

    return std::make_optional<T>(current->value);
}

template<typename T>
template<Traversal traversalType>
void BinarySearchTree<T>::Traverse(std::function<void(const T&)> func) const
{
    Traverse<traversalType>(root.get(), func);
}

template<typename T>
template<Traversal traversalType>
void BinarySearchTree<T>::Traverse(Node* node, std::function<void(const T&)> func) const
{
    if (node == nullptr) return;

    if (traversalType == Traversal::PreOrder) func(node->value);
    Traverse<traversalType>(node->left.get(), func);
    if (traversalType == Traversal::InOrder) func(node->value);
    Traverse<traversalType>(node->right.get(), func);
    if (traversalType == Traversal::PostOrder) func(node->value);
}

template<typename T>
size_t BinarySearchTree<T>::Height() const
{
    return Height(root.get());   
}

template<typename T>
size_t BinarySearchTree<T>::Height(Node* node) const
{
    if (node == nullptr) return 0;
    
    return 1 + std::max(Height(node->left.get()), Height(node->right.get()));
}

template<typename T>
bool BinarySearchTree<T>::Erase(const T& value)
{
    Node* current = root.get();
    Node* prev = nullptr;

    while (current != nullptr && current->value != value) {
        prev = current;
        current = value < current->value ? current->left.get() : current->right.get();
    }
    if (current == nullptr) return false;

    if (current->left == nullptr) {
        if (prev->left.get() == current) prev->left = std::move(current->right);
        else prev->right = std::move(current->right);
        return true;
    }
    if (current->right == nullptr) {
        if (prev->left.get() == current) prev->left = std::move(current->left);
        else prev->right = std::move(current->left);
        return true;
    }

    Node* next = current->right.get();
    Node* nextPrev = nullptr;
    while (next->left != nullptr) {
        nextPrev = next;
        next = next->left.get();
    }
    current->value = next->value;
    if (nextPrev == nullptr) current->right = std::move(next->right);
    else nextPrev->left = std::move(next->right);
    return true;
}
