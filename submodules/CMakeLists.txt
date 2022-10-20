set(LIB_NAME ${PROJECT_NAME}_submodules)
add_subdirectory(googletest)
add_subdirectory(SQLiteCpp)
add_subdirectory(fmt EXCLUDE_FROM_ALL)

set(SPDLOG_FMT_EXTERNAL_HO ON)
add_subdirectory(spdlog EXCLUDE_FROM_ALL)
