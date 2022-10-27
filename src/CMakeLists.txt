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

target_link_libraries(
    ${PROJECT_NAME}_lib
    PUBLIC ${ZLIB_LIBRARIES}
           acl
           cap
           dl
           fmt::fmt
           project_options
           pthread
           spdlog::spdlog
           sqlite3
           stdc++fs)

add_executable(${PROJECT_NAME} filcompare.cpp)
target_include_directories(${PROJECT_NAME} PRIVATE submodules/SQLiteCpp/include/SQLiteCpp/)
target_link_libraries(${PROJECT_NAME} PRIVATE ${PROJECT_NAME}_lib ${PROJECT_NAME}_own_sqlite_lib
                                              ${PROJECT_NAME}_sqlitecpp_lib spdlog::spdlog project_options SQLiteCpp)
