cmake_minimum_required(VERSION 3.10)
project(rustlex)

set(CMAKE_CXX_STANDARD 17)

add_executable(rustlex src/main.cpp src/lexer.cpp)
