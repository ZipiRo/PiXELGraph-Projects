#pragma once

#include <vector>

#include "Core/Color.h"

#include "Transform.h"
#include "AABB.h"
#include "Vertex.h"

class Shape
{
protected:
    std::vector<Vertex> vertices;

private:
    std::vector<Vertex> Tvertices;
    std::vector<Vertex> cameraTvertices;

    AABB boundingBox;
    AABB cameraBoundingBox;

    Color color;
    Color fillColor;

    friend void DrawShape(Shape &shape);
    Shape() {}

public:
    Transform transform;

    Shape(const std::vector<Vertex> &vertices);

    std::vector<Vertex> GetTvertices();
    AABB GetBoundingBox();

    void SetColor(Color color);
    void SetFillColor(Color color);
};

std::vector<Vertex> UpdateVertices(Transform &transform, const std::vector<Vertex> &vertices);
AABB UpdateAABB(const std::vector<Vertex> &vertices);