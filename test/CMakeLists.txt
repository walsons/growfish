cmake_minimum_required(VERSION 3.10)

# set the project name
project(growfish_test)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# debug or release, use gdb to debug
set(CMAKE_BUILD_TYPE "Debug")
set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g2 -ggdb")
set(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")

# add the executable
add_executable(growfish_test ${src_files} main.cpp magic_validator.cpp ${pikafish_nnue_adapter_files})

# add_definitions(-DUSE_SSE2)

include(CheckCXXCompilerFlag)
include(CheckIncludeFile)

# check if the compiler supports AVX2 (would crash)
# check_cxx_compiler_flag("-mavx2" COMPILER_SUPPORTS_AVX2)
# if(COMPILER_SUPPORTS_AVX2)
#     target_compile_options(growfish_test PRIVATE -mavx2)
#     target_compile_definitions(growfish_test PRIVATE USE_AVX2)
#     target_include_directories(growfish_test PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include/immintrin.h)
# endif()

# check if the compiler supports SSE4.1
check_cxx_compiler_flag("-msse4.1" COMPILER_SUPPORTS_SSE41)
if(COMPILER_SUPPORTS_SSE41)
    target_compile_options(growfish_test PRIVATE -msse4.1)
    target_compile_definitions(growfish_test PRIVATE USE_SSE41)
    target_include_directories(growfish_test PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include/smmintrin.h)
endif()

# check if the compiler supports SSSE3
check_cxx_compiler_flag("-mssse3" COMPILER_SUPPORTS_SSSE3)
if(COMPILER_SUPPORTS_SSSE3)
    target_compile_options(growfish_test PRIVATE -mssse3)
    target_compile_definitions(growfish_test PRIVATE USE_SSSE3)
    target_include_directories(growfish_test PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include/tmmintrin.h)
endif()

# check if the compiler supports SSE2
check_cxx_compiler_flag("-msse2" COMPILER_SUPPORTS_SSE2)
if(COMPILER_SUPPORTS_SSE2)
    target_compile_options(growfish_test PRIVATE -msse2)
    target_compile_definitions(growfish_test PRIVATE USE_SSE2)
    target_include_directories(growfish_test PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include/emmintrin.h)
endif()

# check if the compiler supports MMX
check_cxx_compiler_flag("-mmmx" COMPILER_SUPPORTS_MMX)
if(COMPILER_SUPPORTS_MMX)
    target_compile_options(growfish_test PRIVATE -mmmx)
    target_compile_definitions(growfish_test PRIVATE USE_MMX)
    target_include_directories(growfish_test PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include/mmintrin.h)
endif()

# check if the compiler supports ARM NEON
if(CMAKE_SYSTEM_PROCESSOR MATCHES "arm" OR CMAKE_SYSTEM_PROCESSOR MATCHES "aarch64")
    target_compile_definitions(growfish_test PRIVATE USE_NEON)
    target_include_directories(growfish_test PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include/arm_neon.h)
endif()

message(STATUS "Compiler flags: ${CMAKE_CXX_FLAGS}")
message(STATUS "Definitions: ${CMAKE_CXX_FLAGS}")