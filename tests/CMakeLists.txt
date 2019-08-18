enable_testing()
set(gtest_SOURCE_DIR /Users/nikita/CLionProjects/googletest)


include_directories(${gtest_SOURCE_DIR}/googletest/include)
link_directories(${gtest_SOURCE_DIR}/build/lib)

add_executable( UNIT_TESTS test.cpp )
# Link test executable against gtest & gtest_main
target_link_libraries(UNIT_TESTS gtest gtest_main ServerLib)
add_test(UNIT_TESTS UNIT_TESTS)

