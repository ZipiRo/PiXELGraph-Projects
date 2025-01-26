#pragma once

std::vector<Vector2> UpdateVertices(Transform &transfrom, const std::vector<Vector2> &vertices)
{
    std::vector<Vector2> transformedVertices;

    transfrom.SinCosUpdate();

    for(const Vector2 &vertex : vertices)
        transformedVertices.emplace_back(transfrom.TransformVertex(vertex));

    return transformedVertices;
}

AABB UpdateBoundingBox(const std::vector<Vector2> &vertices)
{
    int left = MAX_INT;
    int top = MAX_INT;
    int right = MIN_INT;
    int bottom = MIN_INT;

    for(const Vector2 &vertex : vertices)
    {
        if(vertex.x < left) left = vertex.x;
        if(vertex.y < top) top = vertex.y;

        if(vertex.x > right) right = vertex.x;
        if(vertex.y > bottom) bottom = vertex.y;
    }

    return AABB(left, top, right, bottom);
}

void FillShape(Screen &screen, const AABB &bondingBox, const std::vector<Vector2> &vertices, Color::Color color)
{
    int minY = bondingBox.top;
    int maxY = bondingBox.bottom;

    if(minY > maxY)
    {
        int aux = maxY;
        maxY = minY;
        minY = aux;
    }

    int intersections[maxY * 2 + 5];
    
    for(int y = minY; y <= maxY; y++)
    {   
        int count = 0;

        for(auto vertex = vertices.begin(); vertex != vertices.end(); ++vertex)
        {
            auto next_vertex = std::next(vertex);
            if(next_vertex == vertices.end()) 
                next_vertex = vertices.begin();
            
            Vector2 vertexA = *vertex;
            Vector2 vertexB = *next_vertex;

            if(vertexA.y == vertexB.y) continue;

            if ((y >= vertexA.y && y < vertexB.y) || (y >= vertexB.y && y < vertexA.y))
            {
                int x = vertexA.x + (y - vertexA.y) * (vertexB.x - vertexA.x) / (vertexB.y - vertexA.y);
                intersections[count++] = x; 
            }
        }

        for(int i = 0; i < count; i++)
            for(int j = 0; j < count; j++)
                if(intersections[i] < intersections[j])
                {
                    int aux = intersections[i];
                    intersections[i] = intersections[j];
                    intersections[j] = aux;
                }

        for(int i = 0; i + 1 < count; i += 2)
        {   
            int xStart = intersections[i];
            int xEnd = floor(intersections[i + 1]);

            for(int x = xStart; x <= xEnd; x++)
                screen.PutPixel(x, y, color);
        }
    }
}

class Shape
{
protected:
    std::vector<Vector2> vertices;
    std::vector<Vector2> transformedVertices;

    Transform transform;
    AABB boundingBox;

    Color::Color outlineColor;
    Color::Color fillColor;
         
public:
    Shape() {}

    virtual void Draw(Screen &screen);
    virtual std::vector<Vector2> GetVertices();
    virtual AABB GetBoundingBox();
    virtual void SetPivot(Vector2 pivot) {};

    Transform &Transform();

    void SetOutlineColor(Color::Color color);
    void SetFillColor(Color::Color color);
};

void Shape::Draw(Screen &screen)
{
    if(transform.update)
    {
        transformedVertices = UpdateVertices(transform, vertices);
        boundingBox = UpdateBoundingBox(transformedVertices);
        transform.update = false;
    }

    if(fillColor != Color::Transparent)
        FillShape(screen, boundingBox, transformedVertices, fillColor);

    if(outlineColor != Color::Transparent) 
        DrawLines(screen, transformedVertices, outlineColor);
}

std::vector<Vector2> Shape::GetVertices()
{
    if(transform.update)
        transformedVertices = UpdateVertices(transform, vertices);
    transform.update = false;

    return this->transformedVertices;
}

AABB Shape::GetBoundingBox()
{
    if(transform.update)
    {
        transformedVertices = UpdateVertices(transform, vertices);
        boundingBox = UpdateBoundingBox(transformedVertices);
        transform.update = false;
    }

    return this->boundingBox;
}

Transform &Shape::Transform()
{
    if(transform.update)
    {
        transformedVertices = UpdateVertices(transform, vertices);
        boundingBox = UpdateBoundingBox(transformedVertices);
        transform.update = false;
    }

    return this->transform;
}

void Shape::SetOutlineColor(Color::Color color)
{
    this->outlineColor = color;
}

void Shape::SetFillColor(Color::Color color)
{
    this->fillColor = color;
}