#pragma once

std::vector<Vector2> CreateElipseVertices(float radiusX, float radiusY, const int point_count, float pivotX, float pivotY)
{
    std::vector<Vector2> vertices;

    pivotX = radiusX * pivotX;
    pivotY = radiusY * pivotY;

    float res = (2 * PI) / point_count;
    for(float angle = 0.f; angle < 2 * PI; angle += res)
    {
        // PUT THE VERTICES RELATIVE TO THE 0, 0 PIVOT POINT
        float x = cos(angle) * radiusX - pivotX + radiusX; 
        float y = sin(angle) * radiusY - pivotY + radiusY;

        // PUT THE VERTICES AT THE SPECIFIED PIVOT POINT
        x -= pivotX;
        y -= pivotY;

        vertices.emplace_back(Vector2(x, y));
    }

    return vertices;
}

class Elipse : public Shape  
{
private:
    float radiusX, radiusY;
    int point_count;

public:
    Elipse () {}
    Elipse (float x, float y, float radiusX, float radiusY, const int point_count = 30);

    void Draw(Screen &screen) override;
    void SetPivot(Vector2 pivot) override;
    Box GetBoundingBox() override;
};

Elipse::Elipse(float x, float y, float radiusX, float radiusY, const int point_count)
{
    this->transform.position = Vector2(x, y);
    this->radiusX = radiusX;
    this->radiusY = radiusY;
    this->outlineThickness = 1;
    this->point_count = point_count;
    this->outlineColor = Color::Black;
    this->fillColor = Color::Transparent;

    this->vertices = CreateElipseVertices(this->radiusX, this->radiusY, point_count, 0, 0);

    this->transform.update = true;
}

Box Elipse::GetBoundingBox()
{
    if(transform.update)
    {
        transformedVertices = UpdateVertices(transform, vertices);
        boundingBox = (point_count > 15) ? UpdateElipseBoundingBox(transform.position, transform.scale.x * radiusX, transform.scale.y * radiusY) : 
                                            UpdatePolygonBoundingBox(transformedVertices);
        transform.update = false;
    }

    return this->boundingBox;
}

void Elipse::Draw(Screen &screen)
{
    if(transform.update)
    {
        transformedVertices = UpdateVertices(transform, vertices);
        boundingBox = (point_count > 15) ? UpdateElipseBoundingBox(transform.position, transform.scale.x * radiusX, transform.scale.y * radiusY) : 
                                            UpdatePolygonBoundingBox(transformedVertices);
        transform.update = false;
    }

    if(fillColor != Transparent)
        FillShape(screen, boundingBox, transformedVertices, fillColor);

    if(outlineColor != Transparent) 
        DrawLines(screen, transformedVertices, outlineColor, outlineThickness);
}


void Elipse::SetPivot(Vector2 pivot)
{
    vertices = CreateElipseVertices(this->radiusX, this->radiusY, point_count, pivot.x, pivot.y);
    transform.update = true;
}