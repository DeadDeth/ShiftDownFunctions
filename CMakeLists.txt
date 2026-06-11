cmake_minimum_required(VERSION 3.14)
project(ShiftDownFunctions)

set(CMAKE_CXX_STANDARD 23)

add_library(ShiftDownFunctions INTERFACE)
target_include_directories(ShiftDownFunctions INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

set_source_files_properties(ShiftDownFunctions.hpp PROPERTIES LANGUAGE CXX)

add_library(CLion_Index_Fix OBJECT EXCLUDE_FROM_ALL ShiftDownFunctions.hpp)