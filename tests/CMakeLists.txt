add_executable( UNIT_TESTS test.cpp )
target_link_libraries(UNIT_TESTS ServerLib gtest gtest_main cryptopp pthread)
set_target_properties(UNIT_TESTS PROPERTIES CXX_STANDARD 17)

add_test(UNIT_TESTS UNIT_TESTS)
add_custom_command(
        TARGET UNIT_TESTS
        COMMENT "Run tests"
        POST_BUILD
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        COMMAND UNIT_TESTS
)
