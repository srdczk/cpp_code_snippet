cmake_minimum_required(VERSION 3.10)
project(ChatClient)

set(CMAKE_CXX_STANDARD 17)

add_executable(ChatClient main.cpp ChatClient.cpp ChatClient.h)

target_link_libraries(ChatClient muduo_net muduo_base pthread)