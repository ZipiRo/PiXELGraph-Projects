#pragma once

class Ball
{
private:
    Shapes::Elipse elipse;
    
    Vector2 position;
    Vector2 force;

    float radius;

    void CreateBall()
    {
        showVelocity = false;
        elipse = Shapes::Elipse(radius, radius, 15);
        elipse.transform.SetPosition(position);
    }

    void DrawVelocityLines()
    {
        if (!showVelocity) return;
        std::vector<Vertex> line = {
            {position, Color::Red},
            {position + linearVelocity, Color::Red}
        };
        DrawLines(line, false);
    }

public:
    bool showVelocity;

    Vector2 linearVelocity;
    float restitution;
    float mass;
    float inverseMass;

    Ball()
    {
        position = Vector2::ZERO;
        radius = 1.0f;
        mass = 1.0f;
        inverseMass = 1.0f / mass;

        CreateBall();
    }
    
    Ball(float x, float y, float radius, float mass, float restitution)
    {
        this->position = Vector2(x, y);
        this->radius = radius;
        this->mass = mass;
        this->restitution = restitution;
        inverseMass = 1.0f / this->mass;

        CreateBall();
    }

    void Step(float deltaTime)
    {
        Vector2 acceleration = force * inverseMass;

        linearVelocity += acceleration * deltaTime;
        linearVelocity *= 0.999f; // simple air resistance
        position += linearVelocity * deltaTime;

        force = Vector2::ZERO;        
    }

    void Draw()
    {
        elipse.transform.SetPosition(position);
        DrawShape(elipse);

        DrawVelocityLines();
    }   

    void SetColor(const Color &newColor)
    {
        elipse.SetFillColor(newColor);
    }

    void AddForce(const Vector2 &amount)
    {
        force += amount;
    }

    void SetPosition(float x, float y)
    {
        position = Vector2(x, y);
    }
    
    void Move(const Vector2 &amount)
    {
        position += amount;
    }

    void SetPosition(const Vector2 &position)
    {
        this->position = position;
    }

    Vector2 GetPosition()
    {
        return position;
    }

    float GetRadius()
    {
        return radius;
    }

    AABB GetAABB()
    {
        AABB boundingBox(position.x - radius, position.y - radius, position.x + radius, position.y + radius);
        return boundingBox;
    }

    Vector2 GetVelocity()
    {
        return linearVelocity;
    }
};