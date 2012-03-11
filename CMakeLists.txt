project(heightmap)
cmake_minimum_required(VERSION 2.8)

#CMake policies
cmake_policy(VERSION 2.8)

#Module includes
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")
include(FindGLFW)

#Include-directory
include_directories("${CMAKE_SOURCE_DIR}/include")

#Find required packages
find_package(OpenGL REQUIRED)
find_package(GLFW REQUIRED)

#Add GL Load source folder
add_subdirectory(glload)

#Add main Source folder
add_subdirectory(src)