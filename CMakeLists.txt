cmake_minimum_required(VERSION 3.4)
#set(CMAKE_DISABLE_SOURCE_CHANGES ON)
set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

execute_process(
	COMMAND git describe --tags --always
	WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
	OUTPUT_VARIABLE GIT_COMMIT
	OUTPUT_STRIP_TRAILING_WHITESPACE
)
file(WRITE "src/gitparams.h" "#define GIT_CUR_COMMIT ${GIT_COMMIT}")

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -static-libgcc -static-libstdc++")

set(TARGET dungeon)
project(${TARGET} LANGUAGES CXX)

find_package(OpenGL REQUIRED)
find_package(glbinding REQUIRED)
find_package(Lua REQUIRED)
find_package(SDL2 REQUIRED)
include_directories(
	${OPENGL_INCLUDE_DIR} 
	${SDL2_INCLUDE_DIR} 
	${GLBINDING_INCLUDE_DIRS} 
	${LUA_INCLUDE_DIR} 
	"libs/kaguya-1.3.2/include"
)
message(STATUS ${GLBINDING_LIBRARIES})

link_libraries(
	${OPENGL_LIBRARIES} 
	${SDL2_LIBRARY}
	${LUA_LIBRARIES}
)

file(GLOB SOURCES "src/*.cpp" "src/*.c" "src/*.h")
add_executable(dungeon ${SOURCES})

target_link_libraries(${TARGET} PUBLIC glbinding::glbinding)
