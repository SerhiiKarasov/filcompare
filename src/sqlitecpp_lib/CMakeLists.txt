add_library(${PROJECT_NAME}_sqlitecpp_lib FCSqliteCppImpl.cpp FCSqliteCppImpl.hpp)
target_link_libraries(${PROJECT_NAME}_sqlitecpp_lib PUBLIC fmt::fmt)
