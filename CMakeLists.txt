cmake_minimum_required(VERSION 3.12)
project(hw03_binarytree)

set(CMAKE_CXX_STANDARD 14)
set(SOURCE_FILES bst_test.cpp BinaryTree.hpp catch.hpp)
add_executable(hw4 ${SOURCE_FILES})