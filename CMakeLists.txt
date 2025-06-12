cmake_minimum_required(VERSION 3.10)
project(mini_java)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(compiler
        parser.cpp
        principal.cpp
        scanner.cpp
        stentry.cpp
        symboltable.cpp)
