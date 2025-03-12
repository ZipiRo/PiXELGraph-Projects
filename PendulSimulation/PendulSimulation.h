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
        
        SetTimeScale(2);

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
    int currentTimeScale = 1;
    Vector2 screenMousePosition;

    Text text1;

    GravityPendulum gp1, gp2, gp3;

    void OnStart() override
    {
        screenBorder = GetScreenBounds();
        screenWidth = GetScreenWidth();
        screenHeight = GetScreenHeight();

        font = Font("Resources/basic.f2p");
        text1 = Text(2, 2);
        text1.SetFont(font);
        text1.SetColor(Color::White);

        gp1 = GravityPendulum(Vector2(screenWidth / 2, screenHeight / 2), 70, 10, -1);
        gp2 = GravityPendulum(Vector2(screenWidth / 2, screenHeight / 2), 60, 50, -1);
        gp3 = GravityPendulum(Vector2(screenWidth / 2, screenHeight / 2), 50, 30, -1, true);
    }

    void OnUpdate(float deltaTime) override
    {
        frameTimer += deltaTime;
        if(frameTimer >= 1)
        {
            text1.SetString(std::to_string(int(1.0 / deltaTime)) + "fps");
            frameTimer = 0;
        }

        gp1.update(deltaTime);
        gp2.update(deltaTime, gp1.position);
        gp3.update(deltaTime, gp2.position);
    }

    void OnDraw(Screen &screen) override
    {   
        text1.Draw(screen);

        gp1.draw(screen);
        gp2.draw(screen);
        gp3.draw(screen);

        DrawOpenLines(screen, gp3.points, Color::White);
    }

    void OnQuit() override
    {

    }
};