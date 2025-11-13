#pragma once

class Vector2
{
public:
    float x, y;

    Vector2();
    Vector2(float x, float y);

    Vector2 &operator=(const Vector2 vector);
    bool operator!() const;
    bool operator==(const Vector2 &vector);
    bool operator!=(const Vector2 &vector);

    Vector2 operator-() const;
    Vector2 operator+(const Vector2 vector) const;
    Vector2 operator-(const Vector2 vector) const;
    Vector2 operator*(float scalar) const;
    Vector2 operator/(float scalar) const;
    Vector2 &operator+=(const Vector2 &vector);
    Vector2 &operator-=(const Vector2 &vector);
    Vector2 &operator*=(float scalar);
    Vector2 &operator/=(float scalar);

    static float Length(const Vector2 &vector);
    static Vector2 Normalize(Vector2 &vector);
    static float Distance(const Vector2 &a, const Vector2 &b);
    static float DotProduct(const Vector2 &a, const Vector2 &b);
    static float CrossProduct(const Vector2 &a, const Vector2 &b);

    const static Vector2 ZERO;
    const static Vector2 UP;
    const static Vector2 DOWN;
    const static Vector2 RIGHT;
    const static Vector2 LEFT;
};