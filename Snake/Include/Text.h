void CreateTextVertices(const std::string &string, std::vector<Vector2> &vertices, std::vector<int> &indices, Font &font)
{
    vertices.clear();
    indices.clear();

    int indexOffset = 0;
    float advance = 0;
    float lineLetterCount = 0;
    float lineOffset = 1.5f;
    float letterSpace = 0.25f;
    float lines = 0;

    for(int i = 0; string[i]; i++)
    {
        if(string[i] == ' ' || !font.Exists(string[i])) 
        {
            lineLetterCount++;
            continue;
        }
        else if(string[i] == '\n')
        {
            advance = 0;
            lineLetterCount = 0;
            lines++;
            continue;
        }

        Glyph glyph = font.Get(string[i]);

        for(const Vector2 &vertex : glyph.vertices)
        {
            vertices.emplace_back(vertex + Vector2((letterSpace + advance) * lineLetterCount, lines * lineOffset));
        }

        for(const int &index : glyph.indices)
        {
            indices.emplace_back(index + indexOffset);
        }

        indexOffset += glyph.vertices.size();
        advance = glyph.advance;
        lineLetterCount++;
    }
}

class Text
{
private:
    Font font;

    std::vector<Vector2> vertices;
    std::vector<Vector2> transformedVertices;
    std::vector<int> indices;
    std::string currentString;

    Transform transform;
    Box boundingBox;

    int fontWeight;

    Color fontColor;

public:
    Text() {}
    Text(float x, float y);

    void Draw(Screen &screen);
    Box GetBoundingBox();

    Transform &Transform();

    void setFont(const Font &font);
    void setString(const std::string &string);
    void setColor(Color color);
    void setFontWeight(int weight);
    void setFontSize(float size);
    std::string getCurrentString();
};

Text::Text(float x, float y)
{
    this->transform.position = Vector2(x, y);
    this->transform.scale = Vector2(5, 5);
    this->fontWeight = 1;
    
    this->fontColor = Black;
}

void Text::setString(const std::string &string)
{
    CreateTextVertices(string, vertices, indices, font);
    currentString = string;
    transform.update = true;
}

void Text::Draw(Screen &screen)
{
    if(transform.update)
    {
        transformedVertices = UpdateVertices(transform, vertices);
        boundingBox = UpdatePolygonBoundingBox(transformedVertices);
        transform.update = false;
    }

    if(transformedVertices.empty()) return;

    if(fontColor == Transparent) return;

    for(int i = 0; i < indices.size() - 1; i += 2)
    {   
        Vector2 a = this->transformedVertices[indices[i]];
        Vector2 b = this->transformedVertices[indices[i + 1]];

        if(fontWeight > 1)
            DrawThickLine(screen, a.x, a.y, b.x, b.y, fontWeight, fontColor);
        else
            DrawLine(screen, a.x, a.y, b.x, b.y, fontColor);
    }
}   

Box Text::GetBoundingBox()
{
    if(transform.update)
    {
        transformedVertices = UpdateVertices(transform, vertices);
        boundingBox = UpdatePolygonBoundingBox(transformedVertices);
        transform.update = false;
    }

    return this->boundingBox;
}

Transform &Text::Transform()
{
    if(transform.update)
        transformedVertices = UpdateVertices(transform, vertices);
    transform.update = false;

    return this->transform;
}

void Text::setFont(const Font &font)
{
    this->font = font;
}

void Text::setColor(Color color)
{
    this->fontColor = color;
}

void Text::setFontWeight(int weight)
{
    this->fontWeight = weight;
}

void Text::setFontSize(float size)
{
    this->transform.ScaleTo({size, size});
}

std::string Text::getCurrentString()
{
    return this->currentString;
}