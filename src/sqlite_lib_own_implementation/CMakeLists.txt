add_library(${PROJECT_NAME}_own_sqlite_lib FCSqliteIO.cpp FCSqliteIO.hpp FCSqliteLib.hpp)
target_link_libraries(
    ${PROJECT_NAME}_own_sqlite_lib
    PRIVATE project_options
    PUBLIC fmt::fmt spdlog::spdlog)
