cmake_minimum_required(VERSION 3.12)
project(laba)

set(CMAKE_CXX_STANDARD 14)
add_subdirectory(lib/googletest-release-1.8.1)
include_directories(lib/googletest-release-1.8.1/googletest/include)
include_directories(lib/googletest-release-1.8.1/googlemock/include)


add_executable(laba main.cpp src/Heap.h src/Dynamic_array.h src/BinomialHeap.h src/FibonacciHeap.h tests/Dynamic_array_test.cpp tests/HeapTest.cpp tests/BinomialHeapTest.cpp src/swap.h tests/FibonacciHeapTest.cpp)
target_link_libraries(laba gtest gtest_main)
