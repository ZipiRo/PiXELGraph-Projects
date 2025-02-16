#pragma once

class FallingObject
{
private:
    Vector2 position;
    float width;
    float height;

    Color fillColor;
    Color outlineColor;

    Box shapeBox;
    Shape* shape;

    float fallSpeed;
    float angularSpeed;

    bool updateC;
    bool updateF;
public:
    FallingObject() {}
    FallingObject(float x, float y, float width, float height, int type)
    {
        if(type == 4)
        {
            shape = new Rectangle(x, y, width, height);
            shape->SetPivot(Vector2(0.5, 0.5));
        }
        else if(type == 10)
        {
            width /= 2;
            height /= 2;
            shape = new Elipse(x, y, width, height);
            shape->SetPivot(Vector2(0.5, 0.5));
        }
        else 
        {
            width /= 2;
            height /= 2;
            shape = new Elipse(x, y, width, height, type);
            shape->SetPivot(Vector2(0.5, 0.5));
        }

        position = Vector2(x, y);
        this->width = width;
        this->height = height;

        fillColor = Color::Gray;
        outlineColor = Color::Black;

        updateC = true;
        updateF = true;
    }

    void SetObjectParams(float fallSpeed, float angularSpeed)
    {
        this->fallSpeed = fallSpeed;
        this->angularSpeed = angularSpeed;
    }

    void Update(float deltaTime)
    {
        position += Vector2(0, fallSpeed * deltaTime);
        updateF = true;

        if(updateF)
        {
            shape->GetTransform().MoveTo(position);
            shape->GetTransform().Rotate(angularSpeed * deltaTime);
            shapeBox = shape->GetBoundingBox();

            updateF = false;
        }
    }

    void Draw(Screen &screen)
    {
        if(updateC)
        {
            shape->SetFillColor(fillColor);
            shape->SetOutlineColor(outlineColor);

            updateC = false;
        }

        shape->Draw(screen);
    }

    Color GetFillColor()
    {
        return fillColor;
    }

    Box GetShapeBox()
    {
        return shapeBox;
    }

    Vector2 GetPosition()
    {
        return position;
    }

    void Move(Vector2 amount)
    {
        position += amount;
        updateF = true;
    }

    void MoveTo(Vector2 position)
    {
        position = position;
        updateF = true;
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