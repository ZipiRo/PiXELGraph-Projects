#pragma once

#include "Shape.h"

namespace Shapes
{
    class Elipse : public Shape
    {
    private:
        float radiusX, radiusY;
        int point_count;
    
    public:
        Elipse();
        Elipse(float radiusX, float radiusY, const int point_count = 30);
    };   
}
