#pragma once
#include <functional>

class Button : public Widget
{
private:
    std::string text;
    bool isPressed;

    std::function<void()> onClick;
    std::function<void()> onHold;

public:
    Color fillColor;
    Color outlineColor;
    Color textColor;

    Button()
    {
        this->x = x; 
        this->y = y; 
        this->width = width; 
        this->height = height; 

        isPressed = false;
        text = "";
    }
    
    Button(float x, float y, float width, float height, const std::string &text)
    {
        this->x = x; 
        this->y = y; 
        this->width = width; 
        this->height = height; 
        this->text = text;
        
        isPressed = false;
    }

    void Update(const Vector2 &mousePosition, bool mouseDown) override
    {
        bool overlap = mousePosition.x >= x && mousePosition.x <= x + width && mousePosition.y >= y && mousePosition.y <= y + height;

        isHovered = overlap;

        if(isHovered && mouseDown && !isPressed)
            onClick();
        
        isPressed = isHovered && mouseDown;
    }

    void Draw() override
    {
        Text text_T;
        text_T.SetString(this->text);
        text_T.SetColor(textColor);

        text_T.transform.SetScale(4, 4);

        AABB boundingBox = text_T.GetBoundingBox();
        text_T.transform.SetPosition((x - (boundingBox.right - boundingBox.left) * 0.5f) + width * 0.5f, (y - (boundingBox.bottom - boundingBox.top) * 0.5f) + height * 0.5f);

        Color usedColor = isPressed ? Color(fillColor.r - 100, fillColor.g - 100, fillColor.b - 100):
                            isHovered ? Color(fillColor.r - 50, fillColor.g - 50, fillColor.b - 50):
                            fillColor;
        
        FillRectangle(x, y, width, height, usedColor);
        DrawRectangle(x, y, width, height, outlineColor);
        DrawTEXT(text_T);
    }

    void SetOnClick(std::function<void()> function)
    {
        onClick = function;
    }
};