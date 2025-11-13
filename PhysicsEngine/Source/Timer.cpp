#include "Core/Timer.h"
#include <chrono>

double Time::deltaTime = 0.0;
float Time::TimeScale = 1.0f;

Time &Time::GetInstance()
{
    static Time instance;
    return instance;
}

void Time::Tick()
{
    auto &instance = GetInstance();
    instance.DeltaTime = std::chrono::steady_clock::now() - instance.StartTime;
    deltaTime = instance.DeltaTime.count() * TimeScale;
}

void Time::Reset()
{
    auto &instance = GetInstance();
    instance.StartTime = std::chrono::steady_clock::now();
}