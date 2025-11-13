#pragma once

float VerySmallAmount = 0.00001f;

float minF(float a, float b)
{
    return a > b ? b : a;
}

float maxF(float a, float b)
{
    return a > b ? a : b;
}

bool NearlyEqual(float a, float b)
{
    return std::abs(b - a) < VerySmallAmount;
} 

namespace VectorMath
{
    bool NAN_Values(Vector2 vector)
    {
        return (std::isnan(vector.x) || std::isnan(vector.y));
    }
    
    Vector2 VectorTransformZ(Vector2 vector, PTransform transform)
    {
        return Vector2(transform.cos * vector.x - transform.sin * vector.y + transform.position.x, 
                        transform.sin * vector.x + transform.cos * vector.y + transform.position.y);
    }
    
    float DistanceSquared(Vector2 a, Vector2 b)
    {
        float dx = a.x - b.x;
        float dy = a.y - b.y;
    
        return dx * dx + dy * dy;
    }
    
    float LengthSquared(Vector2 vector)
    {
        return vector.x * vector.x + vector.y * vector.y;
    }
    
    bool NearlyEqualVectors(Vector2 a, Vector2 b)
    {
        return DistanceSquared(a, b) < VerySmallAmount * VerySmallAmount;
    } 
}