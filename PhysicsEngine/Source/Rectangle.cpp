#include <vector>

#include "Graphics/Vertex.h"
#include "Graphics/Rectangle.h"

std::vector<Vertex> CreateRectangleVertices()
{
    std::vector<Vertex> vertices;

    vertices.emplace_back(Vertex(-0.5, 0.5));
    vertices.emplace_back(Vertex(0.5, 0.5));
    vertices.emplace_back(Vertex(0.5, -0.5));
    vertices.emplace_back(Vertex(-0.5, -0.5));

    return vertices;
}

using namespace Shapes;

Rectangle::Rectangle() : Shape(CreateRectangleVertices()) 
{
    width = 1;
    height = 1;

    transform.SetScale(width, height);
}

Rectangle::Rectangle(float width, float height) : Shape(CreateRectangleVertices())
{
    this->width = width;
    this->height = height;

    transform.SetScale(this->width, this->height);
}