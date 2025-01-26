#include "Include/PixelGraph.h"

#define STB_IMAGE_IMPLEMENTATION
#define STBI_NO_SIMD

#include "stb_image.h"

struct Point
{
    Vector2 position;
    Color color;
};

class Game : public PiXELGraph
{
public:
    Game()
    {
        this->backgroundColor = Color(255, 255, 255); 
        this->windowTitle = L"PngVis v1.0";
        this->timeScale = 0.1;
        this->FPS = 60;

        Init(1240 / 2, 720 / 2); 
    }

private:
    std::vector<Point> image;

    void OnStart() override
    {
        Point point;
        int width, height, channels;
        unsigned char *data = stbi_load("image.png", &width, &height, &channels, 0);

        if(!data)
        {
            std::cerr << "Failed to load image" << std::endl;
            return;
        }

        for (int y = 0; y < height; y++)
        {
            for (int x = 0; x < width; x++)
            {
                int index = (y * width + x) * channels;
                point.position = Vector2(x, y);
                point.color.r = data[index];
                point.color.g = data[index + 1];
                point.color.b = data[index + 2];
                image.push_back(point);
            }
        }

        stbi_image_free(data);
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
        for (auto &point : image)
        {
            screen.PlotPixel(point.position.x, point.position.y, point.color);
        }
    }

    void OnQuit() override
    {
        // DestroyThings
    }
};