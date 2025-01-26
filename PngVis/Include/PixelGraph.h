#pragma once

#include <iostream>
#include <fstream>
#include <cmath>
#include <string>
#include <map>
#include <unordered_map>
#include <chrono>
#include <vector>
#include <list>
#include "nlohmann/json.hpp"

namespace winapi 
{
    #include <windows.h>
}

const float PI = 3.1415926535f;

const int MAX_INT = 2147483647;
const int MIN_INT = -2147483647;

const int MAX_WIDTH = 1000;
const int MAX_HEIGHT = 1000;

#include "ConsoleWindow.h"
#include "InputSystem.h"
#include "Timer.h"

#include "Vector2.h"
#include "Box.h"
#include "Utils.h"

#include "Font.h"
#include "Color.h"
#include "Screen.h"
#include "Transform.h"
#include "Shape.h"
#include "Text.h"
#include "Polygon.h"
#include "Rectangle.h"
#include "Elpise.h"

class PiXELGraph
{
protected:
    InputSystem input;

    Color backgroundColor = Color(255, 255, 255);
    std::wstring windowTitle = L"Demo";
    float timeScale = 1.0;
    float FPS = 60;

    void Init(int width, int height, int fontSize);
private:
    ConsoleWindow window;
    Screen screen;
    Timer timer;

    int fontSize;
    Box screenBounds;
    bool running;
public:
    virtual void OnStart() = 0;
    virtual void OnUpdate(float deltaTime) = 0;
    virtual void OnDraw(Screen &screen) = 0;
    virtual void OnQuit() = 0;

    void Run();
    void Quit();

    void SetScreenBackgroundColor(Color color)
    {
        this->backgroundColor = color;
    }

    void SetTimeScale(float timeScale)
    {
        this->timeScale = timeScale < 0 ? 0 : timeScale;
        this->timer.TimeScale(timeScale);
    }

    //todo dont refresh scereen func

    void SetWindowTitle(std::wstring title)
    {
        this->window.SetTitle(title);
    }

    float FontSize() { return this->fontSize; }

    Box GetScreenBounds() { return this->screenBounds; }
};
 
void PiXELGraph::Init(int width, int height, int fontSize = 2)
{
    this->fontSize = fontSize < 2 ? 2 : fontSize;

    this->window = ConsoleWindow(width, height, this->fontSize, this->fontSize, windowTitle);
    this->screen = Screen(width, height); // use the ConsoleWindow Buffer TODO

    this->screenBounds = Box(0, 0, this->screen.GetWidth() - 1, this->screen.GetHeight() - 1);

    this->timer = Timer(this->timeScale);
    this->input = InputSystem(); // use the Console InputHandle TO DO

    this->running = true;
}

void PiXELGraph::Run()
{
    this->OnStart();

    while (running)
    {
        input.PollInput();

        if(input.isKeyPressed(Keyboard::Key_Delete))
            CLEAR_CONSOLE

        timer.Tick();
        if(timer.DeltaTime() >= 1.0 / FPS)
        {
            timer.Reset();
            this->OnUpdate(timer.DeltaTime());
        }

        screen.Clear(backgroundColor);
        this->OnDraw(screen);
        screen.Display();
    }

    this->OnQuit();
}

void PiXELGraph::Quit()
{
    this->running = false;

    // Set the window to the default handle TODO
}