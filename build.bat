@echo off

:: Set your paths here
set SDL2_PATH="..\_libs\SDL2-2.0.5"
set LUA_DIR="E:\Programmering\C++\_libs\lua-5.3.4"

mkdir build
cd build
cmake build -D SDL2_PATH:STRING=%SDL2_PATH% ..
cd ..