#pragma once

#include <functional>
#include <iostream>
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
        Node(const Node&) = delete;
        Node(Node&&) = delete;
        ~Node() {
            std::cout << "Node(" << value << ") deleted" << std::endl;
        }

        T value;
        std::unique_ptr<Node> left{ nullptr };
        std::unique_ptr<Node> right{ nullptr };
    };

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
}

template<typename T>
BinarySearchTree<T>& BinarySearchTree<T>::operator=(const BinarySearchTree& other)
{
    if (this == &other) return *this;

    root.reset();
    other.Traverse<Traversal::PreOrder>([this](const T& item){ this->Insert(item); });
    return *this;
}

template<typename T>
BinarySearchTree<T>& BinarySearchTree<T>::operator=(BinarySearchTree&& other) noexcept
{
    if (this == &other) return *this;

    root = std::move(other.root);
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
    std::unique_ptr<Node>* current = &root;

    while (*current != nullptr) {
        if (value == (*current)->value) {
            return false;
        }

        current = value < (*current)->value ? &(*current)->left : &(*current)->right;
    }

    *current = std::make_unique<Node>(value);
    return true;
}

template<typename T>
bool BinarySearchTree<T>::Contains(const T& value) const
{
    const std::unique_ptr<Node>* current = &root;

    while (*current != nullptr) {
        if (value == (*current)->value) {
            return true;
        }

        if (value < (*current)->value) {
            current = &(*current)->left;
        } else {
            current = &(*current)->right;
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

    const std::unique_ptr<Node>* current = &root;
    while ((*current)->left != nullptr) {
        current = &(*current)->left;
    }

    return std::make_optional<T>((*current)->value);
}

template<typename T>
std::optional<T> BinarySearchTree<T>::Max() const
{
    if (Empty()) { 
        return {};
    };

    const std::unique_ptr<Node>* current = &root;
    while ((*current)->right != nullptr) {
        current = &(*current)->right;
    }

    return std::make_optional<T>((*current)->value);
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
    std::unique_ptr<Node>* current = &root;

    while (*current != nullptr && (*current)->value != value) {
        current = value < (*current)->value ? &(*current)->left : &(*current)->right;
    }
    if (*current == nullptr) return false;

    if ((*current)->left == nullptr) {
        *current = std::move((*current)->right);
        return true;
    }
    if ((*current)->right == nullptr) {
        *current = std::move((*current)->left);
        return true;
    }

    std::unique_ptr<Node>* next = &(*current)->right;
    while ((*next)->left != nullptr) {
        next = &(*next)->left;
    }
    (*current)->value = (*next)->value;
    *next = std::move((*next)->right);
    return true;
}
