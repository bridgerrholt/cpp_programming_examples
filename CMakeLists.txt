cmake_minimum_required(VERSION 3.6)
project(cpp_programming_examples)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

add_executable(example_01 src/example_01.cpp)
add_executable(example_02 src/example_02.cpp)
add_executable(file_structure src/in_progress/file_structure.cpp)