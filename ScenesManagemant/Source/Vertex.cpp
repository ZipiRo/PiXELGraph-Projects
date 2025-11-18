#include "Graphics/Vertex.h"

Vertex::Vertex()
{
    position = Vector2::ZERO;
    color = Color::Transparent;
}

Vertex::Vertex(float x, float y, Color color)
{
    position = Vector2(x, y);
    this->color = color;
}

Vertex::Vertex(Vector2 position, Color color)
{
    this->position = position;
    this->color = color;
}
