#pragma once

enum EventType
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

class Event
{
private:
    Event() {}

    friend class PiXELGraph;

    static void FetchEventData();
    static EventType event;
public:
    Event(const Event &) = delete;
    void operator=(const Event &) = delete;

    static char UChar;

    static EventType GetEvent();

    static Event &GetInstance();
};