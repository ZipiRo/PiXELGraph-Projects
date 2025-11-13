#pragma once

class PTransform
{
    public:
        Vector2 position;
        float sin;
        float cos;

        PTransform() 
        {
            this->position = Vector2(0, 0);
            this->cos = 1.0f;
            this->sin = 0.0f;
        }

        PTransform(Vector2 position, float angle)
        {
            this->position = position;
            this->sin = sinf(angle * DEG_TO_RAD);
            this->cos = cosf(angle * DEG_TO_RAD);
        }

        PTransform(float x, float y, float angle)
        {
            this->position = Vector2(x, y);
            
            this->sin = sinf(angle * DEG_TO_RAD);
            this->cos = cosf(angle * DEG_TO_RAD);
        }
};