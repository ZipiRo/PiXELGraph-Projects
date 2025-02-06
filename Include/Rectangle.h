#pragma once

std::vector<Vector2> CreateRectangleVertices(float width, float height, float pivotX, float pivotY)
{
    std::vector<Vector2> vertices;

    float left = -width * pivotX;
    float right = left + width;
    float bottom = -height * pivotY;
    float top = bottom + height;

    vertices.emplace_back(Vector2(left, top));
    vertices.emplace_back(Vector2(right, top));
    vertices.emplace_back(Vector2(right, bottom));
    vertices.emplace_back(Vector2(left, bottom));

    return vertices;
}

class Rectangle : public Shape
{
private:
    float width, height;

public:
    Rectangle() {}
    Rectangle(float x, float y, float width, float height);
    
    void SetPivot(Vector2 pivot) override;
};

Rectangle::Rectangle(float x, float y, float width, float height)
{
    this->transform.position = Vector2(x, y);
    this->width = width;
    this->height = height;
    this->outlineThickness = 1;
    this->outlineColor = Color::Black;
    this->fillColor = Color::Transparent;

    this->vertices = CreateRectangleVertices(this->height, this->width, 0, 0);

    this->transform.update = true;
}

void Rectangle::SetPivot(Vector2 pivot)
{
    vertices = CreateRectangleVertices(width, height, pivot.x, pivot.y);
    transform.update = true;
}