#include "../Include/PixelGraph.h"
#include "Vector3.h"
#include "Matrix.h"

Matrix CreateCameraMatrix(Vector3 cameraPosition, Vector3 targetPosition, Vector3 upVector) {
    Vector3 forward = Vector3::Normalize(targetPosition - cameraPosition);  
    Vector3 vector = forward * Vector3::DotProduct(upVector, forward);
    Vector3 up = Vector3::Normalize(upVector - vector);
    Vector3 right = Vector3::Normalize(Vector3::CrossProduct(up, forward));

    Matrix viewMatrix(4, 4);

    viewMatrix.matrix[0][0] = right.x;           viewMatrix.matrix[0][1] = right.y;           viewMatrix.matrix[0][2] = right.z;          viewMatrix.matrix[0][3] = 0;
    viewMatrix.matrix[1][0] = up.x;              viewMatrix.matrix[1][1] = up.y;              viewMatrix.matrix[1][2] = up.z;             viewMatrix.matrix[1][3] = 0;
    viewMatrix.matrix[2][0] = forward.x;         viewMatrix.matrix[2][1] = forward.y;         viewMatrix.matrix[2][2] = forward.z;        viewMatrix.matrix[2][3] = 0;
    viewMatrix.matrix[3][0] = cameraPosition.x;  viewMatrix.matrix[3][1] = cameraPosition.y;  viewMatrix.matrix[3][2] = cameraPosition.z; viewMatrix.matrix[3][3] = 1;

    return viewMatrix;
}

Matrix Create_QuikInverseMatrix(Matrix &matrix) // Roatation/Translation Matrix ONLY
{
    Matrix inverseMatrix(4, 4);

    inverseMatrix.matrix[0][0] = matrix.matrix[0][0]; inverseMatrix.matrix[0][1] = matrix.matrix[1][0]; inverseMatrix.matrix[0][2] = matrix.matrix[2][0]; inverseMatrix.matrix[0][3] = 0;
    inverseMatrix.matrix[1][0] = matrix.matrix[0][1]; inverseMatrix.matrix[1][1] = matrix.matrix[1][1]; inverseMatrix.matrix[1][2] = matrix.matrix[2][1]; inverseMatrix.matrix[1][3] = 0;
    inverseMatrix.matrix[2][0] = matrix.matrix[0][2]; inverseMatrix.matrix[2][1] = matrix.matrix[1][2]; inverseMatrix.matrix[2][2] = matrix.matrix[2][2]; inverseMatrix.matrix[2][3] = 0;
    
    inverseMatrix.matrix[3][0] = -(matrix.matrix[3][0] * inverseMatrix.matrix[0][0] + matrix.matrix[3][1] * inverseMatrix.matrix[1][0] + matrix.matrix[3][2] * inverseMatrix.matrix[2][0]);
    inverseMatrix.matrix[3][1] = -(matrix.matrix[3][0] * inverseMatrix.matrix[0][1] + matrix.matrix[3][1] * inverseMatrix.matrix[1][1] + matrix.matrix[3][2] * inverseMatrix.matrix[2][1]);
    inverseMatrix.matrix[3][2] = -(matrix.matrix[3][0] * inverseMatrix.matrix[0][2] + matrix.matrix[3][1] * inverseMatrix.matrix[1][2] + matrix.matrix[3][2] * inverseMatrix.matrix[2][2]);
    inverseMatrix.matrix[3][3] = 1;

    return inverseMatrix;
}

Matrix CreatePerspectiveMatrix(float fov, float aspectRatio, float nearPlane, float farPlane)
{
    float tanHalfFOV = tan(fov / 2);
    Matrix projectionMatrix(4, 4);

    projectionMatrix.matrix[0][0] = 1 / (aspectRatio * tanHalfFOV);
    projectionMatrix.matrix[1][1] = 1 / tanHalfFOV;
    projectionMatrix.matrix[2][2] = -(farPlane + nearPlane) / (farPlane - nearPlane);
    projectionMatrix.matrix[2][3] = -(2 * farPlane * nearPlane) / (farPlane - nearPlane);
    projectionMatrix.matrix[3][2] = -1;
    projectionMatrix.matrix[3][3] = 0;

    return projectionMatrix;
}

Matrix Create_YRotationMatrix(float angle)
{
    float cosY = cos(angle), sinY = sin(angle);

    Matrix yRotationMatrix(4, 4);
    yRotationMatrix.matrix[0][0] = cosY; yRotationMatrix.matrix[0][2] = sinY;
    yRotationMatrix.matrix[1][1] = 1;
    yRotationMatrix.matrix[2][0] = -sinY; yRotationMatrix.matrix[2][2] = cosY;
    yRotationMatrix.matrix[3][3] = 1;

    return yRotationMatrix;
}

