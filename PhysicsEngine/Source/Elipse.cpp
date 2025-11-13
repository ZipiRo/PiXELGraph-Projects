#include <vector>

#include "UMath.h"

#include "Graphics/Vertex.h"
#include "Graphics/Elipse.h"

std::vector<Vertex> CreateElipseVertices(const int point_count)
{
    std::vector<Vertex> vertices;

    float res = (2 * PI) / point_count;
    for(float angle = 0.f; angle < 2 * PI; angle += res)
    {
        float x = cos(angle); 
        float y = sin(angle);
        vertices.emplace_back(Vector2(x, y));
    }

    return vertices;
}

using namespace Shapes;

Elipse::Elipse() : Shape(CreateElipseVertices(30)) 
{
    radiusX = 1;
    radiusY = 1;
    point_count = 30;

    transform.SetScale(radiusX, radiusY);
}

Elipse::Elipse(float radiusX, float radiusY, const int point_count) : Shape(CreateElipseVertices(point_count))
{
    this->radiusX = radiusX;
    this->radiusY = radiusY;
    this->point_count = point_count;

    transform.SetScale(radiusX, radiusY);
}
