cmake_minimum_required(VERSION 3.20)

project(aoc-2024)

include(cmake/CPM.cmake)

CPMAddPackage("gh:fmtlib/fmt#10.2.1")
CPMAddPackage("gh:ericniebler/range-v3#0.12.0")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(day1 day1.cpp)
add_executable(day2 day2.cpp)

target_link_libraries(day1 PRIVATE range-v3)
target_link_libraries(day1 PRIVATE fmt::fmt)
target_link_libraries(day2 PRIVATE range-v3)
target_link_libraries(day2 PRIVATE fmt::fmt)




