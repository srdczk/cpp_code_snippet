cmake_minimum_required(VERSION 3.13)
project(task_pool)

set(CMAKE_CXX_STANDARD 17)

add_executable(task_pool main.cpp TaskPool.h TaskPool.h TaskPool.cpp NotCopyable.h TaskPool.cpp TaskPool.h NotCopyable.h main.cpp)

target_link_libraries(task_pool pthread)