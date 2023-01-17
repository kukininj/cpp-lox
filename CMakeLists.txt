cmake_minimum_required(VERSION 3.24)
project(cpp_lox CXX)

set(CMAKE_CXX_STANDARD 20)

add_definitions( "-std=c++20 -Wall -m64" )
# include_directories(include)

add_executable(cpp_lox src/main.cpp src/scanner.cpp src/token.cpp src/position.cpp src/loxValue.cpp src/expression.cpp)
