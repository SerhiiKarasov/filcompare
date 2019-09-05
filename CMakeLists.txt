project(filcompare LANGUAGES CXX)

cmake_minimum_required(VERSION 3.13)

set(CMAKE_CXX_STANDARD 14)

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    set(CMAKE_CXX_FLAGS "-Wall -Wextra -fprofile-arcs -ftest-coverage")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    set(CMAKE_CXX_FLAGS "-Wall -Wextra -Weverything -Wno-c++98-compat -Wno-c++98-compat-pedantic")
endif()

add_subdirectory(src)
add_subdirectory(tests)
add_subdirectory(submodules)