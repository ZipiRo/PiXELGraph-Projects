#pragma once

class AABB
{
public:
    float left, right, top, bottom;

    AABB();
    AABB(int left, int top, int right, int bottom);
};