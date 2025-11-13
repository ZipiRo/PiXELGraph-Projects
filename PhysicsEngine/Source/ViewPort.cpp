#include "Vector2.h"
#include "Core/ViewPort.h"

void View::SetPosition(const Vector2 &newPosition) 
{
    position = newPosition;
}

void View::SetScreenCenter(const Vector2 &center)
{
    screenCenter = center;
}

View::View()
{
    this->position = Vector2::ZERO;
    this->screenCenter = Vector2::ZERO;
    zoom = 1;
}

View::View(Vector2 position, Vector2 screenCenter)
{
    this->position = position;
    this->screenCenter = screenCenter;
    zoom = 1;
}

View::View(float positionX, float positionY, float screeCenterX, float screeCenterY)
{
    this->position = Vector2(positionX, positionY);
    this->screenCenter = Vector2(screeCenterX, screeCenterY);
    zoom = 1;
}

void View::Move(const Vector2& delta)
{
    position += delta;
}

void View::SetZoom(float newZoom)
{
    zoom = newZoom;
}

void View::Zoom(float amount)
{
    zoom += amount;
    if (zoom < 0.1f) zoom = 0.1f;
}

Vector2 View::WorldToScreen(const Vector2& worldPosition)
{
    return (worldPosition - position) * zoom + screenCenter;
}

Vector2 View::ScreenToWorld(const Vector2& screenPosition)
{
    return (screenPosition - screenCenter) / zoom + position;
}

float View::GetZoom() { return zoom; }
Vector2 View::GetPosition() { return position; }