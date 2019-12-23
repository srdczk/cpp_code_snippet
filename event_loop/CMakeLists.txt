cmake_minimum_required(VERSION 3.10)
project(event_loop)

set(CMAKE_CXX_STANDARD 17)

find_package(Threads REQUIRED)

add_executable(event_loop main.cpp EventLoop.cpp EventLoop.h Server.h)
target_link_libraries(event_loop Threads::Threads)