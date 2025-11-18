HOW TO BUILD USING THE BASH BUILD
build.bat

BASH BUILD APP
============================================================
@echo off
setlocal

cls

if not exist Build mkdir Build
cd Build

g++ ../DebugWindow.cpp -o ../Build/Release/DebugWindow
cmake ..
cmake --build . --config Release

cd Release
start PiXELGraph
cd ../..

============================================================

BASH RUN APP
============================================================
@echo off
setlocal

cls

cd Build
cd Release
start PiXELGraph
cd ../..


============================================================

BASE RUNTIME
============================================================
#include "Core/PiXELGraph.h"

class GameScene : public PiXELGraph
{
private:
    Shapes::Elipse e1;

    void Start() override
    {
        Screen::GetView().SetScreenCenter(Vector2(Screen::Width() / 2, Screen::Height() / 2));
        e1 = Shapes::Elipse(10, 10, 3);
        e1.SetColor(Color::Black);
        e1.SetFillColor(Color::Red);
    }

    void Update() override
    {

    }

    void Draw() override
    {
        DrawShape(e1);
    }

public:
    GameScene()
    {
        MaxFPS = 60;
        Init(1280, 720, 3, L"DEMO");
    }
}; 
============================================================

By ZipiRo
