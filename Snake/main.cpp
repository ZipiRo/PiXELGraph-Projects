#include "Include/PixelGraph.h"

#include "Snake.h"

const int TILE_SIZE = 3;
const int COLS = 60;
const int ROWS = 40;
const int WORLD_WIDTH = COLS * TILE_SIZE;
const int WORLD_HEIGHT = ROWS * TILE_SIZE;
const int FPS = 60;

void DrawGround(Screen &screen, Color colorOne, Color colorTwo)
{
    Rectangle Tile(0, 0, TILE_SIZE, TILE_SIZE);
    Tile.SetPivot({0, 0});
    
    for(int i = 3; i < ROWS; i++)
    {
        for(int j = 0; j < COLS; j++)
        {
            Tile.SetFillColor((i + j) % 2 == 0 ? colorOne : colorTwo);
            Tile.SetOutlineColor((i + j) % 2 == 0 ? colorOne : colorTwo);
            Tile.GetTransform().MoveTo(Vector2(j * TILE_SIZE, i * TILE_SIZE));
            Tile.Draw(screen);
        }
    }
}

Screen screen(WORLD_WIDTH, WORLD_HEIGHT);
Timer timer;

int main()
{
    Font font("Resources/basic.f2p");  
    Box boundry{0, TILE_SIZE * 3, WORLD_WIDTH - 1, WORLD_HEIGHT - 1};

    Snake snake(ROWS / 2, COLS / 2, TILE_SIZE, boundry);
    snake.SetColors(IntenseYellow, Black);
    
    float fps_timer;
    Text fps(1, 1);
    fps.setFontSize(5);
    fps.setFont(font);

    float time = 0;
    bool keyPressed = false;

    int curentKey;
    bool running = true;
    while (running)
    {
        if (_kbhit())
			curentKey = _getch();

		if (curentKey == Key::SPACE) CLEAR_CONSOLE
		if (curentKey == Key::ESCAPE) running = false;

        curentKey = 0;

        timer.Tick();
        if(timer.DeltaTime() >= 1.0f / FPS)
        {
            timer.Reset();
            time += timer.DeltaTime();
            fps_timer += timer.DeltaTime();

            if(fps_timer >= 1)
            {
                fps.setString("POINTS: " + std::to_string(int(1.0 / timer.DeltaTime())));
                fps_timer = 0;
            }

            if(!keyPressed)
                    keyPressed = snake.Control(Key::ARROW_UP, Key::ARROW_DOWN, Key::ARROW_RIGHT, Key::ARROW_LEFT);
                
            if(time >= .2f)
            {
                snake.Update();
                keyPressed = false;
                time = 0;
            }
        }

        screen.Clear(IntenseBlack);
        
        DrawGround(screen, Green, IntenseGreen);

        snake.Draw(screen);

        fps.Draw(screen);

        screen.Display();
    }

    return 0;
}