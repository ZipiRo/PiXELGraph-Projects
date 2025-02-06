#include "../Include/PixelGraph.h"

int cellSize = 40;
int gameWidth = cellSize * 3;
int gameHeight = cellSize * 3;    

class Game : public PiXELGraph
{
public:
    Game()
    {
        this->backgroundColor = Color::White; 
        this->windowTitle = L"TicTacToe";
        this->timeScale = 1;
        this->FPS = 60;

        Init(gameWidth + 1, gameHeight + 15 + 1, 4); // +1 for centering
    }

private:
    Font font;

    bool gameRunning = true;
    bool cellIndex[4][4] = {0};
    int cellSign[4][4] = {0};
    int winnerSign = 0;
    bool PlayerTurn = 0;
    int celli, cellj;
    Text textBox;

    void DrawGameScene(Screen &screen)
    {
        Rectangle cell = Rectangle(0, 0, cellSize, cellSize);

        Elipse circle = Elipse(0, 0, (cellSize / 2) - 2, (cellSize / 2) - 2);
        circle.SetOutlineColor(Color::Blue);

        for(int i = 0; i < 3; i++)
        {
            for(int j = 0; j < 3; j++)
            {
                if(cellIndex[i][j])
                {
                    cell.SetFillColor(Color(235, 235, 235));
                }
                else
                {
                    cell.SetFillColor(Color::Transparent);
                }

                cell.GetTransform().MoveTo(Vector2(i * cellSize, j * cellSize));
                cell.Draw(screen);

                if(cellSign[i][j] == 1) // Draw O Sign
                {
                    circle.GetTransform().MoveTo(Vector2(i * cellSize + 2, j * cellSize + 2));
                    circle.Draw(screen);   
                }
                else if(cellSign[i][j] == 2) // Draw X Sign
                {
                    DrawLine(screen, i * cellSize + 3, j * cellSize + 3, i * cellSize + cellSize - 3, j * cellSize + cellSize - 3, Color::Red);
                    DrawLine(screen, i * cellSize + cellSize - 3, j * cellSize + 3, i * cellSize + 3, j * cellSize + cellSize - 3, Color::Red);
                }

                if(winnerSign != 0)
                {
                    if(celli == -1 && cellj > -1)
                    {
                        float startY = (cellSize * cellj) + cellSize / 2;
                        DrawThickLine(screen, 3, startY, cellSize * 3 - 3, startY, 2, Color::Black);
                    }

                    if(celli > -1 && cellj == -1)
                    {
                        float startX = (cellSize * celli) + cellSize / 2;
                        DrawThickLine(screen, startX, 3, startX, cellSize * 3 - 3, 2, Color::Black);
                    }

                    if(celli == 4)
                    {
                        DrawThickLine(screen, 3, 3, cellSize * 3 - 3, cellSize * 3 - 3, 2, Color::Black);
                    }

                    if(celli == 5)
                    {
                        DrawThickLine(screen, cellSize * 3 - 3, 3, 3, cellSize * 3 - 3, 2, Color::Black);
                    }
                }
            }
        }

        textBox.Draw(screen);
    }

    bool IsMousePositionOnCell(Vector2 mousePosition, int i, int j)
    {   
        if(mousePosition.x < 0 || mousePosition.x > gameWidth || 
            mousePosition.y < 0 || mousePosition.y > gameHeight) return false;

        float cellLeft = i * cellSize;
        float cellRight = cellLeft + cellSize;
        float cellTop = j * cellSize;
        float cellBottom = cellTop + cellSize;

        return mousePosition.x >= cellLeft && mousePosition.x < cellRight &&
            mousePosition.y >= cellTop && mousePosition.y < cellBottom;
    }

    void FindMouseCellPosition(Vector2 mousePosition)
    {
        for(int i = 0; i < 3; i++)
        {
            for(int j = 0; j < 3; j++)
            {
                if(IsMousePositionOnCell(mousePosition, i, j))
                {
                    cellIndex[i][j] = true;
                    celli = i;
                    cellj = j;
                }
                else 
                {
                    cellIndex[i][j] = false;
                }
            }
        }
    }

    bool CheckWinnwer()
    {
        // Check rows and columns
        for (int i = 0; i < 3; i++)
        {
            if (cellSign[i][0] == cellSign[i][1] && cellSign[i][1] == cellSign[i][2] && cellSign[i][0] != 0)
            {
                winnerSign = cellSign[i][0];
                celli = i;
                cellj = -1;
                return true;
            }

            if (cellSign[0][i] == cellSign[1][i] && cellSign[1][i] == cellSign[2][i] && cellSign[0][i] != 0)
            {
                winnerSign = cellSign[0][i];
                cellj = i;
                celli = -1;
                return true;
            }
        }

        // Check diagonals
        if (cellSign[0][0] == cellSign[1][1] && cellSign[1][1] == cellSign[2][2] && cellSign[0][0] != 0)
        {
            winnerSign = cellSign[0][0];
            celli = 4;
            return true;
        }
        if (cellSign[0][2] == cellSign[1][1] && cellSign[1][1] == cellSign[2][0] && cellSign[0][2] != 0)
        {
            winnerSign = cellSign[0][2];
            celli = 5;
            return true;
        }

        return false;
    }

    bool CheckTie()
    {
        for(int i = 0; i < 3; i++)
        {
            for(int  j = 0; j < 3; j++)
            {
                if(cellSign[i][j] == 0)
                    return false;
            }
        }

        return true;
    }

    void ResetGame()
    {
        winnerSign = 0;
        PlayerTurn = 0;
        celli = 3;
        cellj = 3;
        for(int i = 0; i < 3; i++)
        {
            cellIndex[i][0] = cellIndex[i][1] = cellIndex[i][2] = false;
            cellSign[i][0] = cellSign[i][1] = cellSign[i][2] = 0;
        }

        gameRunning = true;
        textBox.SetString("X TURN");
        textBox.GetTransform().MoveTo(Vector2((gameWidth / 2) - (6 * 6) / 2, gameHeight + 5));
        textBox.SetColor(Color::Red);
    }

    void OnStart() override
    {
        font = Font("Resources/basic.f2p");
        textBox = Text((gameWidth / 2) - (6 * 6) / 2, gameHeight + 5);
        textBox.SetFont(font);

        textBox.SetString("X TURN");
        textBox.SetColor(Color::Red);
    }

    Vector2 mousePosition;
    float frameTimer = 1;
    void OnUpdate(float deltaTime) override
    {   
        frameTimer += deltaTime;
        if(frameTimer >= 1)
        {
            SetWindowTitle(windowTitle + L" | FPS: " + std::to_wstring(int (1 / deltaTime)) + L" | DT: " + std::to_wstring(deltaTime) + L" MS");
            frameTimer = 0;
        }

        mousePosition = ScreenMousePosition(Vector2(input.GetMousePositionX(), input.GetMousePositionY()));
        
        if(input.isKeyDown(Keyboard::Key_R)) ResetGame();
        if(input.isKeyDown(Keyboard::Key_Escape)) Quit();

        if(gameRunning)
        {
            FindMouseCellPosition(mousePosition);

            if(mousePosition.x > 0 && mousePosition.x < gameWidth && 
                mousePosition.y > 0 && mousePosition.y < gameHeight) 

            {
                if(input.isMouseButtonDown(Mouse::Left))
                {
                    if(PlayerTurn == 0 && cellSign[celli][cellj] == 0)
                    {
                        cellSign[celli][cellj] = 2;
                        textBox.SetString("0 TURN");
                        textBox.SetColor(Color::Blue);
                        PlayerTurn = 1;
                    }
                    else if(PlayerTurn == 1 && cellSign[celli][cellj] == 0)
                    {
                        cellSign[celli][cellj] = 1;
                        textBox.SetString("X TURN");
                        textBox.SetColor(Color::Red);
                        PlayerTurn = 0;
                    }
                }
            }

            if(CheckWinnwer())
            {
                if(winnerSign == 1)
                {
                    textBox.SetString("0 WINNER");
                    textBox.SetColor(Color::Blue);
                }
                else if(winnerSign == 2)
                {
                    textBox.SetString("X WINNER");
                    textBox.SetColor(Color::Red);
                }
                textBox.GetTransform().MoveTo(Vector2((gameWidth / 2) - (8 * 6) / 2, gameHeight + 5));

                gameRunning = false;
            } 
            else if(CheckTie())
            {
                textBox.SetString("DRAW");
                textBox.GetTransform().MoveTo(Vector2((gameWidth / 2) - (4 * 6) / 2, gameHeight + 5));
                textBox.SetColor(Color::Black);
                gameRunning = false;
            }
        }   
    }

    void OnDraw(Screen &screen) override
    {
        DrawGameScene(screen);
    }

    void OnQuit() override
    {

    }
};