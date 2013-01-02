add_subdirectory(collisiondetection)
add_subdirectory(modelloader)
add_subdirectory(renderer)

set(awesomegame_INCS tunnelsegment.hpp)
set(awesomegame_SRCS tunnelsegment.cpp)

#Build main executable
add_executable(awesomegame main.cpp ${awesomegame_INCS} ${awesomegame_SRCS})
target_link_libraries(awesomegame awesomerenderer ${GLFW_LIBRARIES} glload)
