#pragma once

#include "Vector2.h"

class View 
{
private:
    Vector2 screenCenter;
    Vector2 position;
    float angle;
    float zoom;

    float sin0;
    float cos0;

public:
    bool update;
    
    View();
    View(Vector2 position, Vector2 screenCenter);
    View(float positionX, float positionY, float screeCenterX, float screeCenterY);

    void SetScreenCenter(const Vector2& center);
    void SetPosition(const Vector2& newPosition);
    void Move(const Vector2& delta);

    void SetAngle(float newAngle);
    void Rotate(float amount);

    void SetZoom(float newZoom);
    void Zoom(float amount);

    Vector2 WorldToScreen(const Vector2& worldPosition);
    Vector2 ScreenToWorld(const Vector2& screenPosition);

    float GetZoom();
    Vector2 GetPosition();
    float GetAngle();
};