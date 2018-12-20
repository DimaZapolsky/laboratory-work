//
// Created by Dima Zapolsky on 19/12/2018.
//

#include "../src/FibonacciHeap.h"
#include <gtest/gtest.h>
#include <gmock/gmock.h>

TEST(FibonacciHeap, sizeTestSmall) {
    FibonacciHeap<int> heap;
    EXPECT_TRUE(heap.is_empty());
    EXPECT_ANY_THROW(heap.get_min());
    EXPECT_ANY_THROW(heap.extract_min());
    EXPECT_NO_THROW(heap.insert(1));
    EXPECT_NO_THROW(heap.get_min());
    EXPECT_EQ(heap.get_min(), 1);
    EXPECT_FALSE(heap.is_empty());
    EXPECT_NO_THROW(heap.extract_min());
    EXPECT_TRUE(heap.is_empty());
    EXPECT_ANY_THROW(heap.get_min());
    EXPECT_ANY_THROW(heap.extract_min());
}

TEST(FibonacciHeap, sizeTestBig) {
    FibonacciHeap<int> heap;
    EXPECT_TRUE(heap.is_empty());
    EXPECT_ANY_THROW(heap.get_min());
    EXPECT_ANY_THROW(heap.extract_min());
    int arr[1000000];
    int last = 0;
    for (int i = 0; i < 1000000; ++i) {
        if (rand() % 2 || last == 0) {
            int a = rand();
            EXPECT_NO_THROW(heap.insert(a));
            arr[last++] = a;
        }
        else {
            EXPECT_NO_THROW(heap.extract_min());
            last--;
        }
        if (i % 100) {
            EXPECT_EQ((last == 0), heap.is_empty());
            if (last == 0) {
                EXPECT_ANY_THROW(heap.get_min());
                EXPECT_ANY_THROW(heap.extract_min());
            }
        }
    }
    while (last-- > 0) {
        heap.extract_min();
    }
    EXPECT_TRUE(heap.is_empty());
    EXPECT_ANY_THROW(heap.get_min());
    EXPECT_ANY_THROW(heap.extract_min());
}

TEST(FibonacciHeap, pointerTestSmall) {
    FibonacciHeap<int> heap;
    FibonacciHeap<int>::Pointer ptr[5];
    for (int i = 0; i < 5; ++i) {
        ptr[i] = heap.insert(i);
    }
    EXPECT_EQ(heap.get_min(), 0);
    EXPECT_NO_THROW(heap.decrease(ptr[0], -1000));
    EXPECT_EQ(heap.get_min(), -1000);
    EXPECT_NO_THROW(heap.decrease(ptr[4], -2000));
    EXPECT_EQ(heap.get_min(), -2000);
    EXPECT_EQ(heap.get_min(), -2000);
    EXPECT_NO_THROW(heap.extract_min());
    EXPECT_NO_THROW(heap.extract_min());
    EXPECT_EQ(heap.get_min(), 1);
    EXPECT_NO_THROW(heap.extract_min());
    EXPECT_EQ(heap.get_min(), 2);
    EXPECT_NO_THROW(heap.extract_min());
    EXPECT_EQ(heap.get_min(), 3);
    EXPECT_NO_THROW(heap.extract_min());
    EXPECT_TRUE(heap.is_empty());
    EXPECT_ANY_THROW(heap.get_min());
    EXPECT_ANY_THROW(heap.extract_min());
    EXPECT_ANY_THROW(heap.decrease(ptr[0], 1));
    EXPECT_ANY_THROW(heap.decrease(ptr[1], 2));
    EXPECT_ANY_THROW(heap.decrease(ptr[2], 3));
    EXPECT_ANY_THROW(heap.decrease(ptr[3], 4));
    EXPECT_ANY_THROW(heap.decrease(ptr[4], 5));
}

TEST(FibonacciHeap, pointerTestBig) {
    FibonacciHeap<int> heap;
    int sz = 1000;
    int arr[sz];
    FibonacciHeap<int>::Pointer ptr[sz];
    for (int i = 0; i < sz; ++i) {
        arr[i] = i;
        ptr[i] = heap.insert(arr[i]);
    }
    for (int i = 0; i < 100000; ++i) {
        int pos = rand() % sz;
        int val = rand() % 2000000;
        EXPECT_NO_THROW(heap.decrease(ptr[pos], arr[pos] - val));
        arr[pos] -= val;
        int mn = 1000000000;
        for (int j = 0; j < 1000; ++j) {
            mn = (mn > arr[j] ? arr[j] : mn);
        }
        EXPECT_EQ(mn, heap.get_min());
    }
}

TEST(FibonacciHeap, timeTest) {
    FibonacciHeap<int> heap;
    int t = clock();
    for (int i = 0; i < 1000000; ++i) {
        if (rand() % 3) {
            heap.insert(rand());
        }
        else if (!heap.is_empty()) {
            heap.extract_min();
        }
    }
    EXPECT_TRUE(clock() - t <= 1000000);
}
