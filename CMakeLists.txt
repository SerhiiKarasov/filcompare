project(filcompare LANGUAGES CXX)
cmake_minimum_required(VERSION 3.10)

set(GCC_COVERAGE_COMPILE_FLAGS "-g -O0 -coverage -fprofile-arcs -ftest-coverage")
set(GCC_COVERAGE_LINK_FLAGS "-coverage -lgcov")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${GCC_COVERAGE_LINK_FLAGS}")

# Link this 'library' to set the c++ standard / compile-time options requested
add_library(project_options INTERFACE)
target_compile_features(project_options INTERFACE cxx_std_17)
include_directories(${CMAKE_SOURCE_DIR})
add_subdirectory(src)
add_subdirectory(tests)
add_subdirectory(submodules)
option(ENABLE_TEST_COVERAGE "ENABLE_TEST_COVERAGE" ON)
include(cmake/StaticAnalyzers.cmake)
