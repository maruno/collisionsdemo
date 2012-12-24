add_subdirectory(collisiondetection)
add_subdirectory(modelloader)
add_subdirectory(renderer)

#Build main executable
add_executable(awesomedemo main.cpp)
target_link_libraries(awesomedemo awesomerenderer ${GLFW_LIBRARIES} glload)
