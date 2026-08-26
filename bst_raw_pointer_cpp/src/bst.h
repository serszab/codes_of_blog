#pragma once

#include <functional>
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
        T value;
        Node* left{ nullptr };
        Node* right{ nullptr };
    };

    class BSTException : public std::exception {};

public:
    BinarySearchTree() = default;
    BinarySearchTree(const BinarySearchTree& other);
    BinarySearchTree(BinarySearchTree&& other) noexcept;
    ~BinarySearchTree();

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
    Node* Insert(Node* node, const T& value);
    Node* Dispose(Node* node);
    
    template<Traversal traversalType>
    void Traverse(Node* node, std::function<void(const T&)> func) const;

    size_t Height(Node* node) const;
    Node* Erase(Node* node, const T& value);

private:
    Node* root{ nullptr };
};

template<typename T>
BinarySearchTree<T>::BinarySearchTree(const BinarySearchTree& other)
{
    other.Traverse<Traversal::PreOrder>([this](const T& item){ this->Insert(item); });
}

template<typename T>
BinarySearchTree<T>::BinarySearchTree(BinarySearchTree&& other) noexcept
    : root{ other.root}
{
    other.root = nullptr;
}

template<typename T>
BinarySearchTree<T>::~BinarySearchTree()
{
    root = Dispose(root);
}

template<typename T>
BinarySearchTree<T>& BinarySearchTree<T>::operator=(const BinarySearchTree& other)
{
    if (this == &other) return *this;

    root = Dispose(root);
    other.Traverse<Traversal::PreOrder>([this](const T& item){ this->Insert(item); });
    return *this;
}

template<typename T>
BinarySearchTree<T>& BinarySearchTree<T>::operator=(BinarySearchTree&& other) noexcept
{
    if (this == &other) return *this;

    root = Dispose(root);
    root = other.root;
    other.root = nullptr;
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
    try {
        root = Insert(root, value);
        return true;
    } catch(const BSTException&) {
        return false;
    }
}

template<typename T>
typename BinarySearchTree<T>::Node* BinarySearchTree<T>::Insert(Node* node, const T& value)
{
    if (node == nullptr) {
        return new Node{ value };
    }

    if (value == node->value) {
        throw BSTException{};
    }

    if (value < node->value) {
        node->left = Insert(node->left, value);
    } else {
        node->right = Insert(node->right, value);
    }

    return node;
}

template<typename T>
bool BinarySearchTree<T>::Contains(const T& value) const
{
    Node* current = root;

    while (current != nullptr) {
        if (value == current->value) {
            return true;
        }

        if (value < current->value) {
            current = current->left;
        } else {
            current = current->right;
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

    Node* current = root;
    while (current->left != nullptr) {
        current = current->left;
    }

    return std::make_optional<T>(current->value);
}

template<typename T>
std::optional<T> BinarySearchTree<T>::Max() const
{
    if (Empty()) { 
        return {};
    };

    Node* current = root;
    while (current->right != nullptr) {
        current = current->right;
    }

    return std::make_optional<T>(current->value);
}

template<typename T>
template<Traversal traversalType>
void BinarySearchTree<T>::Traverse(std::function<void(const T&)> func) const
{
    Traverse<traversalType>(root, func);
}

template<typename T>
template<Traversal traversalType>
void BinarySearchTree<T>::Traverse(Node* node, std::function<void(const T&)> func) const
{
    if (node == nullptr) return;

    if (traversalType == Traversal::PreOrder) func(node->value);
    Traverse<traversalType>(node->left, func);
    if (traversalType == Traversal::InOrder) func(node->value);
    Traverse<traversalType>(node->right, func);
    if (traversalType == Traversal::PostOrder) func(node->value);
}

template<typename T>
typename BinarySearchTree<T>::Node* BinarySearchTree<T>::Dispose(Node* node)
{
    if (node == nullptr) return node;

    Dispose(node->left);
    Dispose(node->right);
    delete node;
    return nullptr;
}

template<typename T>
size_t BinarySearchTree<T>::Height() const
{
    return Height(root);   
}

template<typename T>
size_t BinarySearchTree<T>::Height(Node* node) const
{
    if (node == nullptr) return 0;
    
    return 1 + std::max(Height(node->left), Height(node->right));
}

template<typename T>
bool BinarySearchTree<T>::Erase(const T& value)
{
    try {
        root = Erase(root, value);
        return true;
    } catch(const BSTException&) {
        return false;
    }
}

template<typename T>
typename BinarySearchTree<T>::Node* BinarySearchTree<T>::Erase(Node* node, const T& value)
{
    if (node == nullptr) throw BSTException{};

    if (value < node->value) {
        node->left=Erase(node->left, value);
    } else if (value > node->value) {
        node->right=Erase(node->right, value);
    } else {
        if (node->left == nullptr) {
            Node* temp = node;
            node = node->right;
            delete temp;
        } else if (node->right == nullptr) {
            Node* temp = node;
            node = node->left;
            delete temp;
        } else {
            Node* current = node->right;
            while (current->left != nullptr) {
                current = current->left;
            }
            node->value = current->value;
            node->right=Erase(node->right, current->value);
        }
    }
    return node;
}