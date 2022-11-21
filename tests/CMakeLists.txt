add_executable(buffer_tests buffer_tests.c)
target_include_directories(buffer_tests PRIVATE "${CHECK_INCLUDE_DIR}")
target_link_libraries(buffer_tests mhttp check)
add_test(NAME buffer_tests COMMAND buffer_tests)
