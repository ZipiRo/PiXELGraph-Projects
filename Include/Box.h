#pragma once

class Box
{
public:
    std::vector<Vector2> vertices;
    int left; 
    int top; 
    int right; 
    int bottom;

    Box();
    Box(int left, int top, int right, int bottom);
};

Box::Box()
{
    this->left = 0;
    this->top = 0;
    this->right = 0;
    this->bottom = 0;
}

Box::Box(int left, int top, int right, int bottom)
{
    this->left = left;
    this->top = top;
    this->right = right;
    this->bottom = bottom;

    this->vertices.emplace_back(Vector2(left, top));
    this->vertices.emplace_back(Vector2(right, top));
    this->vertices.emplace_back(Vector2(right, bottom));
    this->vertices.emplace_back(Vector2(left, bottom));
}