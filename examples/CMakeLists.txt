cmake_minimum_required(VERSION 3.24)
project(.)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED on)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED on)

add_compile_options(-Wall -g -O3)

add_subdirectory("../" "./Terminos")

# Compile each demo
file(GLOB EXAMPLES "./*.cpp")
foreach(example_path ${EXAMPLES})
    get_filename_component(example ${example_path} NAME_WE)
    add_executable(${example} ${example_path})
    target_link_libraries(${example} Terminos)
endforeach()