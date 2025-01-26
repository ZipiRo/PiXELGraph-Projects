#pragma once

class Transform
{
protected:
    float sin0, cos0;

public:
    bool update;
    
    Vector2 position;
    Vector2 scale;
    float angle;

    Vector2 right;
    Vector2 up;

    Transform();
    Transform(float x, float y, float scale_x, float scale_y, float angle);

    Vector2 TransformVertex(Vector2 vertex);
    Vector2 SinCosUpdate();
    
    void Rotate(float amount);
    void RotateTo(float angle);
    void Move(Vector2 amount);
    void MoveTo(Vector2 position);
    void Scale(Vector2 amount);
    void ScaleTo(Vector2 scale);
};

Transform::Transform()
{
    this->position = ZERO;
    this->scale = Vector2(1.0f, 1.0f);
    this->angle = 0;
    this->sin0 = 0;
    this->cos0 = 1;
    this->right = RIGHT;
    this->up  = UP;
}

Transform::Transform(float x, float y, float scale_x, float scale_y, float angle)
{
    this->position = Vector2(x, y);
    this->scale = Vector2(scale_x, scale_y);
    this->angle = angle;
    this->SinCosUpdate();
}

Vector2 Transform::SinCosUpdate()
{
    cos0 = cos(angle);
    sin0 = sin(angle);

    right = Vector2(cos0, sin0);
    up = Vector2(sin0, -cos0);

    return Vector2(cos0, sin0);
}

Vector2 Transform::TransformVertex(Vector2 vertex)
{
    float sx = vertex.x * scale.x;
    float sy = vertex.y * scale.y;

    float rx = cos0 * sx - sin0 * sy;
    float ry = sin0 * sx + cos0 * sy;

    float tx = rx + position.x;
    float ty = ry + position.y;

    return Vector2(tx, ty);
}

void Transform::ScaleTo(Vector2 scale)
{
    this->scale = scale;
    this->update = true;
}

void Transform::Scale(Vector2 amount)
{
    this->scale += amount;
    this->update = true;
}

void Transform::Rotate(float amount)
{
    this->angle += amount;
    this->angle = NormalizeAngle(this->angle);
    this->update = true;
}

void Transform::RotateTo(float angle)
{
    this->angle = NormalizeAngle(angle);
    this->update = true;
}

void Transform::Move(Vector2 amount)
{
    this->position += amount;
    this->update = true;
}

void Transform::MoveTo(Vector2 position)
{
    this->position = position;
    this->update = true;
}