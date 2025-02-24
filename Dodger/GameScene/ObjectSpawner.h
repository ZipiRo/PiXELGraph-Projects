#pragma once

class ObjectSpawner
{
private:
    struct MinMax
    {
        float min, max;
    };

    struct SpawnerParam
    {
        bool anyDstroyedObject = false;
        Color destroyedObject_color;
        int destroyedObjectsCount = 0;
    };

    struct SpawnerStage
    {
        MinMax objPerTick;
        MinMax spawnTime;
        MinMax obj_FallSpeed;
        MinMax obj_Size;

        SpawnerStage() {}
        SpawnerStage(MinMax objPerTick, MinMax spawnTime, MinMax obj_FallSpeed, MinMax obj_Size)
        {
            this->objPerTick = objPerTick;
            this->spawnTime = spawnTime;
            this->obj_FallSpeed = obj_FallSpeed;
            this->obj_Size = obj_Size;
        }
    };

    std::list<FallingObject> objects;
    float spawnTimer;
    float spawnStartTime;

    int nextStageAmount = 50;

    float spawnWidth;
    float bottomBorder;
    SpawnerStage stage;
public:
    SpawnerParam params;
    ParticleSystem particleSystem;

    ObjectSpawner() {}
    ObjectSpawner(float spawnWidth, float bottomBorder)
    {
        this->spawnWidth = spawnWidth;
        this->bottomBorder = bottomBorder;
        
        this->stage = SpawnerStage({1, 1}, {10, 10}, {70, 130}, {15, 20});

        particleSystem = ParticleSystem(30, .3);
        
        spawnStartTime = stage.spawnTime.max / 10;
        spawnTimer = 0;
    }

    void Update(float deltaTime, Box playerBox)
    {
        spawnTimer += deltaTime;

        // Spawner Update
        if(spawnTimer > spawnStartTime)
        {
            for(int i = 1; i <= IRandomRange(stage.objPerTick.min, stage.objPerTick.max); i++)
            {
                float objectSize = IRandomRange(stage.obj_Size.min, stage.obj_Size.max);

                FallingObject object(IRandomRange(0, spawnWidth), -10, objectSize, objectSize, IRandomRange(3, 10));
                object.SetObjectParams(IRandomRange(stage.obj_FallSpeed.min, stage.obj_FallSpeed.max), IRandomRange(120, 360) * DEG_TO_RAD);
                object.SetFillColor(RandomColor());

                objects.push_back(object);
            }

            spawnTimer = 0;
            spawnStartTime = float(IRandomRange(stage.spawnTime.min, stage.spawnTime.max)) / 10;
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

                
                if((params.destroyedObjectsCount + 1) % nextStageAmount == 0)
                    stage = CreateNextStage();
            }
            else
            {
                ++object_it;
            }
        }
    }

    SpawnerStage CreateNextStage()
    {
        SpawnerStage newStage;
        
        newStage.spawnTime.min = std::max(stage.spawnTime.min * 0.9, 1.5);
        newStage.spawnTime.max = std::max(stage.spawnTime.max * 0.9, 2.0);

        newStage.obj_FallSpeed.min = std::min(stage.obj_FallSpeed.min + 5.0f, 350.0f);
        newStage.obj_FallSpeed.max = std::min(stage.obj_FallSpeed.max + 7.5f, 500.0f);

        newStage.objPerTick.min = std::min(stage.objPerTick.min + stage.objPerTick.min * 0.09f, 2.0f);
        newStage.objPerTick.max = std::min(stage.objPerTick.max + stage.objPerTick.max * 0.09f, 3.0f);

        newStage.obj_Size.min = std::max(stage.obj_Size.min - 0.2f, 10.0f);
        newStage.obj_Size.max = std::max(stage.obj_Size.max - 0.2f, stage.obj_Size.min);

        return newStage;
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