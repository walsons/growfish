cmake_minimum_required(VERSION 3.10)

# set compiler
set(CMAKE_C_COMPILER "gcc")
set(CMAKE_CXX_COMPILER "g++")

# set the project name
project(growfish)

file(GLOB src_files ${CMAKE_SOURCE_DIR}/src/*.cpp)
list(REMOVE_ITEM src_files ${CMAKE_SOURCE_DIR}/src/main.cpp)

file(GLOB pikafish_nnue_adapter_files ${CMAKE_SOURCE_DIR}/src/pikafish_nnue_adapter/*.cpp)
list(APPEND pikafish_nnue_adapter_files ${CMAKE_SOURCE_DIR}/src/pikafish_nnue_adapter/nnue/evaluate_nnue.cpp)
list(APPEND pikafish_nnue_adapter_files ${CMAKE_SOURCE_DIR}/src/pikafish_nnue_adapter/nnue/features/half_ka_v2_xq.cpp)

add_subdirectory(${CMAKE_SOURCE_DIR}/src)
add_subdirectory(${CMAKE_SOURCE_DIR}/test)
add_subdirectory(${CMAKE_SOURCE_DIR}/magic)