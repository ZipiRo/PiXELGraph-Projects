#include "../Include/PixelGraph.h"
#include "ColorPalletes.h"

struct mCell
{
    int row;
    int colon;
    int box;
};
mCell matrixCells[85];
int zlength = 0;
bool rBoxNumbers[15][10] = {0};
bool valid;
int matrix[12][12] = {
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 0, 2, 5, 6, 0, 0, 0, 0, 0},
    {1, 0, 0, 0, 1, 0, 0, 0, 0, 0},
    {1, 0, 0, 0, 0, 0, 0, 1, 0, 0},
    {1, 0, 0, 0, 0, 0, 0, 2, 0, 0},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {1, 0, 0, 0, 2, 0, 0, 0, 0, 0},
    {1, 0, 0, 0, 0, 1, 0, 0, 0, 0},
    {1, 0, 1, 0, 0, 0, 0, 0, 0, 0},
    {1, 0, 0, 0, 0, 0, 3, 0, 1, 0}
};
int trys = 1;

bool Posibil(int x, mCell cell)
{
    if(rBoxNumbers[cell.box][x]) return false;

    for(int i = 1; i <= 9; i++)
        if(matrix[cell.row][i] == x || matrix[i][cell.colon] == x) return false;

    return true;
}

void BackTracking(int k, int randT)
{
    for(int i = 1; i <= 9; i++)
    {
        if(!Posibil(i, matrixCells[k])) continue;
        
        matrix[matrixCells[k].row][matrixCells[k].colon] = i; // ocupy
        rBoxNumbers[matrixCells[k].box][i] = 1;

        if(k == zlength)
        {
            trys += 1;
            if(trys >= randT)
                valid = true;
        }
        else BackTracking(k + 1, randT);
        
        if(valid) return;
        matrix[matrixCells[k].row][matrixCells[k].colon] = 0; // free
        rBoxNumbers[matrixCells[k].box][i] = 0;
    }
}

struct Cell
{
    int number = 0;
    bool mouseOn = false;
    bool lookedNumber = false;
};

struct Sudoku
{
    Vector2 position;
    int cellSize;
    int length;
    std::vector<std::vector<Cell>> table;
    std::vector<int> usedNumbers;

    int mousei, mousej;
    int usingNumber = 1;
};

class Game : public PiXELGraph
{
public:
    Game()
    {
        SetTitle(L"Sudoku");
      
        Init(190 * 3, 190 * 3, 3);
    }

private:
    Font font;
    Vector2 mosusePosition;

    Text titleText;
    Color titleColor;

    Sudoku sudoku;
    Rectangle cell;
    Text cellNumber;
    
    Color cellColor;
    Color cellBorderColor;
    Color borderColor;
    Color numberColor;
    Color lnumberColor;

    Elipse rowCircle;
    Text rowNumber;

    Color rowNumberColor;
    Color rowCircleBorderColor;
    Color rowCircleColor;
    Color uRowCircleColor;
    Color fRowCircleColor;
    
    Color colorb;

    int emptyCells;
    bool completed = false;

    void CreateSudokuMatrix()
    {   
        int x = 1, y = 1;
        for(int i = 1; i <= 9; i++)
        {
            for(int j = 1; j <= 9; j++)
            {
                if(matrix[i][j] == 0) 
                {
                    zlength++;
                    matrixCells[zlength].row = i;
                    matrixCells[zlength].colon = j;
                    matrixCells[zlength].box = x;
                }
                
                rBoxNumbers[x][matrix[i][j]] = true;
                if(j % 3 == 0)
                    x++; // go to next sub 3x3 TO RIGHT
            }

            if(i % 3 == 0)
                y = x; // go to next sub 3x3 DOWN
            x = y;
        }

        srand(time(NULL));
        BackTracking(1, IRandomRange(10000, 50000));

        for(int i = 1; i <= 9; i++)
        {
            for(int j = 1; j <= 9; j++)
            {
                if(matrix[i][j] != 0)
                {
                    sudoku.table[j - 1][i - 1].number = matrix[i][j];
                    sudoku.table[j - 1][i - 1].lookedNumber = true;
                    sudoku.usedNumbers[matrix[i][j] - 1]++;
                }
            }
        }

        while(emptyCells)
        {
            int i = IRandomRange(0, 8);
            int j = IRandomRange(0, 8);
            
            if(sudoku.table[i][j].number != 0)
            {
                emptyCells--;
                
                sudoku.table[i][j].lookedNumber = false;
                sudoku.usedNumbers[sudoku.table[i][j].number - 1]--;
                sudoku.table[i][j].number = 0;
            }
        }
    }

    void DrawGameBoard(Screen &screen)
    {   
        titleText.GetTransform().MoveTo(Vector2(sudoku.position.x + 25, sudoku.position.y - 20));
        titleText.Draw(screen);

        Vector2 cellPosition;
        for(int i = 0; i < 9; i++)
        {
            for(int j = 0; j < 9; j++)
            {
                cellPosition = Vector2(sudoku.cellSize * i, sudoku.cellSize * j);
                cell.GetTransform().MoveTo(sudoku.position + cellPosition); // add the position for correct display

                // draw board
                if(sudoku.table[i][j].mouseOn) cell.SetFillColor(Color(cellColor.r - 20, cellColor.g - 20, cellColor.b - 20)); // if mouse on this i, j cell
                    else cell.SetFillColor(cellColor);
                
                cell.Draw(screen);

                // draw numbers
                if(sudoku.table[i][j].number != 0)
                {
                    if(sudoku.table[i][j].lookedNumber) cellNumber.SetColor(lnumberColor);
                        else cellNumber.SetColor(numberColor);
                    
                    cellNumber.SetString(std::to_string(sudoku.table[i][j].number));
                    cellNumber.GetTransform().MoveTo(sudoku.position + cellPosition + Vector2(5, 4));

                    cellNumber.Draw(screen);
                }
            }
        }

        int x = sudoku.position.x;
        int y = sudoku.position.y;
        int x1 = x + sudoku.length;
        int y1 = y + sudoku.length;

        // draw big square
        DrawLine(screen, x, y, x1, y, borderColor);
        
        DrawLine(screen, x1, y, x1, y1, borderColor);
        
        DrawLine(screen, x1, y1, x, y1, borderColor);
     
        DrawLine(screen, x, y1,  x, y, borderColor);

        // darw inside dividing lines
        x = sudoku.position.x + sudoku.cellSize * 3;
        DrawLine(screen, x, sudoku.position.y, x, sudoku.position.y + sudoku.length, borderColor);
        
        x = sudoku.position.x + sudoku.cellSize * 6;
        DrawLine(screen, x, sudoku.position.y, x, sudoku.position.y + sudoku.length, borderColor);
        
        y = sudoku.position.y + sudoku.cellSize * 3;
        DrawLine(screen, sudoku.position.x, y, sudoku.position.x + sudoku.length, y, borderColor);
     
        y = sudoku.position.y + sudoku.cellSize * 6;
        DrawLine(screen, sudoku.position.x, y, sudoku.position.x + sudoku.length, y, borderColor);
    }

    void DrawNumberRow(Screen &screen)
    {   
        Vector2 position;
        for(int i = 0; i < 9; i++)
        {   
            position = Vector2(sudoku.cellSize * i + 1, sudoku.length + 3);

            if(sudoku.usedNumbers[i] >= 9) rowCircle.SetFillColor(fRowCircleColor);
                else if(sudoku.usingNumber - 1 == i) rowCircle.SetFillColor(uRowCircleColor);
                else rowCircle.SetFillColor(rowCircleColor);

            rowCircle.GetTransform().MoveTo(sudoku.position + position);
            rowCircle.Draw(screen);
            
            rowNumber.GetTransform().MoveTo(sudoku.position + position + Vector2(4, 3));
            rowNumber.SetString(std::to_string(i + 1));
            rowNumber.Draw(screen);
        }
    }

    bool CompletedBoard()
    {
        for(int i = 0; i < 9; i++)
        {
            for(int j = 0; j < 9; j++)
            {
                if(sudoku.table[j][i].number != matrix[i + 1][j + 1]) return false;
            }
        }

        return true;
    }

    bool IsMouseOnCell(int i, int j)
    {
        int x = sudoku.position.x, y = sudoku.position.y;
        int x1 = x + sudoku.length, y1 = y + sudoku.length;

        if(mosusePosition.x > x1 || mosusePosition.y > y1 || mosusePosition.x < x || mosusePosition.y < y) return false;
        int cellSize = sudoku.cellSize;
        
        int left = x + (cellSize * i);
        int right = left + cellSize;
        int top = y + (cellSize * j);
        int bottom = top + cellSize;

        return mosusePosition.x < right && 
            mosusePosition.y < bottom && 
            mosusePosition.x > left && 
            mosusePosition.y > top;
    }

    void InitializeSudoku()
    {
        std::ifstream settings("sett.ini");
        if(!settings)
        {
            emptyCells = 30;
            ParseColorPalette(7);
        }
        else 
        {
            int x;
            settings >> x;
            if(x < 50) emptyCells = x;
                else emptyCells = 50;
    
            settings >> x;
            ParseColorPalette(x);
        }

        SetScreenBackgroundColor(Color(color1.r + 30, color1.g + 20, color1.b + 40));

        titleColor = color3;
        
        cellColor = color1;
        cellBorderColor = color2;
        borderColor = color3;
        numberColor = color4;
        lnumberColor = color2;

        rowNumberColor = color3;
        rowCircleBorderColor = color2;
        rowCircleColor = color1;
        uRowCircleColor = color4;
        fRowCircleColor = color2;

        settings.close();

        // title
        titleText = Text(40, 50);
        titleText.SetFont(font);
        titleText.SetString("SUDOKU");
        titleText.SetFontWeight(2);
        titleText.SetFontSize(6);
        titleText.GetTransform().Scale(Vector2(5, 2));
        titleText.SetColor(titleColor);

        // sudoku
        sudoku.position = Vector2(30, 30);
        sudoku.cellSize = 14;
        sudoku.length = sudoku.cellSize * 9;
        sudoku.table = std::vector<std::vector<Cell>>(9, std::vector<Cell>(9));
        sudoku.usedNumbers = std::vector<int>(9);

        cell = Rectangle(sudoku.position.x, sudoku.position.y, sudoku.cellSize, sudoku.cellSize);
        cell.SetFillColor(cellColor);
        cell.SetOutlineColor(cellBorderColor);

        cellNumber = Text(sudoku.position.x, sudoku.position.y);
        cellNumber.SetFont(font);
        cellNumber.GetTransform().Scale(Vector2(0, 1));
        cellNumber.SetColor(numberColor);
        
        // row numbers
        rowCircle = Elipse(sudoku.position.x, sudoku.position.y + sudoku.length + 2, sudoku.cellSize / 2 - 1, sudoku.cellSize / 2 - 1, 8);
        rowCircle.SetFillColor(rowCircleColor);
        rowCircle.SetOutlineColor(rowCircleBorderColor);

        rowNumber = Text(sudoku.position.x, sudoku.position.y + sudoku.length + 3);
        rowNumber.SetFont(font);
        rowNumber.GetTransform().Scale(Vector2(0, 1));
        rowNumber.SetColor(rowNumberColor);

        CreateSudokuMatrix();
    }

    void OnStart() override
    {
        font = Font("Resources/basic.f2p");
        
        InitializeSudoku();
    }

    float frameTimer = 1;
    void OnUpdate(float deltaTime) override
    {   
        mosusePosition = ScreenMousePosition(Vector2(input.GetMousePositionX(), input.GetMousePositionY())); // get screen mouse position

        frameTimer += deltaTime;
        if(frameTimer >= 1)
        {
            SetWindowTitle(GetWindowTitle() + L" | FPS: " + std::to_wstring(int (1 / deltaTime)) + L" | DT: " + std::to_wstring(deltaTime) + L" MS");
            frameTimer = 0;
        }

        // game logic
        for(int i = 0; i < 9; i++)
        {
            for(int j = 0; j < 9; j++)
            {
                if(IsMouseOnCell(i, j)){ sudoku.table[i][j].mouseOn = true; sudoku.mousei = i; sudoku.mousej = j;}
                else sudoku.table[i][j].mouseOn = false;
            }
        }
            
        if(!completed)
        {
            if(input.isKeyDown(Key_1)) sudoku.usingNumber = 1;
            if(input.isKeyDown(Key_2)) sudoku.usingNumber = 2;
            if(input.isKeyDown(Key_3)) sudoku.usingNumber = 3;
            if(input.isKeyDown(Key_4)) sudoku.usingNumber = 4;
            if(input.isKeyDown(Key_5)) sudoku.usingNumber = 5;
            if(input.isKeyDown(Key_6)) sudoku.usingNumber = 6;
            if(input.isKeyDown(Key_7)) sudoku.usingNumber = 7;
            if(input.isKeyDown(Key_8)) sudoku.usingNumber = 8;
            if(input.isKeyDown(Key_9)) sudoku.usingNumber = 9;
            
            if(input.isKeyDown(Key_Space))
            {
                if(CompletedBoard())
                {
                    fRowCircleColor = color3;
                    completed = true;
                }
            }
            
            if(IsMouseOnCell(sudoku.mousei, sudoku.mousej))
            {
                if(input.isMouseButtonDown(Mouse::Left) && sudoku.table[sudoku.mousei][sudoku.mousej].number == 0 && sudoku.usedNumbers[sudoku.usingNumber - 1] < 9)
                {
                    sudoku.table[sudoku.mousei][sudoku.mousej].number = sudoku.usingNumber;
                    sudoku.usedNumbers[sudoku.usingNumber - 1]++;
                }
                
                if(input.isMouseButtonDown(Mouse::Right) && sudoku.table[sudoku.mousei][sudoku.mousej].number != 0 && !sudoku.table[sudoku.mousei][sudoku.mousej].lookedNumber)
                {
                    sudoku.usedNumbers[sudoku.table[sudoku.mousei][sudoku.mousej].number - 1]--;
                    sudoku.table[sudoku.mousei][sudoku.mousej].number = 0;
                }
            } 
        }
    }
    
    void OnDraw(Screen &screen) override
    {
        DrawGameBoard(screen);
        DrawNumberRow(screen);
    }

    void OnQuit() override
    {

    }
};