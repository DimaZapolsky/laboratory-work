//
// Created by Dima Zapolsky on 03/12/2018.
//

#ifndef LABA_DYNAMIC_ARRAY_H
#define LABA_DYNAMIC_ARRAY_H

#include <stdio.h>
#include <iostream>

template <typename Key>
class dynamic_array {
private:
    int last;
    int sz;
    Key * arr;
public:
    dynamic_array(int sz = 0);
    ~dynamic_array();
    void push_back(Key key);
    void pop_back();
    Key& operator [] (int idx) const;
    int size() const;
};

template<typename Key>
dynamic_array<Key>::dynamic_array(int sz): sz(sz), last(sz) {
    if (sz < 0) {
        throw std::invalid_argument("Size can't be less than 0");
    }
    Key * new_arr = new Key[sz];
    arr = new_arr;
}

template <typename Key>
dynamic_array<Key>::~dynamic_array() {
    delete [] arr;
}

template<typename Key>
void dynamic_array<Key>::push_back(Key key) {
    if (last == sz) {
        int new_sz = (sz == 0 ? 1 : 2 * sz);
        Key * new_arr = new Key[new_sz];
        for (int i = 0; i < sz; ++i)
            new_arr[i] = arr[i];
        delete [] arr;
        arr = new_arr;
        sz = new_sz;
    }
    arr[last++] = key;
}

template<typename Key>
void dynamic_array<Key>::pop_back() {
    if (last == 0) {
        throw std::out_of_range("Array is empty");
    }
    last--;
    if (last * 2 < sz) {
        int new_sz = sz / 2;
        Key * new_arr = new Key[new_sz];
        for (int i = 0; i < last; ++i)
            new_arr[i] = arr[i];
        delete [] arr;
        arr = new_arr;
        sz = new_sz;
    }
}

template<typename Key>
Key& dynamic_array<Key>::operator [] (int idx) const {
    if (idx < 0 || idx >= last) {
        throw std::out_of_range("Index should between zero and size of array");
    }
    return arr[idx];
}

template<typename Key>
int dynamic_array<Key>::size() const {
    return last;
}

#endif //LABA_DYNAMIC_ARRAY_H
