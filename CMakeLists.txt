cmake_minimum_required(VERSION 3.30)
project(expression_finder)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static -static-libgcc -static-libstdc++")

add_executable(expression_finder expression_finder.cpp)
