cmake_minimum_required(VERSION 3.29)
project(example)

set(CMAKE_CXX_STANDARD 20)

add_executable(example src/main.cpp
        src/Game.cpp
        src/Game.h
        src/AI.cpp
        src/AI.h
)
