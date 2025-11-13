#include <cmath>

#include "Vector2.h"

const Vector2 Vector2::ZERO = Vector2(0, 0);
const Vector2 Vector2::UP = Vector2(0, -1);
const Vector2 Vector2::DOWN = Vector2(0, 1);
const Vector2 Vector2::RIGHT = Vector2(1, 0);
const Vector2 Vector2::LEFT = Vector2(-1, 0);

Vector2::Vector2()
{
    this->x = 0;
    this->y = 0;
}

Vector2::Vector2(float x, float y)
{
    this->x = x;
    this->y = y;
}

float Vector2::Length(const Vector2 &vector)
{
    return sqrt(vector.x * vector.x + vector.y * vector.y);
}

float Vector2::Distance(const Vector2 &a, const Vector2 &b)
{
    float dx = a.x - b.x;
    float dy = a.y - b.y;

    return sqrt(dx * dx + dy * dy);
}

Vector2 Vector2::Normalize(Vector2 &vector)
{
    float length = Vector2::Length(vector);
    vector.x /= length;
    vector.y /= length;

    return vector;
}

float Vector2::DotProduct(const Vector2 &a, const Vector2 &b)
{
    return a.x * b.x + a.y * b.y;
}

float Vector2::CrossProduct(const Vector2 &a, const Vector2 &b)
{
    return a.x * b.y - a.y * b.x;
}

Vector2 &Vector2::operator=(const Vector2 vector)
{
    if (this != &vector)
    {
        this->x = vector.x;
        this->y = vector.y;
    }
    return *this;
}

bool Vector2::operator==(const Vector2 &vector)
{
    return (this->x == vector.x && this->y == vector.y);
}

bool Vector2::operator!=(const Vector2 &vector)
{
    return (this->x != vector.x && this->y != vector.y);
}

bool Vector2::operator!() const
{
    return (!this->x && !this->y);
}

Vector2 Vector2::operator-() const
{
    return Vector2(-this->x, -this->y);
}

Vector2 Vector2::operator+(const Vector2 vector) const
{
    return Vector2(this->x + vector.x, this->y + vector.y);
}

Vector2 Vector2::operator-(const Vector2 vector) const
{
    return Vector2(this->x - vector.x, this->y - vector.y);
}

Vector2 operator*(float scalar, const Vector2 &vector)
{
    return Vector2(scalar * vector.x, scalar * vector.y);
}

Vector2 operator/(float scalar, const Vector2 &vector)
{
    return Vector2(vector.x / scalar, vector.y / scalar);
}

Vector2 Vector2::operator*(float scalar) const
{
    return Vector2(this->x * scalar, this->y * scalar);
}

Vector2 Vector2::operator/(float scalar) const
{
    return Vector2(this->x / scalar, this->y / scalar);
}

Vector2 &Vector2::operator+=(const Vector2 &vector)
{
    this->x += vector.x;
    this->y += vector.y;
    return *this;
}

Vector2 &Vector2::operator-=(const Vector2 &vector)
{
    this->x -= vector.x;
    this->y -= vector.y;
    return *this;
}

Vector2 &Vector2::operator*=(float scalar)
{
    this->x *= scalar;
    this->y *= scalar;
    return *this;
}

Vector2 &Vector2::operator/=(float scalar)
{
    this->x /= scalar;
    this->y /= scalar;
    return *this;
}

Vector2 multipyVector2(const Vector2 a, const Vector2 vector)
{
    return Vector2(a.x * vector.x, a.y * vector.y);
}

Vector2 divideVector2(const Vector2 a, const Vector2 vector)
{
    return Vector2(a.x / vector.x, a.y / vector.y);
}