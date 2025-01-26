const int SNAKE_MAX_LENGTH = 1000;

class Snake
{
private:
    Box boundry;

    Vector2 headPosition;
    Vector2 headDirection;
    Vector2 bodyPositions[SNAKE_MAX_LENGTH + 5];

    int snakeLength;
    int bodySize;
    bool alive = 1;

    Color primary_color;
    Color eyes_color;

public:
    Snake() {}
    Snake(int col, int row, int tileSize, Box boundry);

    void Update();
    bool Control(int up, int down, int right, int left);
    void Draw(Screen &screen);
    void SetColors(Color primary, Color eyes);
};

Snake::Snake(int col, int row, int tileSize, Box boundry)
{
    this->headPosition = Vector2(row * tileSize, col * tileSize);
    this->headDirection = ZERO;
    this->snakeLength = 10;
    this->bodySize = tileSize - 1;
    this->boundry = boundry;

    for(int i = 0; i < snakeLength; i++)
        bodyPositions[i] = this->headPosition;

    this->primary_color = Green;
    this->eyes_color = Black;
}

bool Snake::Control(int up, int down, int right, int left)
{
    int curentKey = 0;

    if (_kbhit())
        curentKey = _getch();
    if(curentKey == right) 
    {
        if(headDirection.x != -1)
            headDirection = RIGHT;
        return true;
    }

    if (_kbhit())
        curentKey = _getch();
    if(curentKey == left) 
    {
        if(headDirection.x != 1)
            headDirection = LEFT;
        return true;
    }

    if (_kbhit())
        curentKey = _getch();
    if(curentKey == up) 
    {
        if(headDirection.y != 1)
            headDirection = UP;
        return true;
    }
    
    if (_kbhit())
        curentKey = _getch();
    if(curentKey == down) 
    {
        if(headDirection.y != -1)
            headDirection = DOWN;
        return true;
    }

    return false;
}

bool OutOfBounds(Vector2 position, int bodySize, const Box &bounds)
{
    return (position.x + bodySize >= bounds.right || 
        position.x - bodySize <= bounds.left ||
        position.y + bodySize >= bounds.bottom || 
        position.y - bodySize <= bounds.top);
}

bool CollideWithBody(Vector2 nextPosition, Vector2 bodyPositions[SNAKE_MAX_LENGTH], int snakeLength)
{
    for(int i = 0; i < snakeLength; i++)
    {
        if(nextPosition == bodyPositions[i])
            return true;
    }

    return false;
}

void Snake::Update()
{
    if(!alive || !headDirection) return;

    Vector2 nextPosition = headPosition + headDirection * (bodySize + 1);

    alive = !OutOfBounds(headPosition, bodySize, boundry) && 
            !CollideWithBody(nextPosition, bodyPositions, snakeLength); 

    if(!alive) 
    {
        primary_color = Red;
        headDirection = ZERO;
        return;
    }
    
    headPosition += headDirection * (bodySize + 1); 

    for(int i = snakeLength - 1; i >= 0; i--)
        bodyPositions[i] = bodyPositions[i - 1];
    bodyPositions[0] = headPosition;
}

void Snake::Draw(Screen &screen)
{
    Rectangle BodyPiece(headPosition.x, headPosition.y, bodySize, bodySize);
    BodyPiece.SetPivot({0, 0});
    BodyPiece.SetFillColor(primary_color);
    BodyPiece.SetOutlineColor(primary_color);

    for(int i = 0; i < snakeLength; i++)
    {
        Vector2 position = bodyPositions[i];
        BodyPiece.GetTransform().MoveTo(position);
        BodyPiece.Draw(screen);
    }

    screen.PlotPixel(headPosition.x, headPosition.y, eyes_color);
    screen.PlotPixel(headPosition.x + bodySize, headPosition.y, eyes_color);
}

void Snake::SetColors(Color primary, Color eyes)
{
    this->primary_color = primary;
    this->eyes_color = eyes;
}