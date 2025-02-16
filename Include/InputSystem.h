#pragma once

enum Keyboard 
{
    Key_None = 0,
    Key_Backspace = VK_BACK,
    Key_Tab = VK_TAB,
    Key_Enter = VK_RETURN,
    Key_CapsLock = VK_CAPITAL,
    Key_Delete = VK_DELETE,
    Key_Escape = VK_ESCAPE,
    Key_Space = VK_SPACE,
    Key_PageUp = VK_PRIOR,
    Key_PageDown = VK_NEXT,
    Key_End = VK_END,
    Key_Home = VK_HOME,
    Key_LeftArrow = VK_LEFT,
    Key_UpArrow = VK_UP,
    Key_RightArrow = VK_RIGHT,
    Key_DownArrow = VK_DOWN,
    Key_Select = VK_SELECT,

    Key_0 = 0x30,
    Key_1 = 0x31,
    Key_2 = 0x32,
    Key_3 = 0x33,
    Key_4 = 0x34,
    Key_5 = 0x35,
    Key_6 = 0x36,
    Key_7 = 0x37,
    Key_8 = 0x38,
    Key_9 = 0x39,

    Key_A = 0x41,
    Key_B = 0x42,
    Key_C = 0x43,
    Key_D = 0x44,
    Key_E = 0x45,
    Key_F = 0x46,
    Key_G = 0x47,
    Key_H = 0x48,
    Key_I = 0x49,
    Key_J = 0x4A,
    Key_K = 0x4B,
    Key_L = 0x4C,
    Key_M = 0x4D,
    Key_N = 0x4E,
    Key_O = 0x4F,
    Key_P = 0x50,
    Key_Q = 0x51,
    Key_R = 0x52,
    Key_S = 0x53,
    Key_T = 0x54,
    Key_U = 0x55,
    Key_V = 0x56,
    Key_W = 0x57,
    Key_X = 0x58,
    Key_Y = 0x59,
    Key_Z = 0x5A,

    Key_Numpad0 = VK_NUMPAD0,
    Key_Numpad1 = VK_NUMPAD1,
    Key_Numpad2 = VK_NUMPAD2,
    Key_Numpad3 = VK_NUMPAD3,
    Key_Numpad4 = VK_NUMPAD4,
    Key_Numpad5 = VK_NUMPAD5,
    Key_Numpad6 = VK_NUMPAD6,
    Key_Numpad7 = VK_NUMPAD7,
    Key_Numpad8 = VK_NUMPAD8,
    Key_Numpad9 = VK_NUMPAD9,

    Key_Multiply = VK_MULTIPLY,
    Key_Add = VK_ADD,
    Key_Separator = VK_SEPARATOR,
    Key_Subtract = VK_SUBTRACT,
    Key_Decimal = VK_DECIMAL,
    Key_Divide = VK_DIVIDE,

    Key_F1 = VK_F1,
    Key_F2 = VK_F2,
    Key_F3 = VK_F3,
    Key_F4 = VK_F4,
    Key_F5 = VK_F5,
    Key_F6 = VK_F6,
    Key_F7 = VK_F7,
    Key_F8 = VK_F8,
    Key_F9 = VK_F9,
    Key_F10 = VK_F10,
    Key_F11 = VK_F11,
    Key_F12 = VK_F12,
    Key_F13 = VK_F13,
    Key_F14 = VK_F14,
    Key_F15 = VK_F15,
    Key_F16 = VK_F16,
    Key_F17 = VK_F17,
    Key_F18 = VK_F18,
    Key_F19 = VK_F19,
    Key_F20 = VK_F20,
    Key_F21 = VK_F21,
    Key_F22 = VK_F22,
    Key_F23 = VK_F23,
    Key_F24 = VK_F24,

    Key_LeftShift = VK_LSHIFT,
    Key_RightShift = VK_RSHIFT,
    Key_LeftControl = VK_LCONTROL,
    Key_RightControl = VK_RCONTROL,
    Key_LeftAlt = VK_LMENU,
    Key_RightAlt = VK_RMENU
};

