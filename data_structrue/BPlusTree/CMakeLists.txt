cmake_minimum_required(VERSION 3.10)
project(BPlusTree)

set(CMAKE_CXX_STANDARD 17)

add_executable(BPlusTree BPlusTree.cpp BPlusTree.h PreDefined.h main.cpp)