#pragma once

class ConstantPendul
{
private:
    Elipse e;
    float length;
    float period;
    float angle = 0;
    Vector2 anchor;

public:
    bool addPoints;
    std::vector<Vector2> points;
    Vector2 position = Vector2::ZERO;

    ConstantPendul() {}

    ConstantPendul(float length, float period, Vector2 anchor, bool addPoints = false)
    {
        this->position = Vector2::ZERO;
        this->length = length;
        this->period = period;
        this->anchor = anchor;
        this->addPoints = addPoints;

        e = Elipse(0, 0, 3, 3, 15);

        e.SetFillColor(Color::White);
        e.SetOutlineColor(Color::White);
        e.SetPivot({0.5, 0.5});
    }

    void update(float deltaTime)
    {
        angle += period * deltaTime;

        angle = NormalizeAngle(angle);

        float x = cos(angle) * length;
        float y = sin(angle) * length;

        position = Vector2(x, y) + anchor;

        if(addPoints)
            points.push_back(position);
    }

    void update(float deltaTime, Vector2 anchor)
    {
        this->anchor = anchor;
        update(deltaTime);
    }

    void draw(Screen &screen)
    {
        e.GetTransform().MoveTo(position);
        DrawLine(screen, anchor.x, anchor.y, position.x, position.y, Color::White);

        e.Draw(screen);
    }
};

class CompoundPendulum
{
private:
    Elipse e;
    
    Vector2 anchor;

    float mass;
    float length;
    float inertia;
    float rCM;
    float period;
    float theta;
    float time;

public:
    bool addPoints;
    std::vector<Vector2> points;
    Vector2 position = Vector2::ZERO;

    CompoundPendulum() {}

    CompoundPendulum(Vector2 anchor, float length, float mass, float amplitude, bool addPoints = false)
    {
        this->position = Vector2::ZERO;
        this->length = length;
        this->anchor = anchor;
        this->mass = mass;
        
        this->time = 0;
        this->rCM = length;
        this->inertia = (mass * length * length) / 3;
        this->theta = amplitude;
        this->period = 2 * PI * sqrt(inertia / (mass * 9.81 * rCM));

        this->addPoints = addPoints;

        e = Elipse(0, 0, 3, 3, 15);
        e.SetFillColor(Color::White);
        e.SetOutlineColor(Color::White);
        e.SetPivot({0.5, 0.5});
    }

    void update(float deltaTime)
    {
        time += deltaTime;

        float cos0 = cos((2 * PI * time) / period);
        float theta = theta * cos0 - PI / 2;

        float x = length * cos(theta);
        float y = -length * sin(theta);

        this->position = Vector2(x, y) + anchor;

        if(addPoints)
            points.push_back(position);
    }

    void update(float time, Vector2 anchor)
    {
        this->anchor = anchor;
        update(time);
    }

    void draw(Screen &screen)
    {
        e.GetTransform().MoveTo(position);
        DrawLine(screen, anchor.x, anchor.y, position.x, position.y, Color::White);

        e.Draw(screen);
    }
};

class GravityPendulum
{
private:
    Elipse e;
    
    Vector2 anchor;

    float length;
    float theta;
    float angularVelocity;

public:
    bool addPoints;
    std::vector<Vector2> points;
    Vector2 position = Vector2::ZERO;

    GravityPendulum() {}

    GravityPendulum(Vector2 anchor, float length, float mass, float amplitude, bool addPoints = false)
    {
        this->position = Vector2::ZERO;
        this->length = length;
        this->anchor = anchor;

        this->theta = amplitude;
        this->angularVelocity = 0;

        this->addPoints = addPoints;

        e = Elipse(0, 0, 3, 3, 15);
        e.SetFillColor(Color::White);
        e.SetOutlineColor(Color::White);
        e.SetPivot({0.5, 0.5});
    }

    void update(float deltaTime)
    {
        float angularAcceleration = (-20.81 / length) * sin(theta - PI / 2);
        angularVelocity += angularAcceleration * deltaTime;
        theta += angularVelocity * deltaTime;

        float x = length * sin(theta + PI / 2);
        float y = -length * cos(theta + PI / 2);

        this->position = Vector2(x, y) + anchor;

        if(addPoints)
            points.push_back(position);
    }

    void update(float time, Vector2 anchor)
    {
        this->anchor = anchor;
        update(time);
    }

    void draw(Screen &screen)
    {
        e.GetTransform().MoveTo(position);
        DrawLine(screen, anchor.x, anchor.y, position.x, position.y, Color::White);

        e.Draw(screen);
    }
};