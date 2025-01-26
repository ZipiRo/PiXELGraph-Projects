#pragma once

class Timer
{
private:
    std::chrono::system_clock::time_point mStartTime;
    std::chrono::duration<float> mDeltaTime;
    float mTimeScale;

public:
    Timer();
    Timer(float timeScale);
    void Reset();
    float DeltaTime();
    void TimeScale(float t);
    float TimeScale();  
    void Tick();
};

Timer::Timer()
{
    Reset();
    mTimeScale = 1.0f;
    mDeltaTime = std::chrono::duration<float>(0.0f);
}

Timer::Timer(float timeScale)
{
    Reset();
    mTimeScale = timeScale;
    mDeltaTime = std::chrono::duration<float>(0.0f);
}

void Timer::Reset()
{
    mStartTime = std::chrono::system_clock::now();
}

float Timer::DeltaTime()
{
    return mDeltaTime.count() * mTimeScale;
}

void Timer::TimeScale(float t = 1.0f)
{
    mTimeScale = t;
}

float Timer::TimeScale()
{
    return mTimeScale;
}

void Timer::Tick()
{
    mDeltaTime = std::chrono::system_clock::now() - mStartTime;
}