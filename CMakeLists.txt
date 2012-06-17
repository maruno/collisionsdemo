project(awesomedemo)
cmake_minimum_required(VERSION 2.8)

#CMake policies
cmake_policy(VERSION 2.8)

#Module includes
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")
include(FindGLFW)

#Include-directory
include_directories("${CMAKE_SOURCE_DIR}/include" ${CMAKE_CURRENT_BINARY_DIR})

#Enable C++0xB and use libc++
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -stdlib=libc++")

#HACK Add c++abi and pthread to linker flags
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -lc++abi -lpthread")

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