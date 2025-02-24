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
    //  Slow Motion
    //  Player Speed Boost
    //  Player Size Boost
    /// 
    //Pause Menu
    ///

private:
    ObjectSpawner spawner;

    Player player;
    int screenWidth;
    int screenHeight;
    int highScore;
    Box screenBorder;
    
    Font font;
    std::vector<Text> ui_Texts;
    std::string saveFileName = "DodgerSaveFile";

    int LoadSaveFile()
    {
        std::ifstream saveFile(saveFileName, std::ios::binary);
        if(!saveFile)
        {
            CreateSaveFile();
            return 0;
        }
        
        int data;
        saveFile.read(reinterpret_cast<char*>(&data), sizeof(data));
        saveFile.close();

        return data;
    }

    void CreateSaveFile(int highScore = 0)
    {
        std::ofstream saveFile(saveFileName, std::ios::binary);
        if(!saveFile)
        {
            std::system(strcat("touch >> ", saveFileName.c_str()));
            saveFile.open(saveFileName, std::ios::binary);
        }

        saveFile.write(reinterpret_cast<const char*>(&highScore), sizeof(highScore));
        saveFile.close();
    }

    void InitUIText()
    {
        Text score_Text(5, 2);

        ui_Texts.push_back(score_Text);

        Text highScore_Text(5, screenHeight - 10);

        ui_Texts.push_back(highScore_Text);

        for(auto &text : ui_Texts)
            text.SetFont(font);

        ui_Texts[0].SetString("SCORE:0");
        ui_Texts[1].SetString("HIGHSCORE:" + std::to_string(highScore));
    }

    void OnStart() override
    {
        highScore = LoadSaveFile();
        font = Font("Resources/basic.f2p");
        
        srand(time(NULL));

        screenBorder = GetScreenBounds();
        screenWidth = GetScreenWidth();
        screenHeight = GetScreenHeight();
        InitUIText();

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
        CreateSaveFile(highScore < spawner.params.destroyedObjectsCount ? spawner.params.destroyedObjectsCount : highScore); // if the score is bigger then the highScore than write score
    }
};