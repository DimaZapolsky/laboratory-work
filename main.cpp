#include <iostream>
#include "src/Heap.h"
#include "src/BinomialHeap.h"
#include "src/FibonacciHeap.h"
#include "src/Dynamic_array.h"
#include <gtest/gtest.h>
#include <gmock/gmock.h>

int main(int argc, char* argv[]) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
