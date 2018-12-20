//
// Created by Dima Zapolsky on 03/12/2018.
//

#ifndef LABA_HEAP_H
#define LABA_HEAP_H

#include <stdio.h>
#include <iostream>
#include "Dynamic_array.h"
#include "swap.h"

template <typename Key>
class Heap {
public:
    class Pointer {
    public:
        friend class Heap<Key>;

        Pointer();
    private:
        int * idx;

        Pointer(int idx);
    };
private:
    int deg;
    dynamic_array<Key> arr;
    dynamic_array<Pointer> ptrs;
    bool optimized;

    void sift_down(int idx);
    void sift_up(int idx);
    void swap_nodes(int i, int j);
public:
    Heap();
    ~Heap();
    bool is_empty() const;
    Key get_min() const;
    Key extract_min();
    Pointer insert(Key key);
    void del(Pointer ptr);
    void change(Pointer ptr, Key key);
    template <class Iterator>
    friend Heap<Key> IPriorityQueue(Iterator begin, Iterator end);
    void optimize(size_t insertCount, size_t extractCount);
};

template <typename Key>
Heap<Key>::Pointer::Pointer() : idx(nullptr) {}

template <typename Key>
Heap<Key>::Pointer::Pointer(int idx) : idx(new int(idx)) {}

template <typename Key>
Heap<Key>::Heap() : deg(2), optimized(false), arr(1), ptrs(1) {}

template <typename Key>
Heap<Key>::~Heap() {}

template <typename Key>
void Heap<Key>::swap_nodes(int i, int j) {
    swap(arr[i], arr[j]);
    swap(*ptrs[i].idx, *ptrs[j].idx);
    swap(ptrs[i], ptrs[j]);
}

template <typename Key>
void Heap<Key>::sift_down(int idx) {
    int min_key = idx;
    for (int i = idx * deg; i < idx * deg + deg && i < arr.size(); ++i) {
        if (arr[min_key] > arr[i]) {
            min_key = i;
        }
    }
    if (min_key != idx) {
        swap_nodes(idx, min_key);
        sift_down(min_key);
    }
}

template <typename Key>
void Heap<Key>::sift_up(int idx) {
    while (idx / deg > 0 && arr[idx] < arr[idx / deg]) {
        swap_nodes(idx, idx / deg);
        idx = idx / deg;
    }
}

template <typename Key>
bool Heap<Key>::is_empty() const {
    return arr.size() == 1;
}

template <typename Key>
typename Heap<Key>::Pointer Heap<Key>::insert(Key key) {
    arr.push_back(key);
    ptrs.push_back(Pointer(ptrs.size()));
    sift_up(arr.size() - 1);
    return ptrs[ptrs.size() - 1];
}

template <typename Key>
Key Heap<Key>::get_min() const {
    if (is_empty()) {
        throw std::out_of_range("Heap is empty");
    }
    return arr[1];
}

template <typename Key>
Key Heap<Key>::extract_min() {
    if (is_empty()) {
        throw std::out_of_range("Heap is empty");
    }
    int min_key = arr[1];
    arr[1] = arr[arr.size() - 1];
    arr.pop_back();
    ptrs[ptrs.size() - 1].idx = nullptr;
    ptrs.pop_back();
    sift_down(0);
    return min_key;
}

template <typename Key>
void Heap<Key>::del(Heap<Key>::Pointer ptr) {
    if (optimized) {
        throw std::logic_error("Can't execute this function, because heap is optimized");
    }
    if (*ptr.idx == -1) {
        throw std::invalid_argument("Pointer is null");
    }
    int idx = *ptr.idx;
    swap_nodes(idx, arr.size() - 1);
    arr.pop_back();
    *ptrs[ptrs.size() - 1].idx = -1;
    ptrs.pop_back();
    if (idx != arr.size()) {
        sift_down(idx);
        sift_up(idx);
    }
}

template <typename Key>
void Heap<Key>::change(Heap<Key>::Pointer ptr, Key key) {
    if (optimized) {
        throw std::logic_error("Can't execute this function, because heap is optimized");
    }
    if (ptr.idx == nullptr) {
        throw std::invalid_argument("Pointer is null");
    }
    int idx = *ptr.idx;
    arr[idx] = key;
    sift_down(idx);
    sift_up(idx);
}

template <typename Key, class Iterator>
Heap<Key> IPriorityQueue(Iterator begin, Iterator end) {
    Heap<Key> heap;
    while (begin != end) {
        heap.arr.push_back(*begin);
        heap.ptrs.push_back(Pointer(heap.ptrs.size()));
        ++begin;
    }
    int n = heap.arr.size();
    for (int i = n / heap.deg; i > 0; --i) {
        heap.sift_down(i);
    }
    return heap;
}

template <typename Key>
void Heap<Key>::optimize(size_t insertCount, size_t extractCount) {
    if (!is_empty()) {
        throw std::logic_error("Can't optimize heap, because it's not empty");
    }
    if (extractCount == 0) {
        deg = (int)insertCount;
    }
    else {
        deg = (int)insertCount / (int)extractCount;
    }
    if (deg < 2)
        deg = 2;
    optimized = true;
}

#endif //LABA_HEAP_H
