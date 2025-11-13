#pragma once

void HandleBoundCollision(Ball &ball, const Vector2 &normal)
{
    Vector2 position = ball.GetPosition();
    float radius = ball.GetRadius();

    float vDotNormal = Vector2::DotProduct(ball.linearVelocity, normal);
    if (vDotNormal < 0.0f)
    {
        ball.linearVelocity -= normal * vDotNormal * (1.0f + ball.restitution);
        ball.linearVelocity *= 0.98f;
    }
}

void CollideWorldBounds(Ball &ball, const AABB &worldViewBounds)
{
    AABB ballBoundingBox = ball.GetAABB();
    Vector2 position = ball.GetPosition();
    float radius = ball.GetRadius();

    if (ballBoundingBox.left < worldViewBounds.left)
    {
        ball.SetPosition({worldViewBounds.left + radius, position.y});
        HandleBoundCollision(ball, Vector2(1, 0));
    }
    else if (ballBoundingBox.right > worldViewBounds.right)
    {
        ball.SetPosition({worldViewBounds.right - radius, position.y});
        HandleBoundCollision(ball, Vector2(-1, 0));
    }

    if (ballBoundingBox.bottom > worldViewBounds.bottom)
    {
        ball.SetPosition({position.x, worldViewBounds.bottom - radius});
        HandleBoundCollision(ball, Vector2(0, -1));
    }
    else if (ballBoundingBox.top < worldViewBounds.top)
    {
        ball.SetPosition({position.x, worldViewBounds.top + radius});
        HandleBoundCollision(ball, Vector2(0, 1));
    }
}

void ResolveBasicCollision(Ball &ballA, Ball &ballB, const Vector2 &normal)
{
    Vector2 relativeVelocity = ballB.linearVelocity - ballA.linearVelocity;

    if (Vector2::DotProduct(relativeVelocity, normal) > 0.0f)
        return;

    float e = std::min(ballA.restitution, ballB.restitution);

    float j = -(1 + e) * Vector2::DotProduct(relativeVelocity, normal);
    j = j / (ballA.inverseMass + ballB.inverseMass);

    Vector2 impulse = normal * j;

    ballA.linearVelocity -= impulse * ballA.inverseMass;
    ballB.linearVelocity += impulse * ballB.inverseMass;
}

bool IntersectAABB(Ball &ballA, Ball &ballB)
{
    AABB ballAbounds = ballA.GetAABB();
    AABB ballBbounds = ballB.GetAABB();
    
    return ballAbounds.right >= ballBbounds.left && ballAbounds.left <= ballBbounds.right &&
            ballAbounds.bottom >= ballBbounds.top && ballAbounds.top <= ballBbounds.bottom;
}

bool IntersectCircles(Ball &ballA, Ball &ballB, Vector2 &normal, float &depth)
{
    normal = Vector2::ZERO;
    depth = 0.0f;

    Vector2 ballAPos = ballA.GetPosition();
    Vector2 ballBPos = ballB.GetPosition();

    float ballARadius = ballA.GetRadius();
    float ballBRadius = ballB.GetRadius();

    Vector2 delta = ballBPos - ballAPos;
    float distanceSquared = delta.x * delta.x + delta.y * delta.y;
    float radiI = ballARadius + ballBRadius;

    if (distanceSquared >= radiI * radiI)
        return false;

    float distance = sqrtf(distanceSquared); 
    
    if(distance == 0.0f)
    {
        normal = Vector2::RIGHT;
        depth = radiI;
        return true;
    }

    normal = delta / distance;
    depth = radiI - distance;

    return true;
}