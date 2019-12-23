cmake_minimum_required(VERSION 3.10)
project(muduo)

set(CMAKE_CXX_STANDARD 17)

set(SOURCE_FILE main.cpp TimeServer.cpp TimeServer.h)
set(INC_DIR /home/chengzekun/build/release-install-cpp11/include)
set(LINK_DIR /home/chengzekun/build/release-install-cpp11/lib)

find_package(Threads REQUIRED)

include_directories(${INC_DIR})
link_directories(${LINK_DIR})
link_libraries(muduo_base muduo_net)

add_executable(muduo ${SOURCE_FILE})
target_link_libraries(muduo muduo_base muduo_net Threads::Threads)