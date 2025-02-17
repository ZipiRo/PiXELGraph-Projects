#include "../../include/PixelGraph.h"
#include "../ParticleSystem.h"

class MenuScene : public PiXELGraph
{
public:
    MenuScene()
    {
        SetTitle(L"Dodger");
        SetScreenBackgroundColor(Color(223, 223, 244));

        Init(450, 550, 3);
    }

private:
    Font font;
    std::vector<Text> ui_Texts;
    ParticleSystem particles;
    
    Rectangle rectangleUI;

    Box screenBorder;
    int screenWidth;
    int screenHeight;

    float explosionTimer = 0;
    float colorTimer = 0;

    void InitUIText()
    {
        Text title((screenWidth / 2) - 44, 50);
        title.SetColor(Color(153, 43, 187));
        title.SetFontSize(7);
        title.SetFontWeight(3);
        title.GetTransform().Scale({5, 4.5});
        title.GetTransform().Rotate(-10 * DEG_TO_RAD);

        ui_Texts.push_back(title);

        Text textBox((screenWidth / 2) - 60, 140);
        textBox.SetFontSize(4);
        textBox.SetFontWeight(1);
        textBox.GetTransform().Scale({1, 1.5});

        ui_Texts.push_back(textBox);

        for(auto &text : ui_Texts)
            text.SetFont(font);

        ui_Texts[0].SetString("DODGER");
        ui_Texts[1].SetString("PRESS SPACE TO PLAY!");
    }

    void OnStart() override
    {
        srand(time(NULL));
        font = Font("Resources/basic.f2p");

        screenBorder = GetScreenBounds();
        screenWidth = GetScreenWidth();
        screenHeight = GetScreenHeight();
        
        particles = ParticleSystem(30, .7);

        rectangleUI = Rectangle(screenWidth / 2, screenHeight / 4 - 25, screenWidth + 200, screenHeight / 2 + screenHeight / 4);
        rectangleUI.SetFillColor(Color(33, 33, 33));
        rectangleUI.GetTransform().Rotate(-10 * DEG_TO_RAD);
        rectangleUI.SetPivot(Vector2(0.5, 0.5));

        InitUIText();
    }

    void OnUpdate(float deltaTime) override
    {
        colorTimer += deltaTime;
        explosionTimer += deltaTime;

        if(input.isKeyPressed(Keyboard::Key_Space))
            Quit();

        particles.Update(deltaTime, screenBorder);
        
        if(colorTimer >= .5)
        {
            ui_Texts[0].SetColor(RandomColor());
            colorTimer = 0;
        }

        if(explosionTimer >= .7)
        {
            int randCount = IRandomRange(1, 3);
            for(int i = 1; i <= randCount; i++)
                RandomExplosion();

            explosionTimer = 0;
        }
    }

    void RandomExplosion()
    {
        Vector2 randPosition(IRandomRange(0, screenWidth), IRandomRange(0, screenHeight));

        particles.SetPosition(randPosition);
        particles.RandomDirection(RandomColor());
    }

    void OnDraw(Screen &screen) override
    {   
        rectangleUI.Draw(screen);

        particles.Draw(screen);

        for(auto &text : ui_Texts)
            text.Draw(screen);
    }

    void OnQuit() override
    {

    }
};