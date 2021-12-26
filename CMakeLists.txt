cmake_minimum_required (VERSION 2.9.0)
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
    rmdl
        src/rmdl/main.cpp
    )

add_executable(
    sping
        src/sping/main.cpp
    )

add_executable(
    con
        src/con/main.cpp
        src/con/Graph.cpp
    )

install(FILES ./src/con/units.txt
    DESTINATION ~/.local/config/con/
    )

add_library(
    MOA
        src/MOA/MOA.c
    )

add_library(
    dlLib
        src/dlLib/dlLib.cpp
    )

######################
target_link_libraries(
    mvdl
        dlLib
    )

target_link_libraries(
    rmdl
        dlLib
    )

find_package(CUDA)
if(DEFINED CUDA_TOOLKIT_ROOT_DIR)
    enable_language(CUDA)
    add_library(
        CUDSafe
            src/libCUDSafe/util.cpp
    )

    target_include_directories(CUDSafe PUBLIC ${CUDA_INCLUDE_DIRS})
    target_link_libraries(CUDSafe PUBLIC
        ${CUDA_LIBRARIES}
    )
endif()

set(EXECUTABLE_OUTPUT_PATH "bin")
set(LIBRARY_OUTPUT_PATH "lib")
set(CMAKE_BUILD_TYPE Debug)
add_compile_options(-Wall -Wextra -pedantic -Werror -glldb)
