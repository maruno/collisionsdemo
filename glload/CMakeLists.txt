add_library(glload STATIC gll_c.c gll_cpp.cpp gll_gl_ext.c glxl_ext.c wgll_ext.c)
target_link_libraries(glload ${OPENGL_gl_LIBRARY})