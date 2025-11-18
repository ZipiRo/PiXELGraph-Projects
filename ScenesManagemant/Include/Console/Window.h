#pragma once

#include <string>
#include <windows.h>

#include "ErrorSystem.h"
#include "InputSystem.h"

class Window
{
private:
    Window() {}

    static HWND CONSOLE_WINDOW;

    HANDLE ConsoleOutputH;
    static HANDLE ConsoleInputtH;

    void ConstructWindow();

    static int WIDTH;
    static int HEIGHT;
    static int FONT_SIZE;
    static std::wstring TITLE;

public:
    Window(const Window &) = delete;
    void operator=(const Window &) = delete;
    void SetParameters(int width, int height, int fontSize, const std::wstring &title);

    static Window &GetInstance();

    static void SetTitle(const std::wstring &title);

    static bool Focused();

    static int WindowWidth();
    static int WindowHeight();
    static int WindowFontSize();
    static std::wstring WindowTitle();
    static HWND &ConsoleWindow();
    static HANDLE &ConsoleInputHandle();
};