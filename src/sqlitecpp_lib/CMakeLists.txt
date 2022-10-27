add_library(${PROJECT_NAME}_sqlitecpp_lib FCSqliteCppImpl.cpp FCSqliteCppImpl.hpp)
target_link_libraries(
    ${PROJECT_NAME}_sqlitecpp_lib
    PRIVATE project_options SQLiteCpp
    PUBLIC fmt::fmt spdlog::spdlog)