enum Mouse 
{
    Left = 0,
    Right = 1,
    Middle = 2
};

class InputSystem 
{
public:
    InputSystem() {}
    InputSystem(winapi::HWND *HWNDConsole)
    {   
        this->HWNDConsole = HWNDConsole;
    }

    void PollInput() 
    {
        for (auto& [key, state] : keyStates) 
        {
            state.wasPressed = state.isPressed;
        }

        for (int key = 0; key < 256; ++key) 
        {
            bool isCurrentlyPressed = winapi::GetAsyncKeyState(key) & 0x8000;
            keyStates[Keyboard(key)].isPressed = isCurrentlyPressed;
        }

        for (auto& [button, state] : mouseButtonStates)
        {
            state.wasPressed = state.isPressed;
        }

        mouseButtonStates[Mouse::Left].isPressed = winapi::GetAsyncKeyState(VK_LBUTTON) & 0x8000;
        mouseButtonStates[Mouse::Right].isPressed = winapi::GetAsyncKeyState(VK_RBUTTON) & 0x8000;
        mouseButtonStates[Mouse::Middle].isPressed = winapi:: GetAsyncKeyState(VK_MBUTTON) & 0x8000;

        winapi::POINT point;
        if (winapi::GetCursorPos(&point)) 
        {
            if (winapi::ScreenToClient(*HWNDConsole, &point)) 
            {
                mousePositionX = point.x;
                mousePositionY = point.y;
            }
        }   
    }

    bool isKeyDown(Keyboard key) const 
    {
        auto it = keyStates.find(key);
        return it != keyStates.end() && it->second.isPressed;
    }

    bool isKeyUp(Keyboard key) const 
    {
        auto it = keyStates.find(key);
        if (it != keyStates.end()) 
        {
            const auto& state = it->second;
            return !state.isPressed && state.wasPressed;
        }
        return false;
    }

    bool isKeyPressed(Keyboard key) const 
    {
        auto it = keyStates.find(key);
        if (it != keyStates.end()) 
        {
            const auto& state = it->second;
            return state.isPressed && !state.wasPressed;
        }
        return false;
    }

    bool isKeyHeld(Keyboard key) const 
    {
        auto it = keyStates.find(key);
        if (it != keyStates.end()) 
        {
            const auto& state = it->second;
            return state.isPressed && state.wasPressed;
        }
        return false;
    }

    bool isMouseButtonDown(Mouse button) const 
    {
        auto it = mouseButtonStates.find(button);
        return it != mouseButtonStates.end() && it->second.isPressed;
    }

    bool isMouseButtonUp(Mouse button) const 
    {
        auto it = mouseButtonStates.find(button);
        if (it != mouseButtonStates.end()) 
        {
            const auto& state = it->second;
            return !state.isPressed && state.wasPressed;
        }
        return false;
    }

    bool isMouseButtonPressed(Mouse button) const 
    {
        auto it = mouseButtonStates.find(button);
        if (it != mouseButtonStates.end()) 
        {
            const auto& state = it->second;
            return state.isPressed && !state.wasPressed;
        }
        return false;
    }

    bool isMouseButtonHeld(Mouse button) const 
    {
        auto it = mouseButtonStates.find(button);
        if (it != mouseButtonStates.end()) 
        {
            const auto& state = it->second;
            return state.isPressed && state.wasPressed;
        }
        return false;
    }

    int GetMousePositionX() const 
    {
        return mousePositionX;
    }

    int GetMousePositionY() const 
    {
        return mousePositionY;
    }

private:
    struct KeyState 
    {
        bool isPressed = false;
        bool wasPressed = false;
    };

    struct MouseButtonState {
        bool isPressed = false;
        bool wasPressed = false;
    };

    std::unordered_map<Keyboard, KeyState> keyStates;
    std::unordered_map<Mouse, MouseButtonState> mouseButtonStates;
    int mousePositionX = 0;
    int mousePositionY = 0;

    winapi::HWND *HWNDConsole;
};