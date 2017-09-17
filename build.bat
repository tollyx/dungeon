@echo off

:: Set your paths here
set SDL2_PATH="E:\Programmering\C++\_libs\SDL2-2.0.5"
set GLEW_PATH="E:\Programmering\C++\_libs\GLEW-2.1.0"

mkdir build
cd build
cmake -D SDL2_PATH=%SDL2_PATH% -D GLEW_PATH=%GLEW_PATH% ..
cd ..