#pragma once

const float PI = 3.1415926535f;

const float DEG_TO_RAD = PI / 180.0f;
const float RAD_TO_DEG = 180.0f / PI;

float NormalizeAngle(float angle) 
{
    angle = angle - floor(angle / (2.0f * PI)) * (2.0f * PI);
    
    if (angle < 0)
        angle += 2.0f * PI;
    
    return angle;
}

float LengthV2(Vector2 vector)
{
    return sqrt(vector.x * vector.x + vector.y * vector.y);
}

float DistanceV2(Vector2 a, Vector2 b) 
{
    float dx = a.x - b.x;
    float dy = a.y - b.y;

    return sqrt(dx * dx + dy * dy);
}

Vector2 NormalizeV2(Vector2 &vector)
{
    float length = LengthV2(vector);
    vector.x /= length;
    vector.y /= length;
    
    return vector;
}

float DotProductV2(Vector2 a, Vector2 b) 
{
    return a.x * b.x + a.y * b.y;
}

float CrossProductV2(Vector2 a, Vector2 b) 
{
    return a.x * b.y - a.y * b.x;
}