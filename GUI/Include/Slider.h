#pragma once

class Slider : public Widget
{
private:
    float value;
    bool isDragging;
    
public:
    Color handleColor;
    Color fillColor;
    Color outlineColor;

    Slider() 
    {
        x = 0; 
        y = 0;
        width = 0;
        height = 0;

        isDragging = false;
        value = 0;
    }

    Slider(float x, float y, float width, float height)
    {
        this->x = x; 
        this->y = y; 
        this->width = width; 
        this->height = height; 

        isDragging = false;
        value = 0;
    }

    void Update(const Vector2 &mousePosition, bool mouseDown) override
    {
        isHovered = mousePosition.x >= x && mousePosition.x <= x + width && mousePosition.y >= y && mousePosition.y <= y + height;
        
        if(isHovered && mouseDown)
            isDragging = true;

        if(!mouseDown)
            isDragging = false;

        if(isDragging)
        {
            float newValue = (mousePosition.x - x) / width;
            newValue > 1.0f ? newValue = 1.0f : newValue;
            newValue < 0.0f ? newValue = 0.0f : newValue;
            
            value = newValue;
        }
    }
    
    void Draw()
    {
        FillRectangle(x, y + height / 2 - 2, width, 4, fillColor);
        DrawRectangle(x, y, width, height, outlineColor);

        int handleX = x + float(value * width);

        Color usedColor = handleColor;
        if(isDragging) 
        {
            usedColor.r -= 50;
            usedColor.g -= 50;
            usedColor.b -= 50;
        }

        FillRectangle(handleX - 5, y, 10, height, usedColor);
    }

    float GetValue()
    {
        return value;
    }

    void SetValue(float newValue)
    {
        value = newValue;
    }
};