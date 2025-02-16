#pragma once

#include <iostream>
#include <fstream>
#include <cmath>
#include <string>
#include <thread>
#include <atomic>
#include <unordered_map>
#include <chrono>
#include <vector>
#include <list>
#include "nlohmann/json.hpp"

namespace winapi 
{
    #include <windows.h>
}

const int MAX_INT = 2147483647;
const int MIN_INT = -2147483647;

const int MAX_PIXELSIZE = 2;

const int MAX_WIDTH = 1920 / MAX_PIXELSIZE;
const int MAX_HEIGHT = 1080 / MAX_PIXELSIZE;

#include "ErrorSystem.h"

#include "ConsoleWindow.h"
#include "EventSystem.h"
#include "InputSystem.h"
#include "Timer.h"

#include "Vector2.h"
#include "Math.h"
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
    Event event;

    void Init(int width, int height, int pixelSize);
private:
    Color backgroundColor = Color::White;
    std::wstring windowTitle = L"Demo";
    float timeScale = 1.0;
    float FPS = 60;

    EventSystem eventSystem;
    ConsoleWindow window;
    Screen screen;
    Timer timer;

    Box screenBounds;
    int screenWidth;
    int screenHeight;
    int pixelSize;
    
    std::atomic<bool> running{false};
    std::thread inputThread;
    std::thread eventThread;

    void InputLoop();
    void EventLoop();

    void HandleError(const std::string& errorMsg);
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
        this->timeScale = timeScale > 0 ? timeScale : 0.1;
        this->timer.TimeScale(timeScale);
    }

    void SetWindowTitle(const std::wstring &title) 
    { 
        this->window.SetTitle(title);
    }

    void SetTitle(const std::wstring &title) { this->windowTitle = title; }
    
    void SetMaxFPS(int maxFPS)
    {
        this->FPS = maxFPS < 1 ? 1 : maxFPS;
    }

    Vector2 ScreenMousePosition(const Vector2 &mousePosition) 
    {
        return Vector2(mousePosition.x / pixelSize, mousePosition.y / pixelSize);
    }

    float GetPixelSize() { return this->pixelSize; }
    Box GetScreenBounds() { return this->screenBounds; }
    int GetScreenWidth() { return this->screen.GetWidth(); }
    int GetScreenHeight() { return this->screen.GetHeight(); }
    std::wstring GetWindowTitle() { return this->windowTitle; }

};
 
void PiXELGraph::Init(int width, int height, int pixelSize)
{
    try
    {
        this->pixelSize = pixelSize < MAX_PIXELSIZE ? MAX_PIXELSIZE : pixelSize;
        
        width /= this->pixelSize;
        height /= this->pixelSize;

        width = (width > MAX_WIDTH) ? MAX_WIDTH : width;
        height = (height > MAX_HEIGHT) ? MAX_HEIGHT : height;

        this->window = ConsoleWindow(width, height, this->pixelSize, this->pixelSize, windowTitle);
        this->screen = Screen(width, height);

        this->screenWidth = this->screen.GetWidth();
        this->screenHeight = this->screen.GetHeight();

        this->screenBounds = Box(0, 0, this->screenWidth, this->screenHeight);

        this->timer = Timer(this->timeScale);
        this->eventSystem = EventSystem(window.GetInputHandle());
        this->input = InputSystem(window.GetHWNDConsole());

        this->running = true;
    }
    catch(const std::exception& e)
    {
        HandleError(e.what());
    }
}

void PiXELGraph::Run()
{
    try
    {
        this->OnStart();

        inputThread = std::thread(&PiXELGraph::InputLoop, this);
        eventThread = std::thread(&PiXELGraph::EventLoop, this);

        // RUNTIME LOOP
        while (running)
        {
            if(window.IsFocused())
            {
                if(input.isKeyPressed(Keyboard::Key_Delete))
                    CLEAR_CONSOLE;
            }
            
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

        if (inputThread.joinable()) inputThread.join();
        if (eventThread.joinable()) eventThread.join();   
    }
    catch(const std::exception& e)
    {
        HandleError(e.what());
    }
}

void PiXELGraph::InputLoop()
{
    while (running)
    {
        if(!window.IsFocused()) continue;
        input.PollInput();
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }
}

void PiXELGraph::EventLoop()
{
    while (running)
    {
        if(!window.IsFocused()) continue;
        eventSystem.PoolEvent(event);
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }   
}

void PiXELGraph::HandleError(const std::string& errorMsg)
{
    window.ConstructOGConsole();

    std::cerr << "Error: " << errorMsg << std::endl;
    std::cerr << "Press Enter to exit..." << std::endl;

    this->OnQuit();
    std::cin.get();
    exit(EXIT_FAILURE);
}

void PiXELGraph::Quit()
{
    this->running = false;
}