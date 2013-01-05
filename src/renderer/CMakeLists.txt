#Renderer library
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

set(awesomedemo_INCS render/colouredphongsceneitem.hpp render/lightmanager.hpp scene/scenegroup.hpp scene/universalgravitation.hpp scene/gravitationalobject.hpp scene/sceneitem.hpp scene/scenemanager.hpp scene/perspectivecamera.hpp render/shaderfactory.hpp render/shaderpipeline.hpp render/matrixuniform.hpp render/colourinformationuniform.hpp)

set(awesomedemo_SRCS render/colouredphongsceneitem.cpp render/lightmanager.cpp scene/scenegroup.cpp scene/universalgravitation.cpp scene/gravitationalobject.cpp scene/sceneitem.cpp scene/scenemanager.cpp scene/perspectivecamera.cpp render/shaderfactory.cpp render/shaderpipeline.cpp)

add_library(awesomerenderer STATIC ${awesomedemo_SRCS} ${awesomedemo_INCS})
#add_dependencies(awesomedemo objloader)
target_link_libraries(awesomerenderer ${OPENGL_gl_LIBRARY} objloader awesomecollisions)
