cmake_minimum_required(VERSION 3.15)
project(Ex3 C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(Ex3
        ProductExample.c
        RBTree.c
        RBTree.h
        Structs.c
        Structs.h)
