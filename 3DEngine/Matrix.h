class Matrix
{
public:
    int cols, rows;
    float matrix[5][5];

    Matrix()
    {
        this->cols = 1;
        this->rows = 1;

        matrix[0][0] = 0;
    }

    Matrix(int cols, int rows)
    {
        this->cols = cols;
        this->rows = rows;

        for(int i = 0; i < cols; i++)
            for(int j = 0; j < rows; j++)
                matrix[i][j] = 0;
    }
};

Matrix MatrixMultiply(const Matrix &a, const Matrix &b)
{
    if(a.cols != b.rows)
        return Matrix();

    Matrix result(a.rows, b.cols);

    for(int i = 0; i < a.rows; i++)
    {
        for(int j = 0; j < b.cols; j++)
        {
            for(int k = 0; k < a.cols; k++)
            {
                result.matrix[i][j] += a.matrix[i][k] * b.matrix[k][j];
            }
        }
    }

    return result;
}


Vector3 MatrixMultiplyVector(const Matrix &matrix, const Vector3 &vector)
{
    Matrix a(1, 4);
    a.matrix[0][0] = vector.x;
    a.matrix[1][0] = vector.y;
    a.matrix[2][0] = vector.z;
    a.matrix[3][0] = 1;

    a = MatrixMultiply(matrix, a);

    return Vector3(a.matrix[0][0], a.matrix[1][0], a.matrix[2][0]);
}