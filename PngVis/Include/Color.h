#pragma once

class Color
{
public:
    int r, g, b;

    Color()
    {
        r = 0;
        g = 0;
        b = 0;
    }

    Color(int r, int g, int b)
    {
        this->r = r;
        this->g = g;
        this->b = b;
    }

    bool operator!= (const Color &other) const
    {
        return r != other.r || g != other.g || b != other.b;
    }

    bool operator== (const Color &other) const
    {
        return r == other.r && g == other.g && b == other.b;
    }
};
