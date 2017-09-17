cmake_minimum_required(VERSION 2.8.9)
set(CMAKE_DISABLE_SOURCE_CHANGES ON)
set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libgcc -static-libstdc++")

project(dungeon)

find_package(OpenGL REQUIRED)
find_package(SDL2 REQUIRED)
include_directories(${OPENGL_INCLUDE_DIR} ${SDL2_INCLUDE_DIR} "libs/glm")
link_libraries(${OPENGL_LIBRARIES} ${SDL2_LIBRARY})

file(GLOB SOURCES "src/*.cpp" "src/*.c" "src/*.h")
add_executable(dungeon ${SOURCES})
