cmake_minimum_required(VERSION 2.8.9)
set(CMAKE_DISABLE_SOURCE_CHANGES ON)
set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)

project(dungeon)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${dungeon_SOURCE_DIR}/cmake")

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIR} "libs/glm")
link_libraries(${SDL2_LIBRARY})

file(GLOB SOURCES "src/*.cpp" "src/*.c" "src/*.h")
add_executable(dungeon ${SOURCES})
