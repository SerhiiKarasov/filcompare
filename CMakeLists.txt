project(filcompare LANGUAGES CXX)
cmake_minimum_required(VERSION 3.10)

# Link this 'library' to set the c++ standard / compile-time options requested
add_library(project_options INTERFACE)
target_compile_features(project_options INTERFACE cxx_std_17)
include_directories(${CMAKE_SOURCE_DIR})
add_subdirectory(src)
add_subdirectory(tests)
add_subdirectory(submodules)

include(cmake/StaticAnalyzers.cmake)
