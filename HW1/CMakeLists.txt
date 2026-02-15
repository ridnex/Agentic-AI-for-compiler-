cmake_minimum_required(VERSION 3.14)
project(rust_lexer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${CMAKE_SOURCE_DIR}/include)

# Static library
add_library(lexer_lib STATIC src/lexer.cpp)
target_include_directories(lexer_lib PUBLIC ${CMAKE_SOURCE_DIR}/include)

# CLI executable
add_executable(rustc src/main.cpp)
target_link_libraries(rustc PRIVATE lexer_lib)

# Tests
enable_testing()
add_subdirectory(tests)
