#pragma once

#include "Shape.h"

namespace Shapes
{
    class Rectangle : public Shape
    {
    private:
        float width, height;

    public:
        Rectangle();
        Rectangle(float width, float height);
    };
}