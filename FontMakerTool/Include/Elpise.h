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

AABB UpdateElipseBoundingBox(const Transform &transform, float radiusX, float radiusY)
{   
    int left = transform.position.x - (radiusX * transform.scale.x);
    int top = transform.position.y - (radiusY * transform.scale.y);
    int right = transform.position.x + (radiusX * transform.scale.x);
    int bottom = transform.position.y + (radiusY * transform.scale.y);

    return AABB(left, top, right, bottom);
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
    AABB GetBoundingBox() override;
    void SetPivot(Vector2 pivot) override;
};

Elipse::Elipse(float x, float y, float radiusX, float radiusY, const int point_count)
{
    this->transform.position = Vector2(x, y);
    this->radiusX = radiusX;
    this->radiusY = radiusY;
    this->point_count = point_count;
    this->outlineColor = Color::Transparent;
    this->fillColor = Color::Transparent;

    this->vertices = CreateElipseVertices(this->radiusX, this->radiusY, point_count, 0, 0);
    
    this->transform.update = true;
}

void Elipse::Draw(Screen &screen)
{
    if(transform.update)
    {
        transformedVertices = UpdateVertices(transform, vertices);
        if(point_count > 15) boundingBox = UpdateElipseBoundingBox(transform, radiusX, radiusY);
            else boundingBox = UpdateBoundingBox(transformedVertices);

        transform.update = false;
    }

    if(fillColor != Color::Transparent)
        FillShape(screen, boundingBox, transformedVertices, fillColor);

    if(outlineColor != Color::Transparent) 
        DrawLines(screen, transformedVertices, outlineColor);
}

AABB Elipse::GetBoundingBox()
{
    if(transform.update)
    {
        transformedVertices = UpdateVertices(transform, vertices);
        if(point_count > 15) boundingBox = UpdateElipseBoundingBox(transform, radiusX, radiusY);
            else boundingBox = UpdateBoundingBox(transformedVertices);

        transform.update = false;
    }

    return boundingBox;
}

void Elipse::SetPivot(Vector2 pivot)
{
    vertices = CreateElipseVertices(this->radiusX, this->radiusY, point_count, pivot.x, pivot.y);
}