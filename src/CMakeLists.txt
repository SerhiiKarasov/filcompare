find_package(Boost COMPONENTS system filesystem REQUIRED)
find_package(ZLIB)

set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
add_library(${PROJECT_NAME}_lib
            FCFileInfo.cpp
            FCFileInfoHelpers.cpp
            FCMount.cpp
            FCHelpers.cpp
            FCFileInfo.hpp
            FCFileInfoHelpers.hpp
            FCMount.hpp
            FCHelpers.hpp)

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
  target_compile_options(${PROJECT_NAME}_lib PRIVATE -Werror -Wall -Wextra)
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
  target_compile_options(${PROJECT_NAME}_lib
                         PRIVATE -Wall
                                 -Wextra
                                 -Weverything
                                 -Wno-c++98-compat
                                 -Wno-c++98-compat-pedantic)
endif()

target_compile_features(${PROJECT_NAME}_lib PUBLIC cxx_std_17)
target_link_libraries(${PROJECT_NAME}_lib
                      PUBLIC acl
                             cap
                             sqlite3
                             Boost::system
                             Boost::filesystem
                             ${ZLIB_LIBRARIES})

target_link_libraries(${PROJECT_NAME}_lib PRIVATE "stdc++fs")
add_executable(${PROJECT_NAME} filcompare.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE ${PROJECT_NAME}_lib)
