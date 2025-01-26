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

    static const Color Transparent;
    static const Color White;
    static const Color Black;
    static const Color Red;
    static const Color Green;
    static const Color Blue;
    static const Color Yellow;
    static const Color Cyan;
    static const Color Purple;
    static const Color Gray;
    static const Color LightRed;
    static const Color LightGreen;
    static const Color LightBlue;
    static const Color LightYellow;
    static const Color LightCyan;
    static const Color LightPurple;
};

const Color Color::Transparent = Color(0, 0, -1);
const Color Color::White = Color(255, 255, 255);
const Color Color::Black = Color(0, 0, 0);
const Color Color::Red = Color(255, 0, 0);
const Color Color::Green = Color(0, 255, 0);
const Color Color::Blue = Color(0, 0, 255);
const Color Color::Yellow = Color(255, 255, 0);
const Color Color::Cyan = Color(0, 255, 255);
const Color Color::Purple = Color(255, 0, 255);
const Color Color::Gray = Color(128, 128, 128);
const Color Color::LightRed = Color(255, 128, 128);
const Color Color::LightGreen = Color(128, 255, 128);
const Color Color::LightBlue = Color(128, 128, 255);
const Color Color::LightYellow = Color(255, 255, 128);
const Color Color::LightCyan = Color(128, 255, 255);
const Color Color::LightPurple = Color(255, 128, 255);