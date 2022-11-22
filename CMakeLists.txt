cmake_minimum_required(VERSION 3.0)

project(mhttp)

include_directories("${PROJECT_SOURCE_DIR}/include")

add_subdirectory(src)
add_subdirectory(check)

enable_testing()
add_subdirectory(tests)
