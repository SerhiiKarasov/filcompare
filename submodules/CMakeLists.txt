set(LIB_NAME ${PROJECT_NAME}_submodules)
add_subdirectory(googletest)

set(SQLITECPP_RUN_CPPLINT
    OFF
    CACHE BOOL "Do not run cpplint for sqlitecpp")
set(SQLITECPP_RUN_CPPCHECK
    OFF
    CACHE BOOL "Do not run cppcheck for sqlitecpp")
add_subdirectory(SQLiteCpp)

add_subdirectory(fmt EXCLUDE_FROM_ALL)

set(SPDLOG_FMT_EXTERNAL_HO ON)
add_subdirectory(spdlog EXCLUDE_FROM_ALL)
