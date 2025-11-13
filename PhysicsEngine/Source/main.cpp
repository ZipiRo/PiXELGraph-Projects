#include "Core/PiXELGraph.h"

#include <list>

#include "Physics/PTransform.h"
#include "Physics/VectorMath.h"
#include "Physics/Body.h"
#include "Physics/Manifold.h"
#include "Physics/Box.h"
#include "Physics/Circle.h"
#include "Physics/Collisions.h"
#include "Physics/World.h"

class PhysicsEngine : public PiXELGraph
{
private:
    double timer = 1;

    World world;

    Vector2 ScreenMousePosition;
    Vector2 WorldMousePosition;

    void Start() override
    {
        Screen::GetView().SetScreenCenter(Vector2(Screen::Width() / 2, Screen::Height() / 2));
        Screen::GetView().SetZoom(.5);

        Body *Ground = new Box(300.0f, 20.0f, Vector2(0, 100), 1.0f, 0.5f, Color::Transparent, Color::Black, true);
        world.AddBody(Ground);

        Body *Boxs = new Circle(20.0f, Vector2(0, -100), 1.0f, 0.5f, Color::Transparent, Color::Blue, false);
        world.AddBody(Boxs);
    }

    void Update() override
    {
        ScreenMousePosition = Input::MousePosition / Window::WindowFontSize();
        WorldMousePosition = Screen::GetView().ScreenToWorld(ScreenMousePosition);

        timer += Time::deltaTime;
        if (timer >= 1)
        {
            Window::SetTitle(L" | FPS: " + std::to_wstring(int(1.0f / Time::deltaTime)) + L" | DEMO");
            Debug::Log(std::string("DT: ") + std::to_string(Time::deltaTime) + std::string(" | FPS: ") + std::to_string(int(1.0f / Time::deltaTime)));
            timer = 0;
        }

        world.Step(Time::deltaTime, 10);
    }

    void Draw() override
    {
        for(int i = 0; i < world.BodyCount(); i++)
        {
            Body *body = world.GetBody(i);
            body->Draw();
        }
    }

    void Quit() override
    {

    }

public:
    PhysicsEngine()
    {
        MaxFPS = 60;
        Init(1280, 720, 3, L"DEMO");
    }
};

int main()
{
    PhysicsEngine demo;
    demo.Run();

    return 0;
}