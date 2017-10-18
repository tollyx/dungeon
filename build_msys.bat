@echo off

:: Set your paths here
set SDL2_PATH="/e/Programmering/C++/_libs/SDL2-2.0.5/msys/x86_64-w64-mingw32"
set LUA_DIR="/e/Programmering/C++/_libs/lua-5.3.4"

mkdir build
cd build
cmake build -D LUA_DIR:STRING=%LUA_DIR% -D SDL2_PATH:STRING=%SDL2_PATH% ..
cd ..