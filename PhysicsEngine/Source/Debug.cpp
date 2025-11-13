#include "Console/Debug.h"

#include <fstream>
#include <filesystem>
#include <string>

std::ofstream Debug::debugBuffer;

Debug &Debug::GetDebug()
{
    static Debug instance;
    return instance;
}

void Debug::Log(std::string log)
{
    debugBuffer.open("debug.tmp");
    debugBuffer << log << '\n';
    debugBuffer.close();
}
