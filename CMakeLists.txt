cmake_minimum_required (VERSION 2.8.11)
project(LandonLib)

include_directories(include)

add_executable(
    python_to_matlab
        src/python_to_matlab/main.cpp)

add_executable(
    calc
        src/calc/solve.cpp
        src/calc/main.cpp
    )

add_executable(
    mvdl
        src/mvdl/main.cpp
    )

add_executable(
    sping
        src/sping/main.cpp
    )

add_library(
    MOA
        src/MOA/MOA.c
    )

set(EXECUTABLE_OUTPUT_PATH "bin")
set(LIBRARY_OUTPUT_PATH "lib")
