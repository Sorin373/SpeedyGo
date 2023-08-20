@echo off

rem
mkdir build
cd build

rem
cmake ..

rem
cmake --build .

rem
start main.exe

rem
cd ..

pause
