#include "Core/PiXELGraph.h"

std::vector<Scene> scenes;

class Demo : public Scene
{
private:
    Vector2 ScreenMousePosition;
    Vector2 WorldMousePosition;

    void Start() override
    {
        Debug::Log("Demo 1");
    }

    void Event() override 
    {
    }

    void Update() override
    {
    }

    void Draw() override
    {        
    }

    void Quit() override
    {
        
    }

public:
    Demo() {}
};

class Demo1 : public Scene
{
private:
    void Start() override
    {
        Debug::Log("Demo 2");
    }

    void Event() override 
    {
    }

    void Update() override
    {
    }

    void Draw() override
    {        
    }

    void Quit() override
    {   
    }

public:
    Demo1() {}
};


class Engine : public PiXELGraph
{
public:
    Engine()
    {
        Init(1280, 720, 1, L"Game");

        SceneManager::AddScene<Demo>();
    }
};

int main()
{   
    Engine engine;
    engine.Run();

    return 0;
}