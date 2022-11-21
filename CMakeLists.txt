cmake_minimum_required(VERSION 3.0)
project(mhttp)

set(SOURCES buffer_unix.c) 

add_library(mhttp SHARED ${SOURCES})

set_target_properties(mhttp PROPERTIES MACOSX_RPATH TRUE)
