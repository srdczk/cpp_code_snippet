cmake_minimum_required(VERSION 3.10)
project(muduo)

set(CMAKE_CXX_STANDARD 17)

FILE(GLOB SOURCE_FILE "*.cpp")
FILE(GLOB HEADER_FILE "*.h")

add_executable(muduo ${SOURCE_FILE})

target_link_libraries(muduo muduo_net muduo_base pthread)