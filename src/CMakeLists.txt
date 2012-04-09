add_executable(gltest render/shaderfactory.cpp render/shaderpipeline.cpp heightmap/heightmapgenerator.cpp main.cpp)
target_link_libraries(gltest ${OPENGL_gl_LIBRARY} ${GLFW_LIBRARIES} glload)
add_subdirectory(render)