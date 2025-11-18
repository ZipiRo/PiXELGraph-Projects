#pragma once

#include <chrono>

class Time
{
private:
    Time() { StartTime = std::chrono::steady_clock::now(); }

    std::chrono::steady_clock::time_point StartTime;
    std::chrono::duration<double> DeltaTime;

public:
    static float TimeScale;
    static double deltaTime;

    Time(const Time &) = delete;
    void operator=(const Time &) = delete;

    static Time &GetInstance();

    static void Tick();
    static void Reset();
};