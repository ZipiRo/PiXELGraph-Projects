#pragma once

class Polygon : public Shape
{
public:
    Polygon() {}
    Polygon(const std::vector<Vector2> &vertices, float x, float y);
};

Polygon::Polygon(const std::vector<Vector2> &vertices, float x, float y)
{
    this->transform.position = Vector2(x, y);
    this->outlineThickness = 1;
    this->outlineColor = outlineColor;
    this->outlineColor = Color(0, 0, 0);
    this->fillColor = Color(-1, 0, -1);

    this->vertices = vertices;

    this->transform.update = true;
}