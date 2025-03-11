#include "../include/PixelGraph.h"

class PaintScene : public PiXELGraph
{
public:
    PaintScene()
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

    class Pendul
    {
    private:
        Elipse e;
        float radius;
        float it_angle;
        float angle = 0;
        Vector2 anchor;


    public:
        bool addPoints;
        std::vector<Vector2> points;
        Vector2 position = Vector2::ZERO;
    
        Pendul() {}

        Pendul(float radius, float it_angle, Vector2 position, bool addPoints = false)
        {
            this->position = Vector2::ZERO;
            this->radius = radius;
            this->it_angle = it_angle;
            this->anchor = position;
            this->addPoints = addPoints;

            e = Elipse(0, 0, 3, 3, 15);

            // Color randColor = RandomColor();
            Color randColor = Color::White;

            e.SetFillColor(randColor);
            e.SetOutlineColor(randColor);
            e.SetPivot({0.5, 0.5});
        }

        void update(float time)
        {
            angle += it_angle * time;
            // if(angle >= 2 * PI && addPoints)
            // {
            //     addPoints = false;
            //     points.push_back(points[0]);
            // }

            angle = NormalizeAngle(angle);

            float x = cos(angle) * radius;
            float y = sin(angle) * radius;

            position = Vector2(x, y) + anchor;

            if(addPoints)
                points.push_back(position);
        }

        void update(float time, Vector2 anchor)
        {
            this->anchor = anchor;
            update(time);
        }

        void draw(Screen &screen)
        {
            e.GetTransform().MoveTo(position);
            DrawLine(screen, anchor.x, anchor.y, position.x, position.y, Color::White);

            e.Draw(screen);
        }
    };

    Font font;
    Box screenBorder;
    int screenWidth;
    int screenHeight;
    float frameTimer = 1;
    int currentTimeScale = 1;
    Vector2 screenMousePosition;

    Text text1;
    Text text2;

    std::vector<Pendul> ps;
    int n = 5;

    void OnStart() override
    {
        screenBorder = GetScreenBounds();
        screenWidth = GetScreenWidth();
        screenHeight = GetScreenHeight();

        font = Font("Resources/basic.f2p");
        text1 = Text(2, 2);
        text1.SetFont(font);
        text1.SetColor(Color::White);

        text2 = Text(2, screenHeight - 7);
        text2.SetFont(font);
        text2.SetColor(Color::White);

        float c_radius = 100;
        float c_it_angle = PI / 6;

        float m_radius = c_radius / n;
        float m_it_angle = c_it_angle / n;

        Pendul p(c_radius, c_it_angle, Vector2(screenWidth / 2, screenHeight / 2));
        ps.push_back(p);

        for(int i = 1; i < n; i++)
        {
            c_radius -= m_radius;
            c_it_angle += m_it_angle;

            Pendul p(c_radius, c_it_angle, Vector2(screenWidth / 2, screenHeight / 2));
            ps.push_back(p);
        }

        // ps[n - 1].addPoints = true;
    }

    void OnUpdate(float deltaTime) override
    {
        frameTimer += deltaTime;
        if(frameTimer >= 1)
        {
            text1.SetString(std::to_string(int(1.0 / deltaTime)) + "fps");
            frameTimer = 0;
        }
        text2.SetString(std::to_string(currentTimeScale));

        if(input.isKeyPressed(Keyboard::Key_UpArrow))
        {
            currentTimeScale++;
            if(currentTimeScale > 5)
                currentTimeScale = 5;

            SetTimeScale(currentTimeScale);
        }

        if(input.isKeyPressed(Keyboard::Key_DownArrow))
        {
            currentTimeScale--;
            if(currentTimeScale < 1)
                currentTimeScale = 1;

            SetTimeScale(currentTimeScale);
        }

        ps[0].update(deltaTime);
        for(int i = 1; i < n; i++)
            ps[i].update(deltaTime, ps[i - 1].position);
    }

    void OnDraw(Screen &screen) override
    {   
        for(auto &p : ps)
            p.draw(screen);

        // DrawOpenLines(screen, ps[n - 1].points, Color::White);

        text1.Draw(screen);
        text2.Draw(screen);
    }

    void OnQuit() override
    {

    }
};