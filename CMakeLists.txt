cmake_minimum_required(VERSION 2.8.9)
set(CMAKE_DISABLE_SOURCE_CHANGES ON)
set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)

project(dungeon)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${dungeon_SOURCE_DIR}/cmake")

if(MSVC)
	if(DEFINED GLEW_PATH)
		list(APPEND CMAKE_PREFIX_PATH ${GLEW_PATH})	
		if(CMAKE_SIZEOF_VOID_P MATCHES "8")
			list(APPEND CMAKE_PREFIX_PATH "${GLEW_PATH}/lib/Release/x64")	
		else()
			list(APPEND CMAKE_PREFIX_PATH "${GLEW_PATH}/lib/Release/Win32")
		endif()	
	endif(DEFINED GLEW_PATH)
endif(MSVC)

find_package(SDL2 REQUIRED)
find_package(GLEW REQUIRED)
include_directories(${SDL2_INCLUDE_DIR} ${GLEW_INCLUDE_DIRS} "${dungeon_SOURCE_DIR}/libs/glm")
link_libraries(${SDL2_LIBRARY} ${GLEW_LIBRARIES})

file(GLOB SOURCES "src/*.cpp")
add_executable(dungeon ${SOURCES})