Matrix Create_XRotationMatrix(float angle)
{
    float cosX = cos(angle), sinX = sin(angle);

    Matrix xRotationMatrix(4, 4); 
    xRotationMatrix.matrix[0][0] = 1;
    xRotationMatrix.matrix[1][1] = cosX; xRotationMatrix.matrix[1][2] = -sinX;
    xRotationMatrix.matrix[2][1] = sinX; xRotationMatrix.matrix[2][2] = cosX;
    xRotationMatrix.matrix[3][3] = 1;

    return xRotationMatrix;
}

Matrix Create_ZRotationMatrix(float angle)
{
    float cosZ = cos(angle), sinZ = sin(angle);

    Matrix zRotationMatrix(4, 4);
    zRotationMatrix.matrix[0][0] = cosZ; zRotationMatrix.matrix[0][1] = -sinZ;
    zRotationMatrix.matrix[1][0] = sinZ; zRotationMatrix.matrix[1][1] = cosZ;
    zRotationMatrix.matrix[2][2] = 1;
    zRotationMatrix.matrix[3][3] = 1;

    return zRotationMatrix;
}

Matrix CreateTransformMatrix(Vector3 position, Vector3 scale, Vector3 angle)
{
    Matrix scaleMatrix(4, 4);
    scaleMatrix.matrix[0][0] = scale.x;
    scaleMatrix.matrix[1][1] = scale.y;
    scaleMatrix.matrix[2][2] = scale.z;
    scaleMatrix.matrix[3][3] = 1;

    Matrix xRotationMatrix = Create_XRotationMatrix(angle.x);
    Matrix yRotationMatrix = Create_YRotationMatrix(angle.y);
    Matrix zRotationMatrix = Create_ZRotationMatrix(angle.z);

    Matrix rotationMatrix = MatrixMultiply(zRotationMatrix, MatrixMultiply(yRotationMatrix, xRotationMatrix));

    Matrix scaleRotationMatrix = MatrixMultiply(rotationMatrix, scaleMatrix);

    Matrix translateMatrix(4, 4);
    translateMatrix.matrix[0][0] = 1; translateMatrix.matrix[0][3] = position.x;
    translateMatrix.matrix[1][1] = 1; translateMatrix.matrix[1][3] = position.y;
    translateMatrix.matrix[2][2] = 1; translateMatrix.matrix[2][3] = position.z;
    translateMatrix.matrix[3][3] = 1;

    return MatrixMultiply(translateMatrix, scaleRotationMatrix);
}

Vector3 TransformVertex(const Matrix &projectionMatrix, const Matrix &viewMatrix, Vector3 vertex, Vector3 position, Vector3 scale, Vector3 angle)
{
    angle.z = NormalizeAngle(angle.z);
    angle.x = NormalizeAngle(angle.x);
    angle.y = NormalizeAngle(angle.y);

    Matrix transformMatrix = CreateTransformMatrix(position, scale, angle);

    Matrix a(1, 4);
    a.matrix[0][0] = vertex.x;
    a.matrix[1][0] = vertex.y;
    a.matrix[2][0] = vertex.z;
    a.matrix[3][0] = 1;

    Matrix MVP = MatrixMultiply(projectionMatrix, MatrixMultiply(viewMatrix, transformMatrix));

    a = MatrixMultiply(MVP, a);

    float w = a.matrix[3][0];
    if(w)
    {
        a.matrix[0][0] /= w;
        a.matrix[1][0] /= w;
        a.matrix[2][0] /= w;
    }

    return Vector3(a.matrix[0][0], a.matrix[1][0], a.matrix[2][0]);
}

Color colors[10] = {Color::Red, Color::Green, Color::Blue, Color::LightYellow};

class Game : public PiXELGraph
{
public:
    Game()
    {
        SetScreenBackgroundColor(Color(6, 174, 200)); 
        SetTitle(L"3DEngine");

        Init(1240, 720, 3);
    }

private:
    Font font;
    Text text;

    Box screenBounds;
    Matrix projectionMatrix;
    Matrix cameraRotationMatrix;
    Matrix cameraMatrix;
    Matrix viewMatrix;

    std::vector<Vector3> points;
    std::vector<int> indices = {
        0, 1, 0, 4, 0, 3, 1, 2, 1, 5, 2, 3, 2, 6, 3, 7, 4, 5, 4, 7, 5, 6, 6, 7
    };

    std::vector<Vector3> t1points;
    
    Vector3 cameraPosition;
    Vector3 targetPosition;
    Vector3 lookDirection;

    Vector3 position1;
    Vector3 angle1;
    Vector3 scale1;

    void OnStart() override
    {      
        srand(time(0));

        font = Font("Resources/basic.f2p");
        text = Text(2, 2);
        text.SetFont(font);
        text.SetColor(Color::Black);

        cameraPosition = Vector3(0, 0, 10);
        lookDirection = Vector3::FORWARD;
        targetPosition = Vector3::FORWARD;

        screenBounds = GetScreenBounds();
        cameraMatrix = CreateCameraMatrix(cameraPosition, targetPosition, Vector3::UP);
        viewMatrix = Create_QuikInverseMatrix(cameraMatrix);
        projectionMatrix = CreatePerspectiveMatrix(PI / 2, screenBounds.right / screenBounds.bottom, 0.1, 500);

        Vector3 point1(-0.5, -0.5, -0.5);
        Vector3 point2(0.5, -0.5, -0.5);
        Vector3 point3(0.5, 0.5, -0.5);
        Vector3 point4(-0.5, 0.5, -0.5);
        Vector3 point5(-0.5, -0.5, 0.5);
        Vector3 point6(0.5, -0.5, 0.5);
        Vector3 point7(0.5, 0.5, 0.5);
        Vector3 point8(-0.5, 0.5, 0.5);
        points.push_back(point1);
        points.push_back(point2);
        points.push_back(point3);
        points.push_back(point4);
        points.push_back(point5);
        points.push_back(point6);
        points.push_back(point7);
        points.push_back(point8);

        position1 = Vector3(0, 0, 100);
        angle1 = Vector3(0, PI / 3, 0);
        scale1 = Vector3(20, 20, 20);

        for(Vector3 &point : points)
        {
            Vector3 new_Point = TransformVertex(projectionMatrix, viewMatrix, point, position1, scale1, angle1);
            new_Point.x = (new_Point.x + 1) * 0.5 * screenBounds.right;
            new_Point.y = (1 - new_Point.y) * 0.5 * screenBounds.bottom;

            t1points.push_back(new_Point);
        }
    }

    float frameTimer = 1;
    float angles = 0;

    float fYaw = 0;

    void OnUpdate(float deltaTime) override
    {   
        frameTimer += deltaTime;
        if(frameTimer >= 1)
        {
            SetWindowTitle(GetWindowTitle() + L" | FPS: " + std::to_wstring(int (1 / deltaTime)) + L" | DT: " + std::to_wstring(deltaTime) + L" MS");
            frameTimer = 0;
        }

        if(input.isKeyDown(Keyboard::Key_Escape)) Quit();

        if(input.isKeyDown(Keyboard::Key_UpArrow))
        {
            cameraPosition.y -= 8 * deltaTime;
        }
        
        if(input.isKeyDown(Keyboard::Key_DownArrow))
        {
            cameraPosition.y += 8 * deltaTime;
        }
        
        if(input.isKeyDown(Keyboard::Key_D))
        {
            cameraPosition.x += 8 * deltaTime;
        }

        if(input.isKeyDown(Keyboard::Key_A))
        {
            cameraPosition.x -= 8 * deltaTime;
        }

        Vector3 forward = lookDirection * (8 * deltaTime);

        if(input.isKeyDown(Keyboard::Key_W))
        {
            cameraPosition += forward;
        }
        if(input.isKeyDown(Keyboard::Key_S))
        {
            cameraPosition -= forward;
        }

        if(input.isKeyDown(Keyboard::Key_Q))
        {
            fYaw -= 2 * deltaTime;
        }
        if(input.isKeyDown(Keyboard::Key_E))
        {
            fYaw += 2 * deltaTime;
        }

        cameraRotationMatrix = Create_YRotationMatrix(fYaw);
        lookDirection = Vector3::Normalize(MatrixMultiplyVector(cameraRotationMatrix, targetPosition));
        targetPosition = cameraPosition + lookDirection;

        cameraMatrix = CreateCameraMatrix(cameraPosition, targetPosition, Vector3::UP);

        viewMatrix = Create_QuikInverseMatrix(cameraMatrix);

        t1points.clear();
        angles += PI / 3 * deltaTime;
        angle1 = Vector3(PI / 4, PI / 3, angles);

        text.SetString("CAMERA POS: X" + std::to_string(int(cameraPosition.x)) + " Y" + std::to_string(int(cameraPosition.y)) + " Z" + std::to_string(int(cameraPosition.z)) + "YAW " + std::to_string(fYaw));

        for(Vector3 &point : points)
        {
            Vector3 new_Point = TransformVertex(projectionMatrix, viewMatrix, point, position1, scale1, angle1);
            new_Point.x = (new_Point.x + 1) * 0.5 * screenBounds.right;
            new_Point.y = (1 - new_Point.y) * 0.5 * screenBounds.bottom;

            t1points.push_back(new_Point);
        }
    }
    
    int colorC = 0;
    void OnDraw(Screen &screen) override
    {
        for (size_t i = 0; i < indices.size(); i += 2) {
            Vector3 vertexA = t1points[indices[i]];
            Vector3 vertexB = t1points[indices[i + 1]];

            DrawLine(screen, vertexA.x, vertexA.y, vertexB.x, vertexB.y, colors[colorC++ % 3]);
        }

        text.Draw(screen);

        screen.PlotPixel(screenBounds.right / 2, screenBounds.bottom / 2, Color::Black);
    }

    void OnQuit() override
    {
        // DestroyThings
    }
};