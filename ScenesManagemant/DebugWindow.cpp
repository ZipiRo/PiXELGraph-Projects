#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <windows.h>
#include <cstring>

std::ifstream debug;
std::ofstream debugO;

char buffer[105];
int bufferSize, lastBufferSize;

int main()
{
    wchar_t s[256];
    swprintf_s(s, 256, L"PiXELGraph v2.0 | DebugWindow");
    SetConsoleTitleW(s);
        
    debugO.open("debug.tmp");
    debugO << "";
    debugO.close();

    while (1)
    {      
        bufferSize = std::filesystem::file_size("debug.tmp");
        if(bufferSize == lastBufferSize) continue;
        if(bufferSize == 0)
        {
            lastBufferSize = 0;
            continue;
        }

        lastBufferSize = bufferSize;

        debug.open("debug.tmp");
        debug.getline(buffer, 105);

        if(strstr(buffer, "-0xKILL") != nullptr)
            return 0;

        std::cout << buffer << '\n';
        debug.close();
        
        debugO.open("debug.tmp");
        debugO << "";
        debugO.close();
    }

    return 0;
}