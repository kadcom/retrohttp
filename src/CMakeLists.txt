set(SOURCES buffer_unix.c) 
add_library(mhttp ${SOURCES})
set_target_properties(mhttp PROPERTIES MACOSX_RPATH TRUE)

