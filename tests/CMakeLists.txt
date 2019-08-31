enable_testing()
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

add_executable(${PROJECT_NAME}_test FCFIleInfoTest.cpp)

target_link_libraries(${PROJECT_NAME}_test
                      PRIVATE ${PROJECT_NAME}_lib gtest gtest_main)

add_test(${PROJECT_NAME}_test ${PROJECT_NAME}_test)

# copy files for unit tests to build directory: set(BASE_TEST_RESOURCES
# test_resource) file(GLOB TEST_RESOURCES "${BASE_TEST_RESOURCES}/*") file(COPY
# ${TEST_RESOURCES} DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/test_resource/)
