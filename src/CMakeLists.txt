add_subdirectory(modelloader)

#Build main executable
add_executable(awesomedemo scene/scenegroup.cpp scene/universalgravitation.cpp scene/gravitationalobject.cpp scene/sceneitem.cpp scene/scenemanager.cpp modelloader/vertexbufferfactory.cpp sceneitems/terrain.cpp scene/perspectivecamera.cpp render/shaderfactory.cpp render/shaderpipeline.cpp heightmap/heightmapgenerator.cpp main.cpp)
add_dependencies(awesomedemo objloader)
target_link_libraries(awesomedemo ${OPENGL_gl_LIBRARY} ${GLFW_LIBRARIES} objloader glload)
