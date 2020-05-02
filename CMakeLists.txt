project(
    filcompare
    LANGUAGES CXX)
cmake_minimum_required(VERSION 3.10)

add_library(project_options INTERFACE)
target_compile_features(project_options INTERFACE cxx_std_17)

add_subdirectory(src)
add_subdirectory(tests)
add_subdirectory(submodules)

include(cmake/StaticAnalyzers.cmake)
