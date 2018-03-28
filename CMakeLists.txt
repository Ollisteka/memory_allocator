cmake_minimum_required(VERSION 3.9)
project(malloc)

set(CMAKE_CXX_STANDARD 11)

add_executable(malloc main.cpp my_allocator.cpp my_allocator.h)