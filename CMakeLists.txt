cmake_minimum_required(VERSION 3.23)
project(Algorithms)

set(CMAKE_CXX_STANDARD 14)

add_executable(Algorithms
        sorts/quick_select.cpp
        sorts/quick_sort.cpp
        sorts/radix_byte_sort.cpp
        structs/binary_heap.cpp
        structs/binary_tree.cpp
        structs/fenwick_tree.cpp
        structs/sparse_table.cpp
        structs/avl_tree.cpp
        structs/splay_tree.cpp
        structs/multiset.cpp
        .clang-format)
