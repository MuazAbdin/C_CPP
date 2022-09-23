cmake_minimum_required(VERSION 3.15)
project(ex4)

set(CMAKE_CXX_STANDARD 17)

add_executable(ex4 main.cpp Matrix.h Matrix.cpp Activation.h Activation.cpp Dense.h Dense.cpp
        MlpNetwork.h MlpNetwork.cpp)