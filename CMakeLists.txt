project(filcompare LANGUAGES CXX)
cmake_minimum_required(VERSION 3.10)

# Link this 'library' to set the c++ standard / compile-time options requested
add_library(project_options INTERFACE)

# sanitizer options if supported by compiler
include(cmake/Sanitizers.cmake)
enable_sanitizers(project_options)

# standard compiler warnings
include(cmake/CompilerWarnings.cmake)
set_project_warnings(project_options)
target_compile_features(project_options INTERFACE cxx_std_17)

# allow for static analysis options
include(cmake/StaticAnalyzers.cmake)

add_subdirectory(submodules EXCLUDE_FROM_ALL SYSTEM)
add_subdirectory(src)
add_subdirectory(tests)
