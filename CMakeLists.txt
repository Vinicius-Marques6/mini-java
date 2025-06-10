cmake_minimum_required(VERSION 3.31)
project(mini_java)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(mini_java
        parser.cpp
        parser.h
        principal.cpp
        scanner.cpp
        scanner.h
        stentry.cpp
        stentry.h
        symboltable.cpp
        symboltable.h
        token.h)
