#pragma once

#include <vector>

#include "Color.h"
#include "Graphics/Vertex.h"
#include "Graphics/Shape.h"
#include "Graphics/Text.h"

class Screen
{
private:
    Screen() {}

    friend class PiXELGraph;

    static int ScreenWidth, ScreenHeight;
    std::vector<Color> ScreenBuffer;
    std::vector<Color> PreviosBuffer;
    View viewPort;

    void SetParameters(int WindowWidth, int WindowHeight);
    static void Display();
    static void Clear();

public:
    Screen(const Screen &) = delete;
    void operator=(const Screen &) = delete;

    static Screen &GetInstance();

    static void SetView(const View& view);
    static View& GetView();

    friend void PlotPixel(int, int, Color);

    static int Width();
    static int Height();
    static Color BacgroundColor;
};

void PlotPixel(int x, int y, Color color);

void DrawLine(int x1, int y1, int x2, int y2, Color color);
void DrawRectangle(int x, int y, int width, int height, Color color);
void FillRectangle(int x, int y, int width, int height, Color color);
void DrawElipse(int x, int y, int width, int height, Color color, int point_count = 30);
void FillElipse(int x, int y, int width, int height, Color color, int point_count = 30);

void DrawLines(const std::vector<Vertex> &vertices, bool closed = true);
void DrawShape(Shape &shape);
void DrawTEXT(Text &text);

void Fill(const std::vector<Vector2> &vertices, const AABB &boundingBox, const Color &color);
void FillShape(const std::vector<Vertex> &vertices, const AABB& boundingBox, Color color);