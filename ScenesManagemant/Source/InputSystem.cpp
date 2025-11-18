#include "Console/Window.h"
#include "Console/InputSystem.h"

Vector2 Input::MousePosition = Vector2::ZERO;

Input &Input::GetInstance()
{
    static Input instance;
    return instance;
}

void Input::FetchInputData()
{
    auto &instance = Input::GetInstance();

    for (auto &[key, state] : instance.Keyboard)
    {
        state.wasPressed = state.pressed;
    }

    for (int key = 0; key < 256; ++key)
    {
        bool active = GetAsyncKeyState(key) & 0x8000;
        instance.Keyboard[Key(key)].pressed = active;
    }

    for (auto &[key, state] : instance.Mouse)
    {
        state.wasPressed = state.pressed;
    }

    instance.Mouse[MouseButton::Left].pressed = GetAsyncKeyState(VK_LBUTTON) & 0x8000;
    instance.Mouse[MouseButton::Right].pressed = GetAsyncKeyState(VK_RBUTTON) & 0x8000;
    instance.Mouse[MouseButton::Middle].pressed = GetAsyncKeyState(VK_MBUTTON) & 0x8000;
    instance.Mouse[MouseButton::Back].pressed = GetAsyncKeyState(VK_XBUTTON1) & 0x8000;
    instance.Mouse[MouseButton::Forward].pressed = GetAsyncKeyState(VK_XBUTTON2) & 0x8000;

    POINT MOUSE_POINT;
    if (GetCursorPos(&MOUSE_POINT))
        if (ScreenToClient(Window::ConsoleWindow(), &MOUSE_POINT))
            MousePosition = Vector2(MOUSE_POINT.x, MOUSE_POINT.y);
}

bool Input::IsKeyDown(Key key)
{
    auto &instance = Input::GetInstance();

    auto it = instance.Keyboard.find(key);
    return it != instance.Keyboard.end() && it->second.pressed && !it->second.wasPressed;
}

bool Input::IsKeyUp(Key key)
{
    auto &instance = Input::GetInstance();

    auto it = instance.Keyboard.find(key);
    if (it != instance.Keyboard.end())
    {
        const auto &state = it->second;
        return !state.pressed && state.wasPressed;
    }
    return false;
}

bool Input::IsKey(Key key)
{
    auto &instance = Input::GetInstance();

    auto it = instance.Keyboard.find(key);
    if (it != instance.Keyboard.end())
    {
        const auto &state = it->second;
        return state.pressed && state.wasPressed;
    }
    return false;
}

bool Input::IsMouseButtonDown(MouseButton button)
{
    auto &instance = Input::GetInstance();

    auto it = instance.Mouse.find(button);
    return it != instance.Mouse.end() && it->second.pressed && !it->second.wasPressed;
}

bool Input::IsMouseButtonUp(MouseButton button)
{
    auto &instance = Input::GetInstance();

    auto it = instance.Mouse.find(button);
    if (it != instance.Mouse.end())
    {
        const auto &state = it->second;
        return !state.pressed && state.wasPressed;
    }
    return false;
}

bool Input::IsMouseButton(MouseButton button)
{
    auto &instance = Input::GetInstance();

    auto it = instance.Mouse.find(button);
    if (it != instance.Mouse.end())
    {
        const auto &state = it->second;
        return state.pressed && state.wasPressed;
    }
    return false;
}
