#include "../include/PixelGraph.h"
#include "Pendulums.h"

class Simulation : public PiXELGraph
{
public:
    Simulation()
    {
        SetTitle(L"Pendulums");
        SetScreenBackgroundColor(Color::Black);
        SetMaxFPS(9999);

        Init(1240, 720, 3);
    }

private:

    void DrawOpenLines(Screen &screen, const std::vector<Vector2> &vertices, Color color, int thickness = 1)
    {
        for(auto vertex = vertices.begin(); vertex != vertices.end(); ++vertex)
        {
            auto next_vertex = std::next(vertex);
            if(next_vertex == vertices.end())
                break;
            
            Vector2 vertexA = *vertex;
            Vector2 vertexB = *next_vertex;
            if(thickness > 1) DrawThickLine(screen, vertexA.x, vertexA.y, vertexB.x, vertexB.y, thickness, color);
                else DrawLine(screen, vertexA.x, vertexA.y, vertexB.x, vertexB.y, color);
        }
    }

    Font font;
    Box screenBorder;
    int screenWidth;
    int screenHeight;
    float frameTimer = 1;
    int currentTimeScale = 1;
    Vector2 screenMousePosition;

    Text text1;

    GravityPendulum gp1;
    ConstantPendul cp1;
    CompoundPendulum cmp1;

    void OnStart() override
    {
        screenBorder = GetScreenBounds();
        screenWidth = GetScreenWidth();
        screenHeight = GetScreenHeight();

        font = Font("Resources/basic.f2p");
        text1 = Text(2, 2);
        text1.SetFont(font);
        text1.SetColor(Color::White);

        float screenXMid = screenWidth / 2;
        float screenYMid = screenHeight / 4;

        cp1 = ConstantPendul(3, PI / 3, Vector2(screenXMid / 2, screenYMid));
        gp1 = GravityPendulum(Vector2(screenXMid, screenYMid), 3, 3, -PI / 6, 0.5);
        cmp1 = CompoundPendulum(Vector2(screenWidth - screenXMid / 2, screenYMid), 3, 3, 0.6);

        cp1.addPoints = true;
        gp1.addPoints = true;
        cmp1.addPoints = true;
    }

    void OnUpdate(float deltaTime) override
    {
        frameTimer += deltaTime;
        if(frameTimer >= 1)
        {
            text1.SetString(std::to_string(int(1.0 / deltaTime)) + "fps");
            frameTimer = 0;
        }

        cp1.update(deltaTime);
        gp1.update(deltaTime);
        cmp1.update(deltaTime);
    }

    void OnDraw(Screen &screen) override
    {   
        text1.Draw(screen);

        cp1.draw(screen);
        gp1.draw(screen);
        cmp1.draw(screen);

        DrawOpenLines(screen, cp1.points, Color::Red);
        DrawOpenLines(screen, gp1.points, Color::Green);
        DrawOpenLines(screen, cmp1.points, Color::Blue);

        screen.PlotPixel(cp1.points.back().x, cp1.points.back().y, Color::Black);
        screen.PlotPixel(gp1.points.back().x, gp1.points.back().y, Color::Black);
        screen.PlotPixel(cmp1.points.back().x, cmp1.points.back().y, Color::Black);
    }

    void OnQuit() override
    {

    }
};