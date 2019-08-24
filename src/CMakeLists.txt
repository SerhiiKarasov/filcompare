find_package(Boost COMPONENTS system filesystem program_options REQUIRED)

add_library(${PROJECT_NAME}_lib FCFileInfo.hpp FCFileInfo.cpp)

target_link_libraries(${PROJECT_NAME}_lib
                      PUBLIC acl
                             cap
                             sqlite3
                             Boost::system
                             Boost::filesystem
                             Boost::program_options)

add_executable(${PROJECT_NAME} filcompare.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE ${PROJECT_NAME}_lib)
