#pragma once

#include <Core/PiXELGraph.h>

#include "Ball.h"

#include "Collision.h"
#include "QuadTree.h"

class GameScene : public PiXELGraph
{
private:
    Vector2 gravity;

    Vector2 worldMousePos;
    AABB worldViewBounds;

    std::vector<Ball> balls;

    QuadTree quadTree;

    float fixedDeltaTime = 1.0f / 60.0f;

    void StepBodies(float deltaTime, int totalIterations)
    {
        deltaTime /= totalIterations;
        for(auto &ball : balls)
        {
            ball.AddForce(gravity * ball.mass);
            ball.Step(deltaTime);
        }
    }

    void Start() override
    {
        srand(std::time(0));

        Screen::GetView().SetScreenCenter(Vector2(Screen::Width() / 2, Screen::Height() / 2));
        Screen::GetView().SetZoom(0.5f);

        Vector2 WorldBoundsSize(500, 500);
        worldViewBounds = AABB(-WorldBoundsSize.x / 2, -WorldBoundsSize.y / 2, WorldBoundsSize.x / 2, WorldBoundsSize.y / 2);

        RectangleBoundry rectangleBoundry(0, 0, WorldBoundsSize.x, WorldBoundsSize.x);

        quadTree = QuadTree(rectangleBoundry, 4);

        gravity = Vector2(0.0f, 9.81f);

        for (int i = 0; i < 200; i++)
        {
            int randomAngle = (rand() % 360) * DEG_TO_RAD;
            Vector2 direction = Vector2(cos(randomAngle), sin(randomAngle));
            int randomLength = 50 + rand() % (100 - 50 + 1);
            Vector2 randomPosition = direction * randomLength;

            Ball ball(randomPosition.x, randomPosition.y, 5.0f, 1.0f, 1.0f);
            ball.SetColor(Color::RandomColor());
            // ball.showVelocity = true;

            balls.push_back(ball);
        }
    }

    float timer = 1;

    void Update() override
    {
        worldMousePos = Screen::GetView().ScreenToWorld(Input::MousePosition / Window::WindowFontSize());
        timer += Time::deltaTime;
        if (timer >= 1)
        {
            Debug::Log(std::to_string((int)(1.0f / Time::deltaTime)));
            timer = 0;
        }

        if(Input::IsKey(Key_W))
            Screen::GetView().Move(Vector2(0, -1) * 4.0f);

        if(Input::IsKey(Key_S))
            Screen::GetView().Move(Vector2(0, 1) * 4.0f);

        if(Input::IsKey(Key_A))
            Screen::GetView().Move(Vector2(-1, 0) * 4.0f);

        if(Input::IsKey(Key_D))
            Screen::GetView().Move(Vector2(1, 0) * 4.0f);

        if(Input::IsKey(Key_Q) && Screen::GetView().GetZoom() >= 0.01f)
            Screen::GetView().Zoom(-0.05f);
            
        if(Input::IsKey(Key_E))
            Screen::GetView().Zoom(0.05f);
            
        int totalIterations = 8;
        for (int currentIteration = 1; currentIteration <= totalIterations; currentIteration++)
        {
            StepBodies(Time::deltaTime, totalIterations);
            
            quadTree.Clear();
            for(auto &ball : balls)
                quadTree.Insert({ball.GetPosition(), &ball});

            for(auto &ballA : balls)
            {
                std::vector<Point> querryBalls;
                quadTree.Querry(querryBalls, ballA.GetAABB());

                for(auto &querryBall : querryBalls)
                {
                    Ball *ballB = querryBall.ball;

                    if(ballB == &ballA) continue;

                    Vector2 normal; float depth;
                    if (!IntersectAABB(ballA, *ballB)) continue;
                    if (!IntersectCircles(ballA, *ballB, normal, depth)) continue;
                    if (normal == Vector2::ZERO) continue;
                    
                    ballA.Move((-normal * depth) / 2.0f);
                    ballB->Move((normal * depth) / 2.0f);

                    ResolveBasicCollision(ballA, *ballB, normal);
                }
            }

            for (auto &ball : balls)
                CollideWorldBounds(ball, worldViewBounds);
        }
    }

    void Draw() override
    {
        for (auto &ball : balls) ball.Draw();

        DrawLines({{{worldViewBounds.left, worldViewBounds.bottom}, Color::Blue},
                   {{worldViewBounds.right, worldViewBounds.bottom}, Color::Blue}});

        DrawLines({{{worldViewBounds.left, worldViewBounds.bottom}, Color::Blue},
                   {{worldViewBounds.left, worldViewBounds.top}, Color::Blue}});

        DrawLines({{{worldViewBounds.right, worldViewBounds.bottom}, Color::Blue},
                   {{worldViewBounds.right, worldViewBounds.top}, Color::Blue}});

        DrawLines({{{worldViewBounds.left, worldViewBounds.top}, Color::Blue},
                   {{worldViewBounds.right, worldViewBounds.top}, Color::Blue}});

        // quadTree.ShowSection();
        // quadTree.ShowPoints();
    }

    void Quit() override
    {
        quadTree.Clear();
    }
public:
    GameScene()
    {
        Screen::BacgroundColor = Color::Black;
        MaxFPS = 60;
        Init(1280, 720, 2, L"Game");
    }
};