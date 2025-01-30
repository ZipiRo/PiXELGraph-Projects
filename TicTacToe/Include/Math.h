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