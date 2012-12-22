project(awesomedemo)
cmake_minimum_required(VERSION 2.8)

#CMake policies
cmake_policy(VERSION 2.8)

#Module includes
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")
include(FindGLFW)

#Include-directory
include_directories("${CMAKE_SOURCE_DIR}/include" "${CMAKE_SOURCE_DIR}/src" ${CMAKE_CURRENT_BINARY_DIR})

#Enable C++0xB and use libc++
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -stdlib=libc++")

IF(${CMAKE_GENERATOR} MATCHES "Xcode")
	set(CMAKE_XCODE_ATTRIBUTE_GCC_VERSION "com.apple.compilers.llvm.clang.1_0")
	set(CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LANGUAGE_STANDARD "c++0x")
	set(CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LIBRARY "libc++")
ENDIF()

IF(APPLE)
	#Prefer homebrew-versions of Bison and Flex
	set(CMAKE_PROGRAM_PATH /usr/local/bin /usr/local/opt/flex/bin ${CMAKE_PROGRAM_PATH})
	set(CMAKE_INCLUDE_PATH /usr/local/opt/flex/include ${CMAKE_INCLUDE_PATH})

	#Add homebrew include dir
	include_directories("/usr/local/include")
	
	#Add homebrew flex include dir
	include_directories("/usr/local/opt/flex/include")
ENDIF()

IF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
	#HACK add c++abi and pthread to linker flags
	set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -lc++abi -lpthread")
ENDIF()

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
