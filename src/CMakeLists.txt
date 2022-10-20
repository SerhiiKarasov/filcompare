find_package(ZLIB)
add_subdirectory(sqlite_lib_own_implementation)
add_subdirectory(sqlitecpp_lib)

add_library(
    ${PROJECT_NAME}_lib
    FCFileInfo.cpp
    FCFileInfoHelpers.cpp
    FCMount.cpp
    FCHelpers.cpp
    FCFileInfo.hpp
    FCFileInfoHelpers.hpp
    FCMount.hpp
    FCHelpers.hpp
    IFCSqliteStorage.hpp)

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    target_compile_options(${PROJECT_NAME}_lib PRIVATE -Werror -Wall -Wextra)
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    target_compile_options(${PROJECT_NAME}_lib PRIVATE -Wall -Wextra -Weverything -Wno-c++98-compat
                                                       -Wno-c++98-compat-pedantic)
endif()

# sanitizer options if supported by compiler
include(../cmake/Sanitizers.cmake)
enable_sanitizers(project_options)

# standard compiler warnings
include(../cmake/CompilerWarnings.cmake)
set_project_warnings(project_options)

# allow for static analysis options
include(../cmake/StaticAnalyzers.cmake)

target_compile_features(${PROJECT_NAME}_lib PUBLIC cxx_std_17)
target_link_libraries(
    ${PROJECT_NAME}_lib
    PUBLIC acl
           cap
           sqlite3
           pthread
           dl
           ${ZLIB_LIBRARIES}
           fmt::fmt
           spdlog::spdlog)
target_link_libraries(${PROJECT_NAME}_lib PRIVATE "stdc++fs")
add_executable(${PROJECT_NAME} filcompare.cpp)
target_include_directories(${PROJECT_NAME} PRIVATE submodules/SQLiteCpp/include/SQLiteCpp/)
target_link_libraries(${PROJECT_NAME} PRIVATE ${PROJECT_NAME}_lib ${PROJECT_NAME}_own_sqlite_lib
                                              ${PROJECT_NAME}_sqlitecpp_lib spdlog::spdlog)
target_link_libraries(${PROJECT_NAME} PRIVATE project_options)
target_link_libraries(${PROJECT_NAME} PRIVATE SQLiteCpp)
