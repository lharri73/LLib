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
    sping

        src/sping/main.cpp
    )


set(EXECUTABLE_OUTPUT_PATH "bin")
