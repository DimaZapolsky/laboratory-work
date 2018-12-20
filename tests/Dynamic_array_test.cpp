//
// Created by Dima Zapolsky on 10/12/2018.
//

#include "../src/Dynamic_array.h"
#include <gtest/gtest.h>
#include <gmock/gmock.h>


TEST(dynamic_array, push_pop_back_test) {
    dynamic_array<int> arr;
    EXPECT_EQ(arr.size(), 0);
    EXPECT_ANY_THROW(arr[2]);
    EXPECT_ANY_THROW(arr[-1]);
    EXPECT_ANY_THROW(arr.pop_back());
    arr.push_back(3);
    EXPECT_EQ(arr[0], 3);
    EXPECT_EQ(arr.size(), 1);
    EXPECT_NO_THROW(arr.pop_back());
    EXPECT_ANY_THROW(arr[2]);
    EXPECT_ANY_THROW(arr[-1]);
    EXPECT_ANY_THROW(arr.pop_back());
    arr.push_back(1);
    arr.push_back(2);
    arr.push_back(3);
    arr.push_back(4);
    EXPECT_EQ(arr.size(), 4);
    EXPECT_EQ(arr[0], 1);
    EXPECT_EQ(arr[1], 2);
    EXPECT_EQ(arr[2], 3);
    EXPECT_EQ(arr[3], 4);
    EXPECT_ANY_THROW(arr[10]);
    EXPECT_ANY_THROW(arr[-1]);
    EXPECT_NO_THROW(arr.pop_back());
    EXPECT_NO_THROW(arr.pop_back());
    EXPECT_NO_THROW(arr.pop_back());
    EXPECT_NO_THROW(arr.pop_back());
    EXPECT_EQ(arr.size(), 0);
}

TEST(dynamic_array, size_test) {
    dynamic_array<int> arr(10);
    EXPECT_EQ(arr.size(), 10);
    EXPECT_ANY_THROW(arr[10]);
    EXPECT_ANY_THROW(arr[-1]);
    EXPECT_NO_THROW(arr[0]);
    EXPECT_NO_THROW(arr[5]);
    EXPECT_NO_THROW(arr[9]);
    arr.push_back(10);
    EXPECT_EQ(arr[10], 10);
    EXPECT_EQ(arr.size(), 11);
    EXPECT_NO_THROW(arr.pop_back());
    EXPECT_NO_THROW(arr.pop_back());
    EXPECT_ANY_THROW(arr[9]);
    EXPECT_NO_THROW(arr[5] = 5);
    EXPECT_EQ(arr[5], 5);
}


TEST(dynamic_array, mem_and_time_test) {
    int t = clock();
    dynamic_array<int> arr;
    for (int i = 1; i < 100; ++i) {
        EXPECT_EQ(arr.size(), 0);
        for (int j = 0; j < 1000000; ++j) {
            arr.push_back(1);
        }
        for (int j = 0; j < 1000000; ++j) {
            EXPECT_NO_THROW(arr.pop_back());
        }
        EXPECT_ANY_THROW(arr[-1]);
        EXPECT_ANY_THROW(arr[0]);
    }
    EXPECT_TRUE(clock() - t <= 10 * 1000000);
}


