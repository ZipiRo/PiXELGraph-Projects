#pragma once

void FillRectangle(float x, float y, float width, float height, const Color &color)
{
    float left = x;
    float top = y;
    float right = x + width;
    float bottom = y + height;

    for(int y = top; y <= bottom; y++)
        for(int x = left; x <= right; x++)
            PlotPixel(x, y, color);
}

struct Widget
{
    float x, y, width, height;
    bool isHovered = false;

    virtual void Update(const Vector2 &mousePosition,  bool mouseDown) {}
    virtual void Draw() {}
};