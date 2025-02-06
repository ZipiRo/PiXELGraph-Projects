#include "../Include/PixelGraph.h"

class Game : public PiXELGraph
{
public:
    Game()
    {
        this->backgroundColor = Color::White; 
        this->windowTitle = L"Sudoku";
        this->timeScale = 1;
        this->FPS = 60;

        Init(100, 100, 4);
    }

private:
    void OnStart() override
    {

    }

    float frameTimer = 1;
    void OnUpdate(float deltaTime) override
    {   
        frameTimer += deltaTime;
        if(frameTimer >= 1)
        {
            SetWindowTitle(windowTitle + L" | FPS: " + std::to_wstring(int (1 / deltaTime)) + L" | DT: " + std::to_wstring(deltaTime) + L" MS");
            frameTimer = 0;
        }
    }

    void OnDraw(Screen &screen) override
    {

    }

    void OnQuit() override
    {

    }
};