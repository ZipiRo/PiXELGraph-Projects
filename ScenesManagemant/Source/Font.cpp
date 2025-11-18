#include <filesystem>
#include <string>
#include <cstring>
#include <map>

#include <fstream>

#include "nlohmann/json.hpp"

#include "Console/ErrorSystem.h"
#include "Console/Debug.h"

#include "Graphics/Font.h"

using json = nlohmann::json;

std::map<char, Character> CreateFontTable(const std::string &fontFile)
{
    std::map<char, Character> characters;

    if(!std::filesystem::exists(fontFile))
        throw Error("Font file was not found: " + fontFile);

    if(strstr(fontFile.c_str(), ".cf") == NULL)
        throw Error("File exstension is not correct");

    std::ifstream file(fontFile);

    json FontData;
    file >> FontData;

    if(!FontData.contains("characters"))
        throw Error("Invalid font file format");

    for(const auto &entry : FontData["characters"].items())
    {
        char charValue = entry.key()[0];
        const auto& characterData = entry.value();
        
        Character character;

        for(const auto &vertex : characterData["vertices"])
            // vertex[0] (x) vertex[1] (y)
            character.vertices.emplace_back(Vector2(vertex[0], vertex[1]));
    
        for(const auto &index : characterData["indices"])
            character.indices.emplace_back(index);

        character.devance = characterData["devance"];
        // character.advance = characterData["advance"];

        characters[charValue] = character;
    }

    return characters;
}

std::map<char, Character> Font::characters;

Font::Font()
{
    characters = CreateFontTable("font.cf");
}

Font &Font::GetInstance()
{
    static Font instance;
    return instance;
}

Character Font::GetC(char character)
{
    return characters[character];
}