cmake_minimum_required(VERSION 3.10)

# set the project name
project(magic)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# debug or release, use gdb to debug
set(CMAKE_BUILD_TYPE "Release")
set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g2 -ggdb")
set(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")

# add the executable
add_executable(magic ${src_files} main.cpp magic_generator.cpp ${pikafish_nnue_adapter_files})