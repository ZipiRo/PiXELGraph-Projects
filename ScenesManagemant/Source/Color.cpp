#include "Core/Color.h"

#include <random>

Color::Color() 
{
    r = g = b = 0;
    a = 1.0f;
}

Color::Color(int r, int g, int b)
{
    if (r < 0) r = 0;
    if (g < 0) g = 0;
    if (b < 0) b = 0;
    if (r > 255) r = 255;
    if (g > 255) g = 255;
    if (b > 255) b = 255;
    a = 1.0f;
    
    this->r = r;
    this->g = g;
    this->b = b;
}

bool Color::operator!= (const Color &other) const
{
    return r != other.r || g != other.g || b != other.b || a != other.a;
}

bool Color::operator== (const Color &other) const
{
    return r == other.r && g == other.g && b == other.b && a == other.a;
}

Color Color::operator*(float factor) const
{
    return Color(this->r * factor, this->g * factor, this->b * factor);
}

Color Color::operator/(float factor) const 
{
    return Color(this->r / factor, this->g / factor, this->b / factor);
}

Color Color::operator*(const Color &other) const
{
    return Color(this->r * other.r, this->g * other.g, this->b * other.b);
}

Color Color::operator/(const Color &other) const
{
    return Color(this->r / other.r, this->g / other.g, this->b / other.b);
}

Color Color::operator+(const Color &other) const 
{
    return Color(this->r + other.r, this->g + other.g, this->b + other.b);
}

Color Color::operator-(const Color &other) const 
{
    return Color(this->r - other.r, this->g - other.g, this->b - other.b);
}

Color Color::operator+(float amount) const 
{
    return Color(this->r + amount, this->g + amount, this->b + amount);
}

Color Color::operator-(float amount) const 
{
    return Color(this->r - amount, this->g - amount, this->b - amount);
}

Color::Color(int r, int g, int b, float a)
{
    if (r < 0) r = 0;
    if (g < 0) g = 0;
    if (b < 0) b = 0;
    if (a < 0) a = 0;
    if (r > 255) r = 255;
    if (g > 255) g = 255;
    if (b > 255) b = 255;
    if (a > 1) a = 1.0f;
    
    this->r = r;
    this->g = g;
    this->b = b;
    this->a = a;
}

Color Color::RandomColor()
{
    int r, g ,b;
    r = 0 + rand() % 255;
    g = 0 + rand() % 255;
    b = 0 + rand() % 255;

    return Color(r, g, b);
}

const Color Color::Transparent = Color(0, 0, 0, 0);
const Color Color::White = Color(255, 255, 255);
const Color Color::Black = Color(1, 1, 1);
const Color Color::Red = Color(255, 0, 0);
const Color Color::Green = Color(0, 255, 0);
const Color Color::Blue = Color(0, 0, 255);
const Color Color::Yellow = Color(255, 255, 0);
const Color Color::Cyan = Color(0, 255, 255);
const Color Color::Purple = Color(255, 0, 255);
const Color Color::Gray = Color(128, 128, 128);