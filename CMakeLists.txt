project(awesomedemo)
cmake_minimum_required(VERSION 2.8)

#CMake policies
cmake_policy(VERSION 2.8)

#Module includes
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")
include(FindGLFW)

#Include-directory
include_directories("${CMAKE_SOURCE_DIR}/include" ${CMAKE_CURRENT_BINARY_DIR})

#Enable C++0xB
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

#Find required packages
find_package(OpenGL REQUIRED)
find_package(GLFW REQUIRED)
find_package(FLEX REQUIRED)
find_package(BISON REQUIRED)
find_package(Boost REQUIRED)

#Add GL Load source folder
add_subdirectory(glload)

#Add main Source folder
add_subdirectory(src)