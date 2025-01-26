#pragma once

class ConsoleWindow
{
private:
    winapi::HANDLE ConsoleOutputHandle;
    winapi::HANDLE ConsoleInputHandle;
    winapi::SMALL_RECT WindowRect;

    int ConsoleScreenWidth;
    int ConsoleScreenHeight;
    int ConsoleFontWidth;
    int ConsoleFontHeight;

    std::wstring ConsoleTitle;

    bool ConstructConsole();

public:
    ConsoleWindow() {}
    ConsoleWindow(int width, int height, int fontWidth, int fontHeight, const std::wstring &title)
    {
        this->ConsoleOutputHandle = winapi::GetStdHandle((winapi::DWORD)-11);
        this->ConsoleInputHandle = winapi::GetStdHandle((winapi::DWORD)-10);

        this->ConsoleScreenWidth = (width > MAX_WIDTH) ? MAX_WIDTH : width;
        this->ConsoleScreenHeight = (height > MAX_HEIGHT) ? MAX_HEIGHT : height;

        this->ConsoleFontWidth = fontWidth;
        this->ConsoleFontHeight = fontHeight;

        this->ConsoleTitle = title;

        if(!ConstructConsole());
    }

    winapi::HANDLE GetOutputHandle();
    winapi::HANDLE GetInputHandle();

    //focus

    void SetTitle(const std::wstring &title)
    {
        this->ConsoleTitle = title;
        wchar_t s[256];
        swprintf_s(s, 256, L"PiXELGraph - %s", ConsoleTitle.c_str());
        winapi::SetConsoleTitleW(s);
    }
};

bool ConsoleWindow::ConstructConsole()
{
    if(ConsoleOutputHandle == ((winapi::HANDLE) (winapi::LONG_PTR)-1))
        return 0;

    this->WindowRect = {0, 0, 1, 1};
    SetConsoleWindowInfo(ConsoleOutputHandle, TRUE, &WindowRect);

    winapi::COORD coord = {(short)ConsoleScreenWidth, (short)ConsoleScreenHeight};
    if(!winapi::SetConsoleScreenBufferSize(ConsoleOutputHandle, coord)) return 0;

    if(!winapi::SetConsoleActiveScreenBuffer(ConsoleOutputHandle)) return 0;

    winapi::CONSOLE_FONT_INFOEX consoleFontInfo;
    consoleFontInfo.cbSize = sizeof(consoleFontInfo);
    consoleFontInfo.nFont = 0;
    consoleFontInfo.dwFontSize.X = ConsoleFontWidth;
    consoleFontInfo.dwFontSize.Y = ConsoleFontHeight;
    consoleFontInfo.FontFamily = FF_DONTCARE;
    consoleFontInfo.FontWeight = FW_NORMAL;

    wcscpy(consoleFontInfo.FaceName, L"Consolas"); 
    if(!winapi::SetCurrentConsoleFontEx(ConsoleOutputHandle, FALSE, &consoleFontInfo)) return 0;

    winapi::LONG style = winapi::GetWindowLong(winapi::GetConsoleWindow(), GWL_STYLE);
    style &= ~(WS_SIZEBOX | WS_MAXIMIZEBOX);

    winapi::SetWindowLong(winapi::GetConsoleWindow(), GWL_STYLE, style);
    winapi::SetWindowPos(winapi::GetConsoleWindow(), nullptr, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_FRAMECHANGED);
    
    SetTitle(ConsoleTitle);
    
    this->WindowRect = {0, 0, (short)(ConsoleScreenWidth - 1), (short)(ConsoleScreenHeight - 1)};
    if(winapi::SetConsoleWindowInfo(ConsoleOutputHandle, TRUE, &WindowRect)) return 0;


    return 1;
}

winapi::HANDLE ConsoleWindow::GetOutputHandle()
{
    return this->ConsoleOutputHandle;
}

winapi::HANDLE ConsoleWindow::GetInputHandle()
{
    return this->ConsoleInputHandle;
}