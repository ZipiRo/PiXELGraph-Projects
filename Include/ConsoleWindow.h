#pragma once

class ConsoleWindow
{
private:
    winapi::HANDLE ConsoleOutputHandle;
    winapi::HANDLE ConsoleInputHandle;

    winapi::HWND HWNDConsole;

    int ConsoleScreenWidth;
    int ConsoleScreenHeight;
    int ConsoleFontWidth;
    int ConsoleFontHeight;

    std::wstring ConsoleTitle;

    void ConstructConsole();
public:
    ConsoleWindow() {}
    ConsoleWindow(int width, int height, int fontWidth, int fontHeight, const std::wstring &title)
    {
        this->HWNDConsole = winapi::GetConsoleWindow();

        this->ConsoleOutputHandle = winapi::GetStdHandle((winapi::DWORD)-11);
        this->ConsoleInputHandle = winapi::GetStdHandle((winapi::DWORD)-10);

        this->ConsoleScreenWidth = (width > MAX_WIDTH) ? MAX_WIDTH : width;
        this->ConsoleScreenHeight = (height > MAX_HEIGHT) ? MAX_HEIGHT : height;

        this->ConsoleFontWidth = fontWidth;
        this->ConsoleFontHeight = fontHeight;

        this->ConsoleTitle = title;

        ConstructConsole();
    }

    winapi::HANDLE GetOutputHandle();
    winapi::HANDLE *GetInputHandle();
    winapi::HWND *GetHWNDConsole();

    bool IsFocused()
    {
        return winapi::GetForegroundWindow() == this->HWNDConsole;
    }
    
    void SetTitle(const std::wstring &title)
    {
        this->ConsoleTitle = title;
        wchar_t s[256];
        swprintf_s(s, 256, L"PiXELGraph - %s", ConsoleTitle.c_str());
        winapi::SetConsoleTitleW(s);
    }

    void ConstructOGConsole();
};

void ConsoleWindow::ConstructConsole()
{
    if(ConsoleOutputHandle == ((winapi::HANDLE) (winapi::LONG_PTR)-1))
    { throw Error("Bad Handle"); return; }

    winapi::SMALL_RECT WindowRect;

    winapi::DWORD ConsoleMode = 0;
    if(winapi::GetConsoleMode(ConsoleOutputHandle, &ConsoleMode))
    {   
        ConsoleMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
        winapi::SetConsoleMode(ConsoleOutputHandle, ConsoleMode);  
    }
    
    if (winapi::GetConsoleMode(ConsoleInputHandle, &ConsoleMode)) 
    {
        ConsoleMode &= ~ENABLE_QUICK_EDIT_MODE;
        winapi::SetConsoleMode(ConsoleInputHandle, ConsoleMode);
    }

    WindowRect = {0, 0, 1, 1};
    SetConsoleWindowInfo(ConsoleOutputHandle, TRUE, &WindowRect);

    winapi::COORD coord = {(short)ConsoleScreenWidth, (short)ConsoleScreenHeight};
    if(!winapi::SetConsoleScreenBufferSize(ConsoleOutputHandle, coord))
    { throw Error("SetConsoleScreenBufferSize"); return; }

    if(!winapi::SetConsoleActiveScreenBuffer(ConsoleOutputHandle))
    { throw Error("SetConsoleActiveScreenBuffer"); return; }

    winapi::CONSOLE_FONT_INFOEX consoleFontInfo;
    consoleFontInfo.cbSize = sizeof(consoleFontInfo);
    consoleFontInfo.nFont = 0;
    consoleFontInfo.dwFontSize.X = ConsoleFontWidth;
    consoleFontInfo.dwFontSize.Y = ConsoleFontHeight;
    consoleFontInfo.FontFamily = FF_DONTCARE;
    consoleFontInfo.FontWeight = FW_NORMAL;

    wcscpy(consoleFontInfo.FaceName, L"Consolas"); 
    if(!winapi::SetCurrentConsoleFontEx(ConsoleOutputHandle, FALSE, &consoleFontInfo))
    { throw Error("SetCurrentConsoleFontEx"); return; }

    winapi::CONSOLE_SCREEN_BUFFER_INFO csbi;
    if (!winapi::GetConsoleScreenBufferInfo(ConsoleOutputHandle, &csbi))
    { throw Error("GetConsoleScreenBufferInfo"); return; }

    if (ConsoleScreenHeight > csbi.dwMaximumWindowSize.Y)
    { throw Error("Screen Height / Font Height Too Big"); return; }
    
    if (ConsoleScreenWidth> csbi.dwMaximumWindowSize.X)
    { throw Error("Screen Width / Font Width Too Big"); return; }

    winapi::LONG style = winapi::GetWindowLong(this->HWNDConsole, GWL_STYLE);
    style &= ~(WS_SIZEBOX | WS_MAXIMIZEBOX);

    winapi::SetWindowLong(this->HWNDConsole, GWL_STYLE, style);
    winapi::SetWindowPos(this->HWNDConsole, nullptr, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_FRAMECHANGED);
    
    SetTitle(ConsoleTitle);
    
    WindowRect = {0, 0, (short)(ConsoleScreenWidth - 1), (short)(ConsoleScreenHeight - 1)};
    if(!winapi::SetConsoleWindowInfo(ConsoleOutputHandle, TRUE, &WindowRect)) 
    { throw Error("SetConsoleWindowInfo"); return;}
}

void ConsoleWindow::ConstructOGConsole()
{
    std::system("cls");
    winapi::SMALL_RECT WindowRect;
    
    winapi::DWORD ConsoleMode = 0;
    if (winapi::GetConsoleMode(ConsoleOutputHandle, &ConsoleMode)) 
    {
        ConsoleMode &= ~ENABLE_VIRTUAL_TERMINAL_PROCESSING;
        winapi::SetConsoleMode(ConsoleOutputHandle, ConsoleMode);
    }

    if (winapi::GetConsoleMode(ConsoleInputHandle, &ConsoleMode)) 
    {
        ConsoleMode |= ENABLE_QUICK_EDIT_MODE;
        winapi::SetConsoleMode(ConsoleInputHandle, ConsoleMode);
    }

    WindowRect = {0, 0, 140, 40};
    winapi::SetConsoleWindowInfo(ConsoleOutputHandle, TRUE, &WindowRect);

    winapi::COORD coord = {200, 100};
    winapi::SetConsoleScreenBufferSize(ConsoleOutputHandle, coord);

    winapi::CONSOLE_FONT_INFOEX consoleFontInfo;
    consoleFontInfo.cbSize = sizeof(consoleFontInfo);
    consoleFontInfo.nFont = 0;
    consoleFontInfo.dwFontSize.X = 7; 
    consoleFontInfo.dwFontSize.Y = 14;
    consoleFontInfo.FontFamily = FF_DONTCARE;
    consoleFontInfo.FontWeight = FW_NORMAL;
    wcscpy(consoleFontInfo.FaceName, L"Consolas");
    winapi::SetCurrentConsoleFontEx(ConsoleOutputHandle, FALSE, &consoleFontInfo);

    winapi::LONG style = winapi::GetWindowLong(this->HWNDConsole, GWL_STYLE);
    style |= (WS_SIZEBOX | WS_MAXIMIZEBOX);
    winapi::SetWindowLong(this->HWNDConsole, GWL_STYLE, style);
    winapi::SetWindowPos(this->HWNDConsole, nullptr, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_FRAMECHANGED);

    SetTitle(L"Command Prompt");

    winapi::SetWindowPos(this->HWNDConsole, nullptr, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_FRAMECHANGED);

    winapi::COORD cursorPosition = {0, 0};
    winapi::SetConsoleCursorPosition(ConsoleOutputHandle, cursorPosition);
}

winapi::HANDLE ConsoleWindow::GetOutputHandle()
{
    return this->ConsoleOutputHandle;
}

winapi::HANDLE *ConsoleWindow::GetInputHandle()
{
    return &this->ConsoleInputHandle;
}

winapi::HWND *ConsoleWindow::GetHWNDConsole()
{
    return &this->HWNDConsole;
}