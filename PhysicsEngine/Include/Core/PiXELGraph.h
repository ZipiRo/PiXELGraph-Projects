#pragma once

#include <iostream>
#include <string>
#include <thread>
#include <atomic>
#include <vector>
#include <limits>


#include "UMath.h"
#include "Vector2.h"

#include "Console/ErrorSystem.h"
#include "Console/InputSystem.h"
#include "Console/Window.h"
#include "Console/Debug.h"

class PiXELGraph;

#include "Timer.h"
#include "Color.h"

#include "ViewPort.h"
#include "Screen.h"

#include "Audio/AudioSource.h"

#include "Graphics/Vertex.h"
#include "Graphics/Transform.h"
#include "Graphics/AABB.h"
#include "Graphics/Shape.h"
#include "Graphics/Rectangle.h"
#include "Graphics/Elipse.h"

class PiXELGraph
{
private:
    std::atomic<bool> RUNNING = true;
    std::thread InputThread;

    void InputLoop();

    void HandleError(const std::string &message);

protected:
    double MaxFPS = 60;

    void Init(int WindowWidth, int WindowHeight, int PixelSize, const std::wstring &title);

    virtual void Start() {}
    virtual void Update() {}
    virtual void Draw() {}
    virtual void Quit() {}

    void Exit();

public:
    void Run();
};