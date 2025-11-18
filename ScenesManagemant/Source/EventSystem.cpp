#include "Console/Window.h"
#include "Console/EventSystem.h"
#include "Console/Debug.h"

#include <windows.h>

EventType Event::event = EventType::EVENT_NONE;
char Event::UChar;

Event &Event::GetInstance()
{
    static Event instance;
    return instance;
}

EventType Event::GetEvent()
{
    return event;
}

void Event::FetchEventData()
{
    DWORD events;
    GetNumberOfConsoleInputEvents(Window::ConsoleInputHandle(), &events);

    if(events <= 0) event = EventType::EVENT_NONE;

    DWORD read;
    INPUT_RECORD eventRecord;
    ReadConsoleInput(Window::ConsoleInputHandle(), &eventRecord, 1, &read);

    switch (eventRecord.EventType)
    {
    case KEY_EVENT:
        if(eventRecord.Event.KeyEvent.bKeyDown) 
        {
            event = EventType::EVENT_KEY_PRESS;

            wchar_t wcharachter = eventRecord.Event.KeyEvent.uChar.UnicodeChar;  
            if(wcharachter != 0)
                UChar = (char)wcharachter;
            else 
                UChar = '?';
        }
        else 
            event = EventType::EVENT_KEY_RELEASE; 

        break;
        
    case MOUSE_EVENT:
        if(eventRecord.Event.MouseEvent.dwEventFlags == MOUSE_MOVED)
        {
            event = EventType::EVENT_MOUSE_MOVE;
            
            break;
        }

        if (eventRecord.Event.MouseEvent.dwEventFlags == MOUSE_WHEELED)
        {
            short delta = (short)HIWORD(eventRecord.Event.MouseEvent.dwButtonState);
            
            if (delta > 0) 
                event = EventType::EVENT_MOUSE_SCROLL_UP;
            else if(delta < 0)
                event = EventType::EVENT_MOUSE_SCROLL_DOWN;
            else EventType::EVENT_NONE;

            break;
        }
        
        if (eventRecord.Event.MouseEvent.dwButtonState & FROM_LEFT_1ST_BUTTON_PRESSED) 
            event = EventType::EVENT_MOUSE_LCLICK;
        else if (eventRecord.Event.MouseEvent.dwButtonState & RIGHTMOST_BUTTON_PRESSED) 
            event = EventType::EVENT_MOUSE_RCLICK;
        else if (eventRecord.Event.MouseEvent.dwButtonState & FROM_LEFT_2ND_BUTTON_PRESSED) 
            event = EventType::EVENT_MOUSE_MCLICK;
        else
            event = EventType::EVENT_NONE;

        break;

    default:
        event = EventType::EVENT_NONE;
        
        break;
    }
}