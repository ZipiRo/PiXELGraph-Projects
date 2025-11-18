#pragma once

#include <cmath>

#include "Vector2.h"

class Transform
{
private:
    Vector2 position;
    Vector2 scale;
    float angle;

    float sin0;
    float cos0;

    Vector2 right;
    Vector2 up;

    friend Vector2 TransformVertex(const Transform &transform, Vector2 vertex);

public:
    bool update;

    Transform();
    Transform(Vector2 position, Vector2 scale, float angle);
    Transform(float x, float y, float scaleX, float scaleY, float angle);

    void SetPosition(float x, float y);
    void SetScale(float factorX, float factorY);
    void SetAngle(float angle);
    void SetPosition(Vector2 position);
    void SetScale(Vector2 scale);

    Vector2 GetPosition();
    Vector2 GetScale();
    float GetAngle();

    Vector2 Right();
    Vector2 Up();

    void Move(Vector2 amount);
    void Scale(Vector2 amount);
    void Rotate(float amount);
    void Move(float amountX, float amountY);
    void Scale(float amountX, float amountY);

    Vector2 SinCosUpdate();
};