class Spring
{
private:
    Elipse e;

    Vector2 anchor;  // Fixed point
    Vector2 position;  // Mass position
    Vector2 velocity;
    float mass;
    float k;  // Spring stiffness
    float restLength;
    float damping;
    
public:
    Spring() {}

    Spring(Vector2 anchor, Vector2 startPos, float mass, float k, float restLength, float damping)
    {
        this->anchor = anchor;
        this->position = startPos;
        this->velocity = Vector2(0, 0);
        this->mass = mass;
        this->k = k;
        this->restLength = restLength;
        this->damping = damping;

        e = Elipse(0, 0, 3, 3, 5);

        e.SetFillColor(Color::White);
        e.SetOutlineColor(Color::White);
        e.SetPivot({0.5, 0.5});
    }

    void update(float deltaTime)
    {
        // Compute displacement
        Vector2 displacement = position - anchor;
        float length = Vector2::Length(displacement);

        // Normalize displacement direction
        Vector2 direction = Vector2::Normalize(displacement);

        // Hooke's Law: F = -k * (x - restLength)
        float stretch = length - restLength;
        Vector2 force = -k * stretch * direction;

        // Damping force: F = -b * v
        Vector2 dampingForce = -damping * velocity;

        // Net force
        Vector2 netForce = force + dampingForce;

        // Acceleration: F = m * a -> a = F / m
        Vector2 acceleration = netForce / mass;

        // Integrate motion (Euler method)
        velocity += acceleration * deltaTime;
        position += velocity * deltaTime;
    }

    void draw(Screen &screen)
    {
        // Draw the spring
        DrawLine(screen, anchor.x, anchor.y, position.x, position.y, Color::White);

        // Draw the mass as a circle
        e.GetTransform().MoveTo(position);
        e.Draw(screen);
    }
};

class SpringPendulum
{
private:
    Elipse e;
    
    Vector2 anchor;   // Fixed point
    Vector2 position; // Mass position
    Vector2 velocity;
    float mass;
    float k;  // Spring stiffness
    float restLength;
    float damping;

public:
    SpringPendulum() {}
    
    SpringPendulum(Vector2 anchor, Vector2 startPos, float mass, float k, float restLength, float damping)
    {
        this->anchor = anchor;
        this->position = startPos;
        this->velocity = Vector2(0, 0);
        this->mass = mass;
        this->k = k;
        this->restLength = restLength;
        this->damping = damping;

        e = Elipse(0, 0, 3, 3, 5);

        e.SetFillColor(Color::White);
        e.SetOutlineColor(Color::White);
        e.SetPivot({0.5, 0.5});
    }

    void update(float deltaTime)
    {
        // Compute displacement from anchor
        Vector2 displacement = position - anchor;
        float length = Vector2::Length(displacement);

        // Normalize displacement direction
        Vector2 direction = Vector2::Normalize(displacement);

        // Spring force: F = -k * (x - x_rest)
        float stretch = length - restLength;
        Vector2 springForce = -k * stretch * direction;

        // Gravity force: F = mg
        Vector2 gravityForce(0, mass * g);

        // Damping force: F = -b * v
        Vector2 dampingForce = -damping * velocity;

        // Net force
        Vector2 netForce = springForce + gravityForce + dampingForce;

        // Acceleration: F = ma -> a = F / m
        Vector2 acceleration = netForce / mass;

        // Integrate motion (Euler method)
        velocity += acceleration * deltaTime;
        position += velocity * deltaTime;
    }

    void draw(Screen &screen)
    {
        // Draw the spring
        DrawLine(screen, anchor.x, anchor.y, position.x, position.y, Color::White);

        // Draw the mass as a circle
        e.GetTransform().MoveTo(position);
        e.Draw(screen);
    }
};
