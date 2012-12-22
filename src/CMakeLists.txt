add_subdirectory(modelloader)

set(awesomedemo_INCS render/colouredphongsceneitem.hpp render/lightmanager.hpp scene/collisiondetection/collisiondetector.hpp scene/collisiondetection/axisalignedboundingcuboid.hpp scene/collisiondetection/boundingsphere.hpp scene/scenegroup.hpp scene/universalgravitation.hpp scene/gravitationalobject.hpp scene/sceneitem.hpp scene/scenemanager.hpp scene/perspectivecamera.hpp render/shaderfactory.hpp render/shaderpipeline.hpp)

set(awesomedemo_SRCS render/colouredphongsceneitem.cpp render/lightmanager.cpp scene/collisiondetection/collisiondetector.cpp scene/collisiondetection/axisalignedboundingcuboid.cpp scene/collisiondetection/boundingsphere.cpp scene/scenegroup.cpp scene/universalgravitation.cpp scene/gravitationalobject.cpp scene/sceneitem.cpp scene/scenemanager.cpp scene/perspectivecamera.cpp render/shaderfactory.cpp render/shaderpipeline.cpp main.cpp)

#Build main executable
add_executable(awesomedemo ${awesomedemo_SRCS} ${awesomedemo_INCS})
add_dependencies(awesomedemo objloader)
target_link_libraries(awesomedemo ${OPENGL_gl_LIBRARY} ${GLFW_LIBRARIES} objloader glload)
