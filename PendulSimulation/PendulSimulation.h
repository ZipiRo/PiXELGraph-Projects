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

        Init(1240, 720, 2);
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
    Vector2 screenMousePosition;

    Text text1;

    DoublePendulum dp1;

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
        float screenYMid = screenHeight / 2;
        
        dp1 = DoublePendulum({screenXMid, screenYMid}, 5, 3, 70, 50, PI, PI - 0.05);
        dp1.addPoints = true;
    }

    void OnUpdate(float deltaTime) override
    {
        frameTimer += deltaTime;
        if(frameTimer >= 1)
        {
            text1.SetString(std::to_string(int(1.0 / deltaTime)) + "fps");
            frameTimer = 0;
        }

        dp1.update(deltaTime);
    }

    void OnDraw(Screen &screen) override
    {   
        text1.Draw(screen);
        
        dp1.draw(screen);

        DrawOpenLines(screen, dp1.points, Color::Red);
    }

    void OnQuit() override
    {

    }
};