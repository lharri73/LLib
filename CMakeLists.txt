cmake_minimum_required (VERSION 2.8.11)
project(LandonLib)

include_directories(include)

add_executable(
    python_to_matlab
        src/python_to_matlab/main.cpp)

add_executable(
    calc
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

find_package(CUDA)
if(DEFINED CUDA_TOOLKIT_ROOT_DIR)
    enable_language(CUDA)
    add_library(
        libCUDSafe
            src/libCUDSafe/util.cpp
    )
endif()

set(EXECUTABLE_OUTPUT_PATH "bin")
set(LIBRARY_OUTPUT_PATH "lib")
set(CMAKE_BUILD_TYPE Debug)
