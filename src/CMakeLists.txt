set(SOURCES buffer.c) 
add_library(retrohttp ${SOURCES})
set_target_properties(retrohttp PROPERTIES MACOSX_RPATH TRUE)

