#pragma once

class Player
{
private:
    Vector2 position;
    float width;
    float height;
    float angle;

    float rightBorder;
    float leftBorder;

    Color fillColor;
    Color outlineColor;

    Box shapeBox;
    Rectangle shape;

    bool updateC;
    bool updateF;
public:
    float speed;

    Player() {}

    Player(float x, float y, float width, float height)
    {
        shape = Rectangle(x, y, width, height);
        shape.SetPivot(Vector2(0.5, 0.5));

        position = Vector2(x, y);
        this->width = width;
        this->height = height;

        fillColor = Color::Gray;
        outlineColor = Color::Black;

        updateC = true;
        updateF = true;
    }

    void Update(float deltaTime)
    {
        // Reset Tilt
        if(angle == 1)
        {
            angle = 0;
            shape.GetTransform().RotateTo(angle);
            shapeBox = shape.GetBoundingBox();
        }

        if(updateF)
        {
            shape.GetTransform().MoveTo(position);
            shape.GetTransform().RotateTo(angle);
            shapeBox = shape.GetBoundingBox();
            
            angle = 1;
            updateF = false;
        }
    }

    void Draw(Screen &screen)
    {
        if(updateC)
        {
            shape.SetFillColor(fillColor);
            shape.SetOutlineColor(outlineColor);

            updateC = false;
        }

        shape.Draw(screen);
    }
    
    Box GetShapeBox()
    {
        return shapeBox;
    }

    void SetMoveingBorder(float left, float right)
    {
        leftBorder = left;
        rightBorder = right;
    }

    void Move(float direction)
    {
        float X = position.x + speed * direction;

        // Make Player Tilt In The Moveing Direction
        if(direction > 0)
        {
            angle = 5 * DEG_TO_RAD;
        }
        
        if(direction < 0) 
        {
            angle = -5 * DEG_TO_RAD; 
        }

        // Clip Movemant To Left, Right Borders
        if(X - width / 2 >= leftBorder && X + width / 2 < rightBorder)
        {
            position.x = X;
        }

        updateF = true;
    }

    void MoveTo(float x)
    {
        // Clip Movemant To Left, Right Borders
        if(x - width / 2 >= leftBorder && x + width / 2 <= rightBorder)
        {
            position.x = x;
            updateF = true;
        }
    }

    void SetFillColor(Color color)
    {
        fillColor = color;
        updateC = true;
    }

    void SetOutlineColor(Color color)
    {
        outlineColor = color;
        updateC = true;
    }
};