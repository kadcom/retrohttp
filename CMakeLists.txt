cmake_minimum_required(VERSION 3.0)

project(mhttp)

list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake/")

set(SOURCES buffer_unix.c) 

add_library(mhttp SHARED ${SOURCES})
set_target_properties(mhttp PROPERTIES MACOSX_RPATH TRUE)

add_subdirectory(check)

enable_testing()
add_subdirectory(tests)
