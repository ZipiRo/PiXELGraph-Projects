#pragma once
#include <fstream>
#include <string>

class Debug
{
private:
    Debug() {};

    static std::ofstream debugBuffer;

public:
    Debug(const Debug &) = delete;
    void operator=(const Debug &) = delete;
    
    static Debug &GetDebug();
    static void Log(std::string log);
    static void KillDebuger();
};
