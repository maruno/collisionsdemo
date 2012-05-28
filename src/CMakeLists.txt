add_subdirectory(modelloader)

#Build main executable
add_executable(awesomedemo render/vertexbufferfactory.cpp sceneitems/terrain.cpp scene/world.cpp render/shaderfactory.cpp render/shaderpipeline.cpp heightmap/heightmapgenerator.cpp main.cpp)
add_dependencies(awesomedemo objloader)
target_link_libraries(awesomedemo ${OPENGL_gl_LIBRARY} ${GLFW_LIBRARIES} objloader glload)
