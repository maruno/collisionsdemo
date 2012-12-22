set(glload_SRCS gll_c.c gll_cpp.cpp gll_gl_ext.c)

IF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
	set(glload_SRCS ${glload_SRCS} glxl_ext.c)
ENDIF()

add_library(glload STATIC ${glload_SRCS})
target_link_libraries(glload ${OPENGL_gl_LIBRARY})
