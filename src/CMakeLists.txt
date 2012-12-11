add_subdirectory(modelloader)

#Build main executable
add_executable(awesomedemo sceneitems/star.cpp render/lightmanager.cpp scene/collisiondetection/collisiondetector.cpp scene/collisiondetection/axisalignedboundingcuboid.cpp scene/collisiondetection/boundingsphere.cpp sceneitems/planet.cpp sceneitems/genericplanet.cpp modelloader/vertexbuffer.cpp scene/scenegroup.cpp scene/universalgravitation.cpp scene/gravitationalobject.cpp scene/sceneitem.cpp scene/scenemanager.cpp modelloader/vertexbufferfactory.cpp scene/perspectivecamera.cpp render/shaderfactory.cpp render/shaderpipeline.cpp heightmap/heightmapgenerator.cpp main.cpp)
add_dependencies(awesomedemo objloader)
target_link_libraries(awesomedemo ${OPENGL_gl_LIBRARY} ${GLFW_LIBRARIES} objloader glload)
