enable_testing()
add_executable( UNIT_TESTS test.cpp )
target_link_libraries(UNIT_TESTS ServerLib gtest gtest_main cryptopp pthread)
add_test(UNIT_TESTS UNIT_TESTS)
set_target_properties(UNIT_TESTS PROPERTIES CXX_STANDARD 17)

