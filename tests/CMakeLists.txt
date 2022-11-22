
set(RETRO_TEST_LIBS retrohttp check)

add_executable(buffer_tests buffer_tests.c)
target_include_directories(buffer_tests PRIVATE "${CHECK_INCLUDE_DIR}")
target_link_libraries(buffer_tests ${RETRO_TEST_LIBS})
add_test(NAME buffer_tests COMMAND buffer_tests)



add_executable(parser_tests parser_tests.c)
target_include_directories(parser_tests PRIVATE "${CHECK_INCLUDE_DIR}")
target_link_libraries(parser_tests ${RETRO_TEST_LIBS})
add_test(NAME parser_tests COMMAND parser_tests)
