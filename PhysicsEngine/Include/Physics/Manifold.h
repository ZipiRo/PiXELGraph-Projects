#pragma once

class Manifold
{
    public:
        Body *bodyA;
        Body *bodyB;
        Vector2 normal;
        float depth;
        Vector2 contactOne;
        Vector2 contactTwo;
        int contactCount;

        Manifold(Body *bodyA, Body *bodyB, Vector2 normal, float depth, Vector2 contactOne, Vector2 contactTwo, int contactCount)
        {
            this->bodyA = bodyA;
            this->bodyB = bodyB;
            this->normal = normal;
            this->depth = depth;
            this->contactOne = contactOne;
            this->contactTwo = contactTwo;
            this->contactCount = contactCount;
        }
};