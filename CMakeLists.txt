cmake_minimum_required(VERSION 3.24)
project(.)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED on)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED on)

add_compile_options(-Wall -g -O3 -fPIC)

set(BUILD_SHARED_LIBS OFF)

# Terminos library files
file(GLOB_RECURSE INCLUDE "./src/*.hpp" "./src/*.h")
file(GLOB_RECURSE SOURCES "./src/*.cpp" "./src/*.c")

# Make Terminos a shared library
add_library(Terminos SHARED ${SOURCES} ${INCLUDE} ${STB_SOURCES})
target_include_directories(Terminos PUBLIC "./src" "./submodules/stb")

# Link all libraries together
target_link_libraries(Terminos)
