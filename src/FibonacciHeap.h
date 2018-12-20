//
// Created by Dima Zapolsky on 05/12/2018.
//

#ifndef LABA_FIBONACCIHEAP_H
#define LABA_FIBONACCIHEAP_H

#include <iostream>
#include "Dynamic_array.h"
#include "swap.h"

template <typename Key>
class FibonacciHeap {
public:
    class Pointer;
private:
    class Buffer;

    class Node {
    public:
        Key key;
        Node * parent;
        Node * child;
        Node * left;
        Node * right;
        int deg;
        bool mark;
        Buffer * buffer;

        Node(Key key, Node * parent = nullptr, Node * child = nullptr, Node * left = nullptr, Node * right = nullptr, int deg = 0, bool mark = false, Buffer * buffer = nullptr);
        ~Node();
    };

    class Buffer {
    public:
        friend class FibonacciHeap<Key>;
    private:
        Node * node;
        Buffer(Node * node = nullptr);
        ~Buffer();
    };

    Node * min_node;
    int size;

    void unionLists(Node * first, Node * second);
    void consolidate();
    void cut(Node * node);
    void cascadingCut(Node * node);
public:
    class Pointer {
    private:
        Buffer * buffer;
        Pointer (Buffer * buffer);
    public:
        friend class FibonacciHeap<Key>;
        Pointer();
        ~Pointer();
    };

    FibonacciHeap();
    ~FibonacciHeap();
    bool is_empty() const;
    Key get_min() const;
    Key extract_min();
    void merge(FibonacciHeap &otherHeap);
    Pointer insert(Key key);
    void decrease(Pointer ptr, Key key);
};

template <typename Key>
FibonacciHeap<Key>::Node::Node(Key key, Node * parent, Node * child, Node * left, Node * right, int deg, bool mark, Buffer * buffer) : key(key), parent(parent), child(child), left(left), right(right), deg(deg), mark(mark), buffer(buffer) {}

template <typename Key>
FibonacciHeap<Key>::Node::~Node() {}

template <typename Key>
FibonacciHeap<Key>::Buffer::Buffer(FibonacciHeap<Key>::Node *node) : node(node) {}

template <typename Key>
FibonacciHeap<Key>::Buffer::~Buffer() {}

template <typename Key>
FibonacciHeap<Key>::Pointer::Pointer() : buffer(nullptr) {}

template <typename Key>
FibonacciHeap<Key>::Pointer::Pointer(FibonacciHeap<Key>::Buffer *buffer) : buffer(buffer) {}

template <typename Key>
FibonacciHeap<Key>::Pointer::~Pointer() {}

template <typename Key>
FibonacciHeap<Key>::FibonacciHeap() {
    size = 0;
    min_node = nullptr;
}

template <typename Key>
FibonacciHeap<Key>::~FibonacciHeap() {
    if (min_node != nullptr)
        delete min_node;
}

template <typename Key>
bool FibonacciHeap<Key>::is_empty() const {
    return size == 0;
}

template <typename Key>
typename FibonacciHeap<Key>::Pointer FibonacciHeap<Key>::insert(Key key) {
    Node * new_node = new Node(key);
    Buffer * buffer = new Buffer(new_node);
    Pointer ptr(buffer);
    if (size == 0) {
        min_node = new_node;
        min_node->left = min_node;
        min_node->right = min_node;
    }
    else {
        Node * prev_right = min_node->right;
        min_node->right = new_node;
        new_node->left = min_node;
        new_node->right = prev_right;
        prev_right->left = new_node;
    }
    if (new_node->key < min_node->key) {
        min_node = new_node;
    }
    size++;
    return ptr;
}

template <typename Key>
void FibonacciHeap<Key>::unionLists(Node *first, Node *second) {
    if (first == nullptr || second == nullptr) {
        return;
    }
    Node * L = first->left;
    Node * R = second->right;
    second->right = first;
    first->left = second;
    L->right = R;
    R->left = L;
}

template <typename Key>
Key FibonacciHeap<Key>::get_min() const {
    if (size == 0) {
        throw std::out_of_range("Heap is empty");
    }
    return min_node->key;
}

template <typename Key>
void FibonacciHeap<Key>::merge(FibonacciHeap<Key> &otherHeap) {
    if (otherHeap.is_empty()) {
        return;
    }
    if (is_empty()) {
        min_node = otherHeap.min_node;
        size = otherHeap.size;
        otherHeap.min_node = nullptr;
        otherHeap.size = 0;
        return;
    }
    unionLists(min_node, otherHeap.min_node);
    size += otherHeap.size;
    if (min_node->key > otherHeap.min_node->key) {
        min_node = otherHeap.min_node;
    }
    otherHeap.min_node = nullptr;
    otherHeap.size = 0;
}

template <typename Key>
Key FibonacciHeap<Key>::extract_min() {
    if (is_empty()) {
        throw std::out_of_range("Heap is empty");
    }
    Node * prev_min = min_node;
    unionLists(min_node, min_node->child);
    Node * L = min_node->left;
    Node * R = min_node->right;
    L->right = R;
    R->left = L;
    if (prev_min->right == prev_min) {
        min_node = nullptr;
        size--;
        return prev_min->key;
    }
    min_node = min_node->right;
    consolidate();
    size--;
    return prev_min->key;
}

template <typename Key>
void FibonacciHeap<Key>::consolidate() {
    Node* nodes[32];
    for (int i = 0; i < 32; ++i)
        nodes[i] = nullptr;
    dynamic_array<Node*> arr;
    arr.push_back(min_node);
    Node * now = min_node->right;
    while (now != min_node) {
        arr.push_back(now);
        now = now->right;
    }
    for (int i = 0; i < arr.size(); ++i) {
        while (nodes[arr[i]->deg] != nullptr) {
            Node * conflict = nodes[arr[i]->deg];
            if (conflict->key < arr[i]->key) {
                swap(conflict, arr[i]);
            }
            Node * L = conflict->left;
            Node * R = conflict->right;
            L->right = R;
            R->left = L;
            conflict->parent = arr[i];
            arr[i]->deg++;
            conflict->right = conflict;
            conflict->left = conflict;
            if (arr[i]->child == nullptr) {
                arr[i]->child = conflict;
            }
            else {
                unionLists(arr[i]->child, conflict);
            }
            conflict->mark = false;
            nodes[conflict->deg] = nullptr;
        }
        nodes[arr[i]->deg] = arr[i];
    }
    min_node = nullptr;
    for (int i = 0; i < 32; ++i) {
        if (nodes[i] != nullptr && (min_node == nullptr || min_node->key > nodes[i]->key)) {
            min_node = nodes[i];
        }
    }
}

template <typename Key>
void FibonacciHeap<Key>::decrease(Pointer ptr, Key key) {
    if (ptr.buffer == nullptr || ptr.buffer->node == nullptr) {
        throw std::invalid_argument("Pointer is null");
    }
    Node * node = ptr.buffer->node;
    if (key > node->key) {
        throw std::logic_error("Key should be smaller than current");
    }
    node->key = key;
    if (node->parent != nullptr && key < node->parent->key) {
        cut(node);
        cascadingCut(node->parent);
    }
    if (key < min_node->key) {
        min_node = node;
    }
}

template <typename Key>
void FibonacciHeap<Key>::cut(Node *node) {
    Node * L = node->left;
    Node * R = node->right;
    R->left = L;
    L->right = R;
    node->right = node;
    node->left = node;
    node->parent->deg--;
    if (node->parent->child == node) {
        if (node->right == node) {
            node->parent->child = nullptr;
        }
        else {
            node->parent->child = node->right;
        }
    }
    node->mark = false;
    node->parent = nullptr;
    unionLists(min_node, node);
}

template <typename Key>
void FibonacciHeap<Key>::cascadingCut(Node * node) {
    if (node->parent != nullptr) {
        if (!node->mark) {
            node->mark = true;
        }
        else {
            cut(node);
            cascadingCut(node->parent);
        }
    }
}

#endif //LABA_FIBONACCIHEAP_H
