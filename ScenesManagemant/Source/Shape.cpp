#include <limits>

#include "Graphics/Transform.h"
#include "Graphics/Shape.h"

Shape::Shape(const std::vector<Vertex> &vertices)
{
    this->vertices = vertices;

    this->color = Color::Transparent;
    this->fillColor = Color::Transparent;

    boundingBox = UpdateAABB(vertices);

    for (auto &vertex : this->vertices)
        vertex.color = this->color;
}

std::vector<Vertex> UpdateVertices(Transform &transfrom, const std::vector<Vertex> &vertices)
{
    std::vector<Vertex> Tvertices;

    transfrom.SinCosUpdate();

    for (auto &vertex : vertices)
        Tvertices.emplace_back(Vertex(TransformVertex(transfrom, vertex.position), vertex.color));

    return Tvertices;
}

AABB UpdateAABB(const std::vector<Vertex> &vertices)
{
    float left = vertices[0].position.x;
    float right = left;
    float top = vertices[0].position.y;
    float bottom = top;

    for (const auto &vertex : vertices)
    {
        if (vertex.position.x < left)
            left = vertex.position.x;
        if (vertex.position.y < top)
            top = vertex.position.y;

        if (vertex.position.x > right)
            right = vertex.position.x;
        if (vertex.position.y > bottom)
            bottom = vertex.position.y;
    }

    return AABB(left, top, right, bottom);
}

std::vector<Vertex> Shape::GetTvertices()
{
    if (transform.update)
    {
        Tvertices = UpdateVertices(transform, vertices);
        transform.update = false;
    }

    return Tvertices;
}

AABB Shape::GetBoundingBox()
{
    if (transform.update)
    {
        Tvertices = UpdateVertices(transform, vertices);
        boundingBox = UpdateAABB(Tvertices);
        transform.update = false;
    }

    return boundingBox;
}

void Shape::SetColor(Color color)
{
    this->color = color;
    for (auto &vertex : vertices)
        vertex.color = this->color;
}

void Shape::SetFillColor(Color color)
{
    fillColor = color;
}