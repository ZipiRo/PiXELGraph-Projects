#include "Graphics/AABB.h"

AABB::AABB()
{
    this->left = 0;
    this->top = 0;
    this->right = 0;
    this->bottom = 0;
}

AABB::AABB(int left, int top, int right, int bottom)
{
    this->left = left;
    this->top = top;
    this->right = right;
    this->bottom = bottom;
}