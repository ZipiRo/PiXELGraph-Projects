#include <iostream>

#include "Console/Window.h"

int Window::WIDTH = 0;
int Window::HEIGHT = 0;
int Window::FONT_SIZE = 0;
std::wstring Window::TITLE = L"";
HWND Window::CONSOLE_WINDOW;
HANDLE Window::ConsoleInputtH;

Window &Window::GetInstance()
{
    static Window instance;
    return instance;
}

void Window::ConstructWindow()
{
    system("start DebugWindow.exe");

    if (ConsoleOutputH == INVALID_HANDLE_VALUE)
    {
        throw Error("Bad Handle");
        return;
    }

    SMALL_RECT WindowRect;

    DWORD ConsoleMode = 0;
    if (GetConsoleMode(ConsoleOutputH, &ConsoleMode))
    {
        ConsoleMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
        SetConsoleMode(ConsoleOutputH, ConsoleMode);
    }

    if (GetConsoleMode(ConsoleInputtH, &ConsoleMode))
    {
        ConsoleMode |= ENABLE_EXTENDED_FLAGS;  
        ConsoleMode &= ~ENABLE_QUICK_EDIT_MODE;
        ConsoleMode |= ENABLE_MOUSE_INPUT;     
        SetConsoleMode(ConsoleInputtH, ConsoleMode);
    }

    WindowRect = {0, 0, 1, 1};
    SetConsoleWindowInfo(ConsoleOutputH, TRUE, &WindowRect);

    COORD coord = {(short)WIDTH, (short)HEIGHT};
    if (!SetConsoleScreenBufferSize(ConsoleOutputH, coord))
    {
        throw Error("SetConsoleScreenBufferSize");
        return;
    }

    if (!SetConsoleActiveScreenBuffer(ConsoleOutputH))
    {
        throw Error("SetConsoleActiveScreenBuffer");
        return;
    }

    CONSOLE_FONT_INFOEX consoleFontInfo;
    consoleFontInfo.cbSize = sizeof(consoleFontInfo);
    consoleFontInfo.nFont = 0;
    consoleFontInfo.dwFontSize.X = FONT_SIZE;
    consoleFontInfo.dwFontSize.Y = FONT_SIZE;
    consoleFontInfo.FontFamily = FF_DONTCARE;
    consoleFontInfo.FontWeight = FW_NORMAL;

    wcscpy(consoleFontInfo.FaceName, L"Consolas");
    if (!SetCurrentConsoleFontEx(ConsoleOutputH, FALSE, &consoleFontInfo))
    {
        throw Error("SetCurrentConsoleFontEx");
        return;
    }
    
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(ConsoleOutputH, &cursorInfo);
    cursorInfo.bVisible = FALSE;
    
    if(!SetConsoleCursorInfo(ConsoleOutputH, &cursorInfo))
    {
        throw Error("SetCursorInfo");
        return;
    }


    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if (!GetConsoleScreenBufferInfo(ConsoleOutputH, &csbi))
    {
        throw Error("GetConsoleScreenBufferInfo");
        return;
    }

    if (HEIGHT > csbi.dwMaximumWindowSize.Y)
    {
        throw Error("Screen Height / FontSize Too Big");
        return;
    }

    if (WIDTH > csbi.dwMaximumWindowSize.X)
    {
        throw Error("Screen Width / FontSize Too Big");
        return;
    }

    LONG style = GetWindowLong(CONSOLE_WINDOW, GWL_STYLE);
    style &= ~(WS_SIZEBOX | WS_MAXIMIZEBOX);

    SetWindowLong(CONSOLE_WINDOW, GWL_STYLE, style);
    SetWindowPos(CONSOLE_WINDOW, nullptr, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_FRAMECHANGED);

    SetTitle(TITLE);

    WindowRect = {0, 0, (short)(WIDTH - 1), (short)(HEIGHT - 1)};
    if (!SetConsoleWindowInfo(ConsoleOutputH, TRUE, &WindowRect))
    {
        throw Error("SetConsoleWindowInfo");
        return;
    }
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
}

bool Window::Focused()
{
    return GetForegroundWindow() == CONSOLE_WINDOW;
}

void Window::SetParameters(int width, int height, int fontSize, const std::wstring &title)
{
    fontSize += 1;
    if (fontSize <= 1)
        fontSize = 2;

    WIDTH = width / fontSize;
    HEIGHT = height / fontSize;
    FONT_SIZE = fontSize;
    TITLE = title;

    CONSOLE_WINDOW = GetConsoleWindow();
    ConsoleOutputH = GetStdHandle(STD_OUTPUT_HANDLE);
    ConsoleInputtH = GetStdHandle(STD_INPUT_HANDLE);

    ConstructWindow();
}

void Window::SetTitle(const std::wstring &title)
{
    TITLE = title;
    wchar_t s[256];
    swprintf_s(s, 256, L"PiXELGraph v2.0 - %s", TITLE.c_str());
    SetConsoleTitleW(s);
}

int Window::WindowWidth() { return WIDTH; }
int Window::WindowHeight() { return HEIGHT; }
int Window::WindowFontSize() { return FONT_SIZE; }
std::wstring Window::WindowTitle() { return TITLE; }
HWND &Window::ConsoleWindow() { return CONSOLE_WINDOW; }
HANDLE &Window::ConsoleInputHandle() { return ConsoleInputtH; }