@echo off

:: Set your paths here
set SDL2_PATH="E:\Programmering\C++\_libs\SDL2-2.0.5"

mkdir build
cd build
cmake build -D SDL2_PATH:STRING=%SDL2_PATH% ..
cd ..