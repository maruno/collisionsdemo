add_subdirectory(collisiondetection)
add_subdirectory(modelloader)
add_subdirectory(renderer)

set(awesomegame_INCS asteroid.hpp player.hpp rocket.hpp explosion.hpp)
set(awesomegame_SRCS asteroid.cpp player.cpp rocket.cpp explosion.cpp)

#Build main executable
add_executable(awesomegame main.cpp ${awesomegame_INCS} ${awesomegame_SRCS})
target_link_libraries(awesomegame awesomerenderer ${GLFW_LIBRARIES} ${GCD_LINK_LIBRARIES} glload)
