#pragma once

#include <vector>

#include "Color.h"
#include "Graphics/Vertex.h"
#include "Graphics/Shape.h"

class Screen
{
private:
    Screen() {}

    friend class PiXELGraph;

    static int ScreenWidth, ScreenHeight;
    std::vector<Color> ScreenBuffer;

    static View viewPort;

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
void DrawElipse(int x, int y, int radius, Color color, int point_count = 30);
void DrawVertex(Vertex vertex);

void DrawLines(const std::vector<Vertex> &vertices, bool closed = true);
void DrawShape(const Shape &shape);

void Fill(const std::vector<Vertex> &vertices, const AABB& boundingBox, Color color);