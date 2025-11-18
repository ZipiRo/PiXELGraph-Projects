#include "Console/Debug.h"
#include "Graphics/Font.h"
#include "Graphics/Text.h"
#include "Graphics/Shape.h"

void CreateTextVertices(const std::string &text, std::vector<Vertex> &vertices, std::vector<int> &indices)
{
    vertices.clear();
    indices.clear();

    int linesCount = 0;
    int lineLetterCount = 0;
    int indexOffset = 0;
    float space = 0;

    const float LineSpace = 0.50f;
    const float LetterSpace = 0.50f;
     
    for(const auto &letter : text)
    {
        if(letter == ' ')
        {
            space += LetterSpace;
            continue;
        }

        if(letter == '\n')
        {
            lineLetterCount = 0;
            space = 0;
            linesCount++;
            continue;
        }

        Character character = Font::GetC(letter);
        space += character.devance;
        
        for(const auto &vertex : character.vertices)
        {
            Vector2 position = vertex.position;
            if(lineLetterCount != 0)
                position.x += character.devance;

            vertices.emplace_back(position + Vector2(lineLetterCount + space, linesCount * (LineSpace + 1)));
        }

        for(const auto &index : character.indices)
            indices.emplace_back(index + indexOffset);

        indexOffset += character.vertices.size();
        lineLetterCount++;
    }
}

Text::Text()
{
    this->color = Color::Transparent;
    this->fillColor = Color::Transparent;
}

std::vector<Vertex> Text::GetTvertices()
{
    if (transform.update)
    {
        Tvertices = UpdateVertices(transform, vertices);
        transform.update = false;
    }

    return Tvertices;
}

AABB Text::GetBoundingBox()
{
    if (transform.update)
    {
        Tvertices = UpdateVertices(transform, vertices);
        boundingBox = UpdateAABB(Tvertices);
        transform.update = false;
    }

    return boundingBox;
}

void Text::SetString(const std::string &text)
{
    this->string = text;
    CreateTextVertices(this->string, vertices, indices);
    
    for (auto &vertex : vertices)
        vertex.color = this->color;
    
    transform.update = true;
}

void Text::SetColor(Color color)
{
    this->color = color;
    for (auto &vertex : vertices)
        vertex.color = this->color;
}