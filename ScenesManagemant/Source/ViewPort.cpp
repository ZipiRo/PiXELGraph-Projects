#include <cmath>

#include "UMath.h"
#include "Vector2.h"
#include "Core/ViewPort.h"

View::View()
{
    position = Vector2::ZERO;
    screenCenter = Vector2::ZERO;
    angle = 0;
    sin0 = 0;
    cos0 = 1;
    zoom = 1;
    update = true;
}

View::View(Vector2 position, Vector2 screenCenter)
{
    this->position = position;
    this->screenCenter = screenCenter;
    angle = 0;
    sin0 = 0;
    cos0 = 1;
    zoom = 1;
    update = true;
}

View::View(float positionX, float positionY, float screeCenterX, float screeCenterY)
{
    this->position = Vector2(positionX, positionY);
    this->screenCenter = Vector2(screeCenterX, screeCenterY);
    angle = 0;
    sin0 = 0;
    cos0 = 1;
    zoom = 1;
    update = true;
}

void View::SetAngle(float newAngle)
{
    angle = newAngle;
    NormalizeAngle(angle);
    sin0 = sin(angle);
    cos0 = cos(angle);
    update = true;
}

void View::Rotate(float amount)
{
    angle += amount;
    NormalizeAngle(angle);
    sin0 = sin(angle);
    cos0 = cos(angle);
    update = true;
}

void View::Move(const Vector2 &delta)
{
    position += delta;
    update = true;
}

void View::SetZoom(float newZoom)
{
    zoom = newZoom;
    update = true;
}

void View::Zoom(float amount)
{
    zoom += amount;
    update = true;
}

void View::SetPosition(const Vector2 &newPosition)
{
    position = newPosition;
    update = true;
}

void View::SetScreenCenter(const Vector2 &center)
{
    screenCenter = center;
    update = true;
}

Vector2 View::WorldToScreen(const Vector2 &worldPosition)
{
    Vector2 relative = worldPosition - position;

    Vector2 rotated(relative.x * cos0 - relative.y * sin0, 
                    relative.x * sin0 + relative.y * cos0);

    return rotated * zoom + screenCenter;
}

Vector2 View::ScreenToWorld(const Vector2 &screenPosition)
{
    Vector2 relative = (screenPosition - screenCenter) / zoom;
    
    float cos0 = -this->cos0;
    float sin0 = -this->sin0;

    Vector2 unRotated(relative.x * -cos0 - relative.y * -sin0, 
                    relative.x * -sin0 + relative.y * -cos0);

    return unRotated + position;
}

float View::GetZoom() { return zoom; }
Vector2 View::GetPosition() { return position; }
float View::GetAngle() { return angle; }