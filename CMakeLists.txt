cmake_minimum_required(VERSION 3.0)
project(Tokenizer)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")

add_library(tokenize tokenizer.cpp)

add_executable(demo demo.cpp)
target_link_libraries(demo LINK_PUBLIC tokenize)
