cmake_minimum_required(VERSION 3.14)
project(ShiftDownFunctions)

set(CMAKE_CXX_STANDARD 23)

add_library(ShiftDownFunctions INTERFACE)
target_include_directories(ShiftDownFunctions INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
add_custom_target(CLion_Index_Fix SOURCES ShiftDownFunctions.hpp)