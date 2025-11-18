#pragma once

#include <string>
#include <vector>
#include <map>

#include "Vertex.h"

struct Character
{
    std::vector<Vertex> vertices;
    std::vector<int> indices;
    float devance;
    float advance;
};

class Font
{
private:
    static std::map<char, Character> characters;
    
    Font();
public:
    Font(const Font &) = delete;
    void operator=(const Font &) = delete;

    static Font &GetInstance();
    static Character GetC(char character);
};