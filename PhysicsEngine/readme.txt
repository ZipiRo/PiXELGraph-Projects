BASH BUILD APP
============================================================
@echo off

g++ DebugWindow.cpp -o Build/Debug/DebugWindow.exe

cd Build 
cmake ..
cmake --build . 

cd Debug
start PiXELGraph
cd ../..
============================================================

BASH RUN APP
============================================================
@echo off

cd Build/Debug
start PiXELGraph
cd ../..
============================================================

By ZipiRo