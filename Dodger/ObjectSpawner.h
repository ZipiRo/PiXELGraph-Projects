#pragma once

struct SpawnerParams
{
    bool anyDstroyedObject = false;
    Color destroyedObject_color;
    int destroyedObjectsCount = 0;
};

class ObjectSpawner
{
private:
    std::list<FallingObject> objects;
    float spawnTimer;
    float spawnStartTime;
    int objectsPerTickRange;

    float spawnWidth;
    float bottomBorder;

public:
    SpawnerParams params;
    ParticleSystem particleSystem;

    ObjectSpawner() {}
    ObjectSpawner(float spawnWidth, float bottomBorder, int objectsPerTickRange)
    {
        this->spawnWidth = spawnWidth;
        this->bottomBorder = bottomBorder;
        this->objectsPerTickRange = objectsPerTickRange;
        
        particleSystem = ParticleSystem(30, .3);
        
        spawnStartTime = 1;
        spawnTimer = 0;
    }

    void Update(float deltaTime, Box playerBox)
    {
        spawnTimer += deltaTime;

        // Spawner Update
        if(spawnTimer > spawnStartTime)
        {
            for(int i = 1; i <= IRandomRange(1, objectsPerTickRange); i++)
            {
                FallingObject object(IRandomRange(0, spawnWidth), -10, 15, 15, IRandomRange(3, 10));
                object.SetObjectParams(IRandomRange(100, 150), IRandomRange(180, 360) * DEG_TO_RAD);
                object.SetFillColor(RandomColor());

                objects.push_back(object);
            }

            spawnTimer = 0;
            // spawnStartTime = IRandomRange(8, 11) / 10; // Really Cool Addition
        }

        // Falling Objects Update
        for (auto object_it = objects.begin(); object_it != objects.end();)
        {
            FallingObject& object = *object_it;

            object.Update(deltaTime);

            if (object.GetShapeBox().top > bottomBorder)
            {
                object_it = objects.erase(object_it);
            }
            else if(CollisionDetection(playerBox, object.GetShapeBox()))
            {
                params.anyDstroyedObject = true;
                params.destroyedObjectsCount += 1;
                params.destroyedObject_color = object.GetFillColor();
                
                particleSystem.SetPosition(object.GetPosition());
                particleSystem.RandomDirection(object.GetFillColor());

                object_it = objects.erase(object_it);
            }
            else
            {
                ++object_it;
            }
        }
    }

    bool CollisionDetection(Box playerBox, Box objectBox)
    {
        if(playerBox.right > objectBox.left && playerBox.left < objectBox.right &&
            playerBox.top + 4 < objectBox.bottom && playerBox.bottom > objectBox.top) return true;

        return false;
    }

    void Draw(Screen &screen)
    {
        for(auto &object : objects)
            object.Draw(screen);
    }
};