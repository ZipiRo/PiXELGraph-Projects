#pragma once

class AABB
{
private:
public:
    std::vector<Vector2> box;
    int left; 
    int top; 
    int right; 
    int bottom;

    AABB() {}
    AABB(int left, int top, int right, int bottom);
};

AABB::AABB(int left, int top, int right, int bottom)
{
    this->left = left;
    this->top = top;
    this->right = right;
    this->bottom = bottom;

    this->box.emplace_back(Vector2(left, top));
    this->box.emplace_back(Vector2(right, top));
    this->box.emplace_back(Vector2(right, bottom));
    this->box.emplace_back(Vector2(left, bottom));
}