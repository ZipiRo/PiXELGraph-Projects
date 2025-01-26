class Vector2
{
public:
    float x, y;

    Vector2();
    Vector2(float x, float y);

    Vector2 &operator=(const Vector2 vector);
    bool operator!() const;
    bool operator==(const Vector2& vector);
    bool operator!=(const Vector2& vector);
    
    Vector2 operator-() const;
    Vector2 operator+(const Vector2 vector) const;
    Vector2 operator-(const Vector2 vector) const;
    
    friend Vector2 operator*(float scalar, const Vector2& vector);
    friend Vector2 operator/(float scalar, const Vector2& vector);

    Vector2 operator*(float scalar) const;
    Vector2 operator/(float scalar) const;
    Vector2 &operator+=(const Vector2& vector);
    Vector2 &operator-=(const Vector2& vector);
    Vector2 &operator*=(float scalar);
    Vector2 &operator/=(float scalar);
};

const Vector2 ZERO(0, 0);
const Vector2 UP(0, -1);
const Vector2 DOWN(0, 1);
const Vector2 RIGHT(1, 0);
const Vector2 LEFT(-1, 0);

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

Vector2& Vector2::operator=(const Vector2 vector) 
{
    if (this != &vector) 
    {
        this->x = vector.x;
        this->y = vector.y;
    }
    return *this;
}

bool Vector2::operator==(const Vector2& vector)
{
    return (this->x == vector.x && this->y == vector.y);
}

bool Vector2::operator!=(const Vector2& vector)
{
    return (this->x != vector.x && this->y != vector.y);
}

bool Vector2::operator!() const
{
    return (!this->x && !this->y);
}

Vector2 Vector2::operator-() const {
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

Vector2 operator*(float scalar, const Vector2& vector) 
{
    return Vector2(scalar * vector.x, scalar * vector.y);
}

Vector2 operator/(float scalar, const Vector2& vector) 
{
    return Vector2(scalar / vector.x, scalar / vector.y);
}

Vector2 Vector2::operator*(float scalar) const 
{
    return Vector2(this->x * scalar, this->y * scalar);
}

Vector2 Vector2::operator/(float scalar) const 
{
    return Vector2(this->x / scalar, this->y / scalar);
}

Vector2& Vector2::operator+=(const Vector2& vector) 
{
    this->x += vector.x;
    this->y += vector.y;
    return *this;
}

Vector2& Vector2::operator-=(const Vector2& vector) 
{
    this->x -= vector.x;
    this->y -= vector.y;
    return *this;
}

Vector2& Vector2::operator*=(float scalar) 
{
    this->x *= scalar;
    this->y *= scalar;
    return *this;
}

Vector2& Vector2::operator/=(float scalar) 
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