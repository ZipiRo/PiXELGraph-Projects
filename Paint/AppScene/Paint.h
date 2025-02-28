#include "../../include/PixelGraph.h"

class PaintScene : public PiXELGraph
{
public:
    PaintScene()
    {
        SetTitle(L"Paint");
        SetScreenBackgroundColor(Color::White);

        Init(500, 500, 2);
    }

private:
    struct Doodle
    {
        std::vector<Vector2> points;
        Color color;
        int strokeSize;
    };

    Box screenBorder;
    int screenWidth;
    int screenHeight;

    int countDodles = 0;
    bool isPainting = false;
    int strokeSize = 3;
    Color curentColor = Color::Black;

    std::vector<Doodle> doodles;

    Vector2 screenMousePosition;

    void OnStart() override
    {
        screenBorder = GetScreenBounds();
        screenWidth = GetScreenWidth();
        screenHeight = GetScreenHeight();

        // Initialize the doodles vector with one empty Doodle
        doodles.push_back(Doodle());
    }

    void OnUpdate(float deltaTime) override
    {
        screenMousePosition = ScreenMousePosition(Vector2(input.GetMousePositionX(), input.GetMousePositionY()));

        if(input.isKeyDown(Keyboard::Key_1)) curentColor = Color::Red;
        if(input.isKeyDown(Keyboard::Key_2)) curentColor = Color::Green;
        if(input.isKeyDown(Keyboard::Key_3)) curentColor = Color::Black;
        if(input.isKeyDown(Keyboard::Key_4)) curentColor = Color::Yellow;
        if(input.isKeyDown(Keyboard::Key_5)) curentColor = Color::Purple;
        if(input.isKeyDown(Keyboard::Key_6)) curentColor = Color::LightPurple;
        if(input.isKeyDown(Keyboard::Key_7)) curentColor = Color::White;

        if(input.isKeyDown(Keyboard::Key_LeftControl))
        {
            if(input.isKeyPressed(Keyboard::Key_UpArrow))
            {
                strokeSize--;
                if(strokeSize < 1)
                    strokeSize = 1;
            }
            else if(input.isKeyPressed(Keyboard::Key_DownArrow))
            {
                strokeSize++;
                if(strokeSize > 10)
                    strokeSize = 10;
            }

            if(input.isKeyPressed(Keyboard::Key_Z))
            {
                doodles[countDodles].points.clear(); 
                if(countDodles > 1)
                    countDodles--;
            }
        }

        if(input.isMouseButtonDown(Mouse::Left))
        {
            if (!isPainting)
            {
                // Start a new doodle
                doodles.push_back(Doodle());
                countDodles++;
                isPainting = true;
            }

            doodles[countDodles].points.push_back(screenMousePosition);
            doodles[countDodles].color = curentColor;
            doodles[countDodles].strokeSize = strokeSize;
        }
        else if (isPainting)
        {
            isPainting = false;
        }
    }

    void OnDraw(Screen &screen) override
    {   
        for(int i = 0; i <= countDodles; i++)
        {
            std::vector<Vector2> points = doodles[i].points;
            Color color = doodles[i].color;
            int strokeSize = doodles[i].strokeSize;

            for(auto point = points.begin(); point != points.end(); ++point)
            {
                auto next_point = std::next(point);
                if(next_point == points.end()) break;
                
                Vector2 A = *point;
                Vector2 B = *next_point;

                DrawThickLine(screen, A.x, A.y, B.x, B.y, strokeSize, color);
            }
        }
    }

    void OnQuit() override
    {

    }
};