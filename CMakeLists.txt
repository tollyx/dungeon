cmake_minimum_required(VERSION 3.4)
#set(CMAKE_DISABLE_SOURCE_CHANGES ON)
set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)

execute_process(
	COMMAND git describe --tags --always
	WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
	OUTPUT_VARIABLE GIT_COMMIT
	OUTPUT_STRIP_TRAILING_WHITESPACE
)
file(WRITE "src/gitparams.h" "#define GIT_CUR_COMMIT ${GIT_COMMIT}")

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0 -Wall -Wextra")

set(TARGET dungeon)
project(${TARGET} LANGUAGES CXX)

find_package(OpenGL REQUIRED)
find_package(glbinding REQUIRED)
find_package(Lua REQUIRED)
find_package(SDL2 REQUIRED)
include_directories("libs/kaguya-1.3.2/include")

file(GLOB SOURCES "src/*.cpp" "src/*.c" "src/*.h")
add_executable(${TARGET} ${SOURCES})

#target_link_libraries(${TARGET} PUBLIC glbinding::glbinding)
#target_link_libraries(${TARGET} PUBLIC ${SDL2_LIBRARY})
#target_link_libraries(${TARGET} PUBLIC ${OPENGL_LIBRARY})
#target_link_libraries(${TARGET} PUBLIC ${LUA_LIBRARY})

target_link_libraries(${TARGET} PUBLIC glbinding::glbinding)
target_link_libraries(${TARGET} PUBLIC SDL2)
target_link_libraries(${TARGET} PUBLIC OpenGL)
target_link_libraries(${TARGET} PUBLIC lua)
