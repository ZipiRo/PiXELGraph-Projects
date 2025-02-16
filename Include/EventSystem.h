#pragma once

enum Event
{
    EVENT_NONE,
    EVENT_KEY_PRESS,
    EVENT_KEY_RELEASE,
    EVENT_MOUSE_MOVE,
    EVENT_MOUSE_LCLICK,
    EVENT_MOUSE_RCLICK,
    EVENT_MOUSE_MCLICK,
    EVENT_MOUSE_SCROLL_UP,
    EVENT_MOUSE_SCROLL_DOWN,
};

class EventSystem
{
public:
    EventSystem() {}
    EventSystem(winapi::HANDLE *ConsoleInputHandle)
    {   
        this->ConsoleInputHandle = ConsoleInputHandle;
    }

    void PoolEvent(Event &event)
    {
        winapi::DWORD events;

        winapi::GetNumberOfConsoleInputEvents(*ConsoleInputHandle, &events);

        if(events > 0)
        {
            winapi::DWORD read;
            winapi::ReadConsoleInput(*ConsoleInputHandle, &eventRecord, 1, &read);

            switch (eventRecord.EventType)
            {
            case KEY_EVENT:
                if(eventRecord.Event.KeyEvent.bKeyDown) 
                    event = EVENT_KEY_PRESS;
                else 
                    event = EVENT_KEY_RELEASE; 
                break;

            case MOUSE_EVENT:
                if (eventRecord.Event.MouseEvent.dwEventFlags == MOUSE_MOVED)
                    event = EVENT_MOUSE_MOVE;
                if (eventRecord.Event.MouseEvent.dwButtonState & FROM_LEFT_1ST_BUTTON_PRESSED)
                    event = EVENT_MOUSE_LCLICK;
                if (eventRecord.Event.MouseEvent.dwButtonState & RIGHTMOST_BUTTON_PRESSED)
                    event = EVENT_MOUSE_RCLICK;
                if (eventRecord.Event.MouseEvent.dwButtonState & FROM_LEFT_2ND_BUTTON_PRESSED)
                    event = EVENT_MOUSE_MCLICK;
                if (eventRecord.Event.MouseEvent.dwEventFlags == MOUSE_WHEELED)
                {
                    if ((winapi::SHORT)eventRecord.Event.MouseEvent.dwButtonState > 0)
                        event = EVENT_MOUSE_SCROLL_UP;
                    else
                        event = EVENT_MOUSE_SCROLL_DOWN;
                }
                break;

            default:
                event = EVENT_NONE;
            }   
        }
    }

private:
    winapi::HANDLE *ConsoleInputHandle;
    winapi::INPUT_RECORD eventRecord;
};