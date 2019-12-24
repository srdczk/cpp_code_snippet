cmake_minimum_required(VERSION 3.10)
project(ChatServer)

set(CMAKE_CXX_STANDARD 17)

add_executable(ChatServer main.cpp ChatServer.cpp ChatServer.h NotCopyable.h)

target_link_libraries(ChatServer muduo_net muduo_base pthread)