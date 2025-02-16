#pragma once

class Particle
{
public:
    Vector2 position;
    Vector2 direction;
    float magnitude;
    Color color;
    float alivetimer;
    
    Particle () {}
    Particle (Vector2 position, Vector2 direction, float magnitude, Color color)
    {
        this->position = position;
        this->direction = direction;
        this->magnitude = magnitude;
        this->color = color;
        this->alivetimer = 0;
    }
};

class ParticleSystem
{
private:
    std::list<Particle> particles;
    Vector2 position;
    
    int particleCount;
    float aliveTime;

public:    
    ParticleSystem () {}
    ParticleSystem (int count, float aliveTime)
    {
        this->position = Vector2::ZERO;
        this->particleCount = count;
        this->aliveTime = aliveTime;
    }

    void Radial(float magnitude)
    {
        float angle_it = (2 * PI) / particleCount;

        for(float angle = 0; angle < 2 * PI; angle += angle_it)
        {
            Vector2 direction(cos(angle), sin(angle));
            Particle new_Particle(position, direction, magnitude, Color::Blue);

            this->particles.emplace_back(new_Particle);
        }
    }

    void RandomDirection(Color color)
    {
        srand(time(NULL));

        for(int i = 0; i < particleCount; i++)
        {
            int neg = rand() % 2;
            float x = float(1 + rand() % (100 - 1 + 1)) / 100.0f * (!neg ? -1 : 1);
            neg = rand() % 2;
            float y = float(1 + rand() % (100 - 1 + 1)) / 100.0f * (!neg ? -1 : 1);

            float magnitude = 40 + rand() % (140 - 40 + 1);

            Vector2 direction = Vector2(x, y);
            direction = Vector2::Normalize(direction);
            Particle new_Particle(position, direction, magnitude, color);

            this->particles.emplace_back(new_Particle);
        }
    }

    void Update(float deltaTime, Box screenBounds)
    {  
        for (auto particle_it = particles.begin(); particle_it != particles.end();)
        {
            Particle& particle = *particle_it;

            if (particle.alivetimer > aliveTime ||
                particle.position.x > screenBounds.right || 
                particle.position.x < screenBounds.left ||
                particle.position.y > screenBounds.bottom || 
                particle.position.y < screenBounds.top)
            {
                particle_it = particles.erase(particle_it);
            }
            else
            {
                ++particle_it;
            }
        }


        for(Particle &particle : this->particles)
        {
            Vector2 direction = particle.direction;
            float magnitude = particle.magnitude;

            particle.position += direction * magnitude * deltaTime; 

            particle.alivetimer += deltaTime;
        }      
    }

    void Draw(Screen &screen)
    {
        for(const Particle &particle : particles)
        {
            screen.PlotPixel(particle.position.x, particle.position.y, particle.color);
            screen.PlotPixel(particle.position.x  -1, particle.position.y, Color::Black);
            screen.PlotPixel(particle.position.x, particle.position.y - 1, particle.color);
        }
    }

    void Reset()
    {
        this->particles.clear();
    }

    void SetPosition(Vector2 position)
    {
        this->position = position;
    }
};