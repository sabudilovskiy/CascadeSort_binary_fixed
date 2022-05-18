cmake_minimum_required(VERSION 3.20)
project(CascadeSort_binary_fixed)

set(CMAKE_CXX_STANDARD 23)
include_directories(includes)
add_executable(CascadeSort_binary_fixed main.cpp
        includes/CascadeSorter.h
        includes/SeriesReader.h
        src/CascadeSorter.cpp
        src/SeriesReader.cpp
        includes/Student.h
        src/Student.cpp includes/Support.h src/Support.cpp)
