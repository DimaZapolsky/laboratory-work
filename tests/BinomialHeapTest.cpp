//
// Created by Dima Zapolsky on 19/12/2018.
//

#include "../src/BinomialHeap.h"
#include <gtest/gtest.h>
#include <gmock/gmock.h>

TEST(BinomialHeap, sizeTestSmall) {
    BinomialHeap<int> heap;
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

TEST(BinomialHeap, sizeTestBig) {
    BinomialHeap<int> heap;
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
        if (i % 100 == 0) {
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

TEST(BinomialHeap, pointerTestSmall) {
    BinomialHeap<int> heap;
    BinomialHeap<int>::Pointer ptr[5];
    for (int i = 0; i < 5; ++i) {
        ptr[i] = heap.insert(i);
    }
    EXPECT_EQ(heap.get_min(), 0);
    EXPECT_NO_THROW(heap.change(ptr[0], -1000));
    EXPECT_EQ(heap.get_min(), -1000);
    EXPECT_NO_THROW(heap.change(ptr[4], -2000));
    EXPECT_EQ(heap.get_min(), -2000);
    EXPECT_NO_THROW(heap.del(ptr[0]));
    EXPECT_ANY_THROW(heap.del(ptr[0]));
    EXPECT_EQ(heap.get_min(), -2000);
    EXPECT_NO_THROW(heap.del(ptr[4]));
    EXPECT_ANY_THROW(heap.del(ptr[4]));
    EXPECT_EQ(heap.get_min(), 1);
    EXPECT_NO_THROW(heap.extract_min());
    EXPECT_EQ(heap.get_min(), 2);
    EXPECT_NO_THROW(heap.extract_min());
    EXPECT_EQ(heap.get_min(), 3);
    EXPECT_NO_THROW(heap.extract_min());
    EXPECT_TRUE(heap.is_empty());
    EXPECT_ANY_THROW(heap.get_min());
    EXPECT_ANY_THROW(heap.extract_min());
    EXPECT_ANY_THROW(heap.del(ptr[0]));
    EXPECT_ANY_THROW(heap.del(ptr[1]));
    EXPECT_ANY_THROW(heap.del(ptr[2]));
    EXPECT_ANY_THROW(heap.del(ptr[3]));
    EXPECT_ANY_THROW(heap.del(ptr[4]));
    EXPECT_ANY_THROW(heap.change(ptr[0], 1));
    EXPECT_ANY_THROW(heap.change(ptr[1], 2));
    EXPECT_ANY_THROW(heap.change(ptr[2], 3));
    EXPECT_ANY_THROW(heap.change(ptr[3], 4));
    EXPECT_ANY_THROW(heap.change(ptr[4], 5));
}

TEST(BinomialHeap, pointerTestBig) {
    BinomialHeap<int> heap;
    int sz = 1000;
    int arr[sz];
    BinomialHeap<int>::Pointer ptr[sz];
    for (int i = 0; i < sz; ++i) {
        arr[i] = i;
        ptr[i] = heap.insert(arr[i]);
    }
    for (int i = 0; i < 10000; ++i) {
        int pos = rand() % sz;
        int val = rand() % 2000000 - 1000000;
        EXPECT_NO_THROW(heap.change(ptr[pos], val));
        arr[pos] = val;
        int mn = 1000000000;
        for (int j = 0; j < 1000; ++j) {
            mn = (mn > arr[j] ? arr[j] : mn);
        }
        EXPECT_EQ(mn, heap.get_min());
    }
    for (int i = 0; i < 10000; ++i) {
        if (heap.is_empty())
            break;
        if (rand() % 5 != 0) {
            int pos = rand() % sz;
            if (arr[pos] == 1000000000)
                continue;
            int val = rand() % 2000000 - 1000000;
            EXPECT_NO_THROW(heap.change(ptr[pos], val));
            arr[pos] = val;
        }
        else {
            int pos = rand() % sz;
            if (arr[pos] == 1000000000)
                continue;
            arr[pos] = 1000000000;
            EXPECT_NO_THROW(heap.del(ptr[pos]));
            EXPECT_ANY_THROW(heap.del(ptr[pos]));
        }
        int mn = 1000000000;
        for (int j = 0; j < sz; ++j) {
            mn = (mn > arr[j] ? arr[j] : mn);
        }
        EXPECT_EQ(mn, heap.get_min());
    }
}

TEST(BinomialHeap, timeTest) {
    BinomialHeap<int> heap;
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
