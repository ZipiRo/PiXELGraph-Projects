#pragma once

const float g = 9.81;
const float pixelsPerMeter = 10;

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

    ConstantPendul(float length, float period, Vector2 anchor)
    {
        this->position = Vector2::ZERO;
        this->length = length;
        this->period = period;
        this->anchor = anchor;
        this->addPoints = false;

        e = Elipse(0, 0, 3, 3, 5);

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

        position = Vector2(x, y) * pixelsPerMeter;
        position += anchor;

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
    float amplitude;
    float time;

public:
    bool addPoints;
    std::vector<Vector2> points;
    Vector2 position = Vector2::ZERO;

    CompoundPendulum() {}

    CompoundPendulum(Vector2 anchor, float length, float mass, float amplitude)
    {
        this->position = Vector2::ZERO;
        this->length = length;
        this->anchor = anchor;
        this->mass = mass;
        
        this->time = 0;
        this->rCM = length;
        this->inertia = (mass * length * length) / 3;
        this->amplitude = amplitude;
        this->period = 2 * PI * sqrt(inertia / (mass * g * rCM));

        this->addPoints = false;

        e = Elipse(0, 0, 3, 3, 15);
        e.SetFillColor(Color::White);
        e.SetOutlineColor(Color::White);
        e.SetPivot({0.5, 0.5});
    }

    void update(float deltaTime)
    {
        time += deltaTime;

        float cos0 = cos((2 * PI * time) / period);
        float theta = amplitude * cos0;

        float x = length * cos(theta);
        float y = -length * sin(theta);

        position = Vector2(x, y) * pixelsPerMeter;
        position += anchor;

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

    float angularVelocity;
    float length;
    float theta;
    float mass;
    float inertia;
    float dampingFactor;
    float invInertia;

    float currentAngularAcc;
    float timer;
public:
    bool addPoints;
    std::vector<Vector2> points;
    Vector2 position = Vector2::ZERO;

    GravityPendulum() {}

    GravityPendulum(Vector2 anchor, float length, float mass, float theta, float dampingFactor = 0.1f)
    {
        this->position = Vector2::ZERO;
        this->anchor = anchor;
        this->length = length;
        this->mass = mass;
        this->theta = theta;
        this->dampingFactor = dampingFactor;
        
        this->inertia = mass * length * length / 3;
        this->invInertia = 1 / inertia;
        this->angularVelocity = 0;
        this->timer = 0;

        this->addPoints = false;

        e = Elipse(0, 0, 3, 3, 5);
        e.SetFillColor(Color::White);
        e.SetOutlineColor(Color::White);
        e.SetPivot({0.5, 0.5});
    }

    void update(float deltaTime)
    {
        timer += deltaTime;
        float angularAcceleration = (-g * sin(theta)) / length - (dampingFactor * invInertia) * angularVelocity;
        currentAngularAcc = angularAcceleration;

        angularVelocity += angularAcceleration * deltaTime;
        theta += angularVelocity * deltaTime;

        float x = length * sin(theta);
        float y = length * cos(theta);

        position = Vector2(x, y) * pixelsPerMeter;
        position += anchor;

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

    float GetAngularVelocity()
    {   
        return angularVelocity;
    }
    
    float GetCurenAngularAcc()
    {   
        return currentAngularAcc;
    }

    float Timer()
    {
        return timer;
    }
};

class DoublePendulum
{
private:
    Elipse e;
    Vector2 anchor;

    float m1, m2;    
    float L1, L2;    
    float I1, I2;   
    float theta1, theta2; 
    float omega1, omega2; 
    float dampingFactor = 0.1;

public:
    bool addPoints;
    std::vector<Vector2> points;
    Vector2 position1, position2;

    DoublePendulum() {}
    
    DoublePendulum(Vector2 anchor, float L1, float L2, float m1, float m2, float theta1, float theta2)
    {
        this->anchor = anchor;
        this->L1 = L1;
        this->L2 = L2;
        this->m1 = m1;
        this->m2 = m2;
        this->theta1 = theta1;
        this->theta2 = theta2;
        this->omega1 = 0;
        this->omega2 = 0;

        I1 = m1 * L1 * L1 / 3;
        I2 = m2 * L2 * L2 / 3;

        e = Elipse(0, 0, 3, 3, 5);
        e.SetFillColor(Color::White);
        e.SetOutlineColor(Color::White);
        e.SetPivot({0.5, 0.5});

        addPoints = false;
    }

    void update(float deltaTime)
    {
        float num1 = -g * (2 * m1 + m2) * sin(theta1);
        float num2 = -m2 * g * sin(theta1 - 2 * theta2);
        float num3 = -2 * sin(theta1 - theta2) * m2 * (omega2 * omega2 * L2 + omega1 * omega1 * L1 * cos(theta1 - theta2));
        float den1 = I1 + m2 * L1 * L1;
        float alpha1 = (num1 + num2 + num3) / den1 - dampingFactor * omega1;

        float num4 = 2 * sin(theta1 - theta2) * (omega1 * omega1 * L1 * (m1 + m2) + g * (m1 + m2) * cos(theta1) + omega2 * omega2 * L2 * m2 * cos(theta1 - theta2));
        float den2 = I2 + m2 * L2 * L2;
        float alpha2 = num4 / den2 - dampingFactor * omega2;

        omega1 += alpha1 * deltaTime;
        omega2 += alpha2 * deltaTime;
        theta1 += omega1 * deltaTime;
        theta2 += omega2 * deltaTime;

        float x1 = L1 * sin(theta1);
        float y1 = L1 * cos(theta1);
        position1 = Vector2(x1, y1) * pixelsPerMeter;
        position1 += anchor;

        float x2 = x1 + L2 * sin(theta2);
        float y2 = y1 + L2 * cos(theta2);
        position2 = Vector2(x2, y2) * pixelsPerMeter;
        position2 += anchor;

        if (addPoints)
            points.push_back(position2);
    }

    void draw(Screen &screen)
    {
        DrawLine(screen, anchor.x, anchor.y, position1.x, position1.y, Color::White);
        DrawLine(screen, position1.x, position1.y, position2.x, position2.y, Color::White);

        e.GetTransform().MoveTo(position1);
        e.Draw(screen);

        e.GetTransform().MoveTo(position2);
        e.Draw(screen);
    }
};

struct RopeSegment
{
    Vector2 position;
    Vector2 oldPosition; // For Verlet integration

    RopeSegment(Vector2 pos) : position(pos), oldPosition(pos) {}
};

class Rope
{
private:
    int numSegments = 50; // Number of rope segments
    float segmentLength = 5.0f; // Length of each segment
    int constraintIterations = 5; // More iterations = stiffer rope
    std::vector<RopeSegment> segments;
    Vector2 anchor; // The fixed point at the top

public:
    Rope() {}

    Rope(Vector2 startPos)
    {
        anchor = startPos;

        // Create rope segments
        for (int i = 0; i < numSegments; i++)
        {
            segments.push_back(RopeSegment(Vector2(startPos.x * 10, startPos.y + i * segmentLength)));
        }
    }

    void update(float deltaTime)
    {
        // Verlet integration step: move segments based on gravity
        for (size_t i = 1; i < segments.size(); i++) // Ignore the anchor (fixed point)
        {
            Vector2 velocity = segments[i].position - segments[i].oldPosition;
            segments[i].oldPosition = segments[i].position;
            segments[i].position += velocity;
            segments[i].position.y += g * deltaTime * deltaTime; // Apply gravity
        }

        // Apply constraints (make sure segments stay at fixed length)
        for (int j = 0; j < constraintIterations; j++)
        {
            segments[0].position = anchor; // Keep the first segment fixed

            for (size_t i = 0; i < segments.size() - 1; i++)
            {
                Vector2 diff = segments[i + 1].position - segments[i].position;
                float dist = Vector2::Length(diff);
                float error = segmentLength - dist;
                Vector2 correction = Vector2::Normalize(diff) * (error * 0.5f);

                if (i > 0) // Do not move the fixed anchor
                    segments[i].position -= correction;
                segments[i + 1].position += correction;
            }
        }
    }

    void draw(Screen &screen)
    {
        for (size_t i = 0; i < segments.size() - 1; i++)
        {
            DrawLine(screen, segments[i].position.x, segments[i].position.y, segments[i + 1].position.x, segments[i + 1].position.y, Color::White);
        }
    }
};