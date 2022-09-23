cmake_minimum_required(VERSION 3.15)
project(Ex4)

set(CMAKE_CXX_STANDARD 17)

add_executable(Ex4 main.cpp
               Matrix.h Matrix.cpp
               Activation.h Activation.cpp
               MlpNetwork.h MlpNetwork.cpp
               Dense.h Dense.cpp
               Digit.h)