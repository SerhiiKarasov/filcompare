find_package(Boost COMPONENTS system filesystem REQUIRED)
find_package(ZLIB)

set(CMAKE_VERBOSE_MAKEFILE ON)

add_library(${PROJECT_NAME}_lib
            FCFileInfo.cpp
            FCFileInfoHelpers.cpp
            FCFileInfo.hpp
            FCFileInfoHelpers.hpp)
target_compile_features(${PROJECT_NAME}_lib PUBLIC cxx_std_14)
target_link_libraries(${PROJECT_NAME}_lib
                      PUBLIC acl
                             cap
                             sqlite3
                             Boost::system
                             Boost::filesystem
                             ${ZLIB_LIBRARIES})

add_executable(${PROJECT_NAME} filcompare.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE ${PROJECT_NAME}_lib)
