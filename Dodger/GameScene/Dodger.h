#include "../../include/PixelGraph.h"
#include "../ParticleSystem.h"

#include "Player.h"
#include "FallingObject.h"
#include "ObjectSpawner.h"

class Dodger : public PiXELGraph
{
public:
    Dodger()
    {
        SetTitle(L"Dodger");
        SetScreenBackgroundColor(Color(223, 223, 244));

        Init(450, 550, 3);
    }

    ///
    //TO DO: Player PowerUps 
    //Slow Motion
    //Player Speed Boost
    //Player Size Boost
    /// 

private:
    ObjectSpawner spawner;

    Player player;
    int screenWidth;
    int screenHeight;

    Box screenBorder;
    
    Font font;
    std::vector<Text> ui_Texts;

    void InitUIText()
    {
        Text score_Text(5, 2);

        ui_Texts.push_back(score_Text);

        for(auto &text : ui_Texts)
            text.SetFont(font);

        ui_Texts[0].SetString("SCORE:0");
    }

    void OnStart() override
    {
        srand(time(NULL));

        font = Font("Resources/basic.f2p");
        InitUIText();

        screenBorder = GetScreenBounds();
        screenWidth = GetScreenWidth();
        screenHeight = GetScreenHeight();

        spawner = ObjectSpawner(screenWidth, screenHeight);
        player = Player(screenWidth / 2, screenHeight - 20, 25, 5);
        player.SetMoveingBorder(0, screenWidth);

        player.speed = 200;
    }

    void OnUpdate(float deltaTime) override
    {
        if(input.isKeyPressed(Keyboard::Key_Escape))
            Quit();

        if(input.isKeyDown(Keyboard::Key_RightArrow) || input.isKeyDown(Keyboard::Key_D))
            player.Move(1 * deltaTime);

        if(input.isKeyDown(Keyboard::Key_LeftArrow) || input.isKeyDown(Keyboard::Key_A))
            player.Move(-1 * deltaTime);

        player.Update(deltaTime);
        spawner.Update(deltaTime, player.GetShapeBox());
        spawner.particleSystem.Update(deltaTime, screenBorder);

        if(spawner.params.anyDstroyedObject)
        {
            player.SetFillColor(spawner.params.destroyedObject_color);
            spawner.params.anyDstroyedObject = false;

            ui_Texts[0].SetString("SCORE:" + std::to_string(spawner.params.destroyedObjectsCount));
        }
    }

    void OnDraw(Screen &screen) override
    {
        player.Draw(screen);
        spawner.Draw(screen);
        spawner.particleSystem.Draw(screen);

        for(auto &text : ui_Texts)
            text.Draw(screen);
    }

    void OnQuit() override
    {

    }
};