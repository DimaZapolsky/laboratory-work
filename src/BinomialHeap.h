//
// Created by Dima Zapolsky on 04/12/2018.
//

#ifndef LABA_BINOMIALHEAP_H
#define LABA_BINOMIALHEAP_H

#include <iostream>
#include "swap.h"

template <typename Key>
class BinomialHeap {
public:
    class Pointer;
private:
    class Buffer;

    class Node {
    public:
        Key key;
        Node * parent;
        Node * child;
        Node * sibling;
        Node * last_child;
        int deg;
        Buffer * buffer;

        Node(Key key, Node * parent = nullptr, Node * child = nullptr, Node * sibling = nullptr, Node * last_child = nullptr, int deg = 0, Buffer * buffer = nullptr);
        ~Node();
    };

    class Buffer {
    public:
        friend class BinomialHeap<Key>;
    private:
        Node * node;
        Buffer(Node * node = nullptr);
        ~Buffer();
    };

    Node * head;
    Node * min_node;
public:
    class Pointer {
    private:
        Buffer * buffer;
        Pointer (Buffer * buffer);
    public:
        friend class BinomialHeap<Key>;
        Pointer();
        ~Pointer();
    };

    BinomialHeap();
    BinomialHeap(Node * node);
    ~BinomialHeap();
    bool is_empty() const;
    Key get_min() const;
    Key extract_min();
    void merge(BinomialHeap &otherHeap);
    Pointer insert(Key key);
    void del(Pointer ptr);
    void change(Pointer ptr, Key key);
};

template <typename Key>
BinomialHeap<Key>::Node::Node(Key key, Node * parent, Node * child, Node * sibling, Node * last_child, int deg, Buffer * buffer) : key(key), parent(parent), child(child), sibling(sibling), last_child(last_child), deg(deg), buffer(buffer) {}

template <typename Key>
BinomialHeap<Key>::Node::~Node() {}

template <typename Key>
BinomialHeap<Key>::Buffer::Buffer(BinomialHeap<Key>::Node *node) : node(node) {}

template <typename Key>
BinomialHeap<Key>::Buffer::~Buffer() {}

template <typename Key>
BinomialHeap<Key>::Pointer::Pointer() : buffer(nullptr) {}

template <typename Key>
BinomialHeap<Key>::Pointer::Pointer(BinomialHeap<Key>::Buffer *buffer) : buffer(buffer) {}

template <typename Key>
BinomialHeap<Key>::Pointer::~Pointer() {}

template <typename Key>
BinomialHeap<Key>::BinomialHeap() {
    head = nullptr;
    min_node = nullptr;
}

template <typename Key>
BinomialHeap<Key>::BinomialHeap(Node * node) {
    head = node;
    min_node = node;
    while (node != nullptr) {
        if (node->key < min_node->key) {
            min_node = node;
        }
        node = node->sibling;
    }
}

template <typename Key>
BinomialHeap<Key>::~BinomialHeap() {
    if (head != nullptr)
        delete head;
    if (min_node != nullptr)
        delete min_node;
}

template <typename Key>
bool BinomialHeap<Key>::is_empty() const {
    return head == nullptr;
}

template <typename Key>
typename BinomialHeap<Key>::Pointer BinomialHeap<Key>::insert(Key key) {
    auto new_node = new BinomialHeap<Key>::Node(key);
    auto buffer = new Buffer(new_node);
    new_node->buffer = buffer;
    Pointer ptr(buffer);
    BinomialHeap<Key> new_heap(new_node);
    merge(new_heap);
    return ptr;
}

template <typename Key>
Key BinomialHeap<Key>::get_min() const {
    if (head == nullptr) {
        throw std::out_of_range("Heap is empty");
    }
    return min_node->key;
}

template <typename Key>
Key BinomialHeap<Key>::extract_min() {
    if (head == nullptr) {
        throw std::out_of_range("Heap is empty");
    }
    Key ans = min_node->key;
    Node * node = head;
    Node * prev = nullptr;
    while (node != min_node) {
        prev = node;
        node = node->sibling;
    }
    if (prev != nullptr) {
        prev->sibling = node->sibling;
    }
    else {
        head = node->sibling;
    }
    Node * was_min_node = min_node;
    min_node = head;
    Node * pos_min_node = head;
    while (pos_min_node != nullptr) {
        if (pos_min_node->key < min_node->key) {
            min_node = pos_min_node;
        }
        pos_min_node = pos_min_node->sibling;
    }
    was_min_node->deg = -1;
    node = was_min_node->child;
    while (node != nullptr) {
        node->parent = nullptr;
        node = node->sibling;
    }
    BinomialHeap<Key> new_heap(was_min_node->child);
    merge(new_heap);
    was_min_node->buffer->node = nullptr;
    return ans;
}

template <typename Key>
void BinomialHeap<Key>::merge(BinomialHeap<Key> &otherHeap) {
    if (otherHeap.is_empty()) {
        return;
    }
    if (is_empty()) {
        head = otherHeap.head;
        min_node = otherHeap.min_node;
        otherHeap.head = nullptr;
        otherHeap.min_node = nullptr;
        return;
    }
    Node * node1 = head;
    Node * node2 = otherHeap.head;
    Node * prev = nullptr;
    while (node1 != nullptr && node2 != nullptr) {
        if (node1->deg > node2->deg) {
            if (prev != nullptr) {
                prev->sibling = node2;
            }
            else {
                head = node2;
            }
            prev = node2;
            node2 = node2->sibling;
        }
        else {
            if (prev != nullptr) {
                prev->sibling = node1;
            }
            prev = node1;
            node1 = node1->sibling;
        }
    }
    while (node1 != nullptr) {
        prev->sibling = node1;
        prev = node1;
        node1 = node1->sibling;
    }
    while (node2 != nullptr) {
        prev->sibling = node2;
        prev = node2;
        node2 = node2->sibling;
    }
    otherHeap.head = nullptr;
    otherHeap.min_node = nullptr;
    Node * node = head;
    prev = nullptr;
    while (node->sibling != nullptr) {
        if (node->sibling->sibling != nullptr && node->sibling->sibling->deg == node->deg) {
            prev = node;
            node = node->sibling;
            continue;
        }
        if (node->deg == node->sibling->deg) {
            if (node->sibling->key < node->key) {
                if (prev != nullptr) {
                    prev->sibling = node->sibling;
                }
                else {
                    head = node->sibling;
                }
                Node * was_next = node->sibling;
                node->sibling = node->sibling->sibling;
                was_next->sibling = node;
                node = was_next;
            }
            node->sibling->parent = node;
            if (node->last_child != nullptr) {
                node->last_child->sibling = node->sibling;
            }
            else {
                node->child = node->sibling;
            }
            node->last_child = node->sibling;
            node->sibling = node->sibling->sibling;
            node->last_child->sibling = nullptr;
            node->deg++;
        }
        else {
            prev = node;
            node = node->sibling;
        }
    }
    min_node = head;
    Node * pos_min_node = head;
    while (pos_min_node != nullptr) {
        if (pos_min_node->key < min_node->key) {
            min_node = pos_min_node;
        }
        pos_min_node = pos_min_node->sibling;
    }
}

template <typename Key>
void BinomialHeap<Key>::del(Pointer ptr) {
    if (ptr.buffer == nullptr || ptr.buffer->node == nullptr) {
        throw std::invalid_argument("Pointer is null");
    }
    Node * node = ptr.buffer->node;
    while (node->parent != nullptr) {
        swap(node->key, node->parent->key);
        swap(node->buffer->node, node->parent->buffer->node);
        swap(node->buffer, node->parent->buffer);
        node = node->parent;
    }
    min_node = node;
    extract_min();
    ptr.buffer = nullptr;
}

template <typename Key>
void BinomialHeap<Key>::change(Pointer ptr, Key key) {
    if (ptr.buffer == nullptr || ptr.buffer->node == nullptr) {
        throw std::invalid_argument("Pointer is null");
    }
    if (key < ptr.buffer->node->key) {
        Node * node = ptr.buffer->node;
        node->key = key;
        while (node->parent != nullptr && node->key < node->parent->key) {
            swap(node->key, node->parent->key);
            swap(node->buffer->node, node->parent->buffer->node);
            swap(node->buffer, node->parent->buffer);
            node = node->parent;
        }
        if (node->parent == nullptr && (min_node == nullptr || node->key < min_node->key)) {
            min_node = node;
        }
    }
    else {
        del(ptr);
        Pointer new_ptr = insert(key);
        *ptr.buffer = *new_ptr.buffer;
        new_ptr.buffer->node->buffer = ptr.buffer;
    }
}

#endif //LABA_BINOMIALHEAP_H
