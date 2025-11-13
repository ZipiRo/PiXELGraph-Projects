#pragma once

const float infinity = 3.4028234e38f;

void PointSegmentDistance(Vector2 point, Vector2 a, Vector2 b, 
    float& distanceSquared, Vector2& closestPoint)
{
    Vector2 AB = b - a;
    Vector2 AP = point - a;

    float projection = Vector2::DotProduct(AP, AB);
    float ABLengthSquared = VectorMath::LengthSquared(AB);
    float t = projection / ABLengthSquared;

    if(t <= 0)
    {
        closestPoint = a;
    }
    else if(t >= 1.0f)
    {
        closestPoint = b;
    }
    else
    {
        closestPoint = a + AB * t;
    }

    distanceSquared = VectorMath::DistanceSquared(point, closestPoint);
}
void ProjectVertices(std::vector<Vector2>vertices, Vector2 axis, float& min, float& max)
{
    min = infinity;
    max = -infinity;

    for (Vector2 vertex : vertices)
    {
        float projection = Vector2::DotProduct(vertex, axis);
        
        if(projection < min) min = projection;
        if(projection > max) max = projection;   
    }
}
void ProjectCircle(Vector2 center, float radius, Vector2 axis, float& min, float& max)
{
    Vector2 direction = Vector2::Normalize(axis);
    Vector2 directionRadius = direction * radius;

    Vector2 point1 = center + directionRadius;
    Vector2 point2 = center - directionRadius;

    min = Vector2::DotProduct(point1, axis);
    max = Vector2::DotProduct(point2, axis);

    if(min > max)
    {
        float aux = min;
        min = max;
        max = aux;
    }
}
int ClosestPointOnPolygon(Vector2 center, std::vector<Vector2> vertices)
{
    int result = -1, i = 0;
    float minDistance = infinity;


    for(Vector2 vertex : vertices)
    {
        i++;
        float distance = Vector2::Distance(vertex, center);

        if(distance < minDistance)
        {
            minDistance = distance;
            result = i;
        }
    }      

    return result;
}

void FindCircleContactPoint(Vector2 centerA, float radiusA, Vector2 centerB, 
    Vector2& contactPoint)
{
    contactPoint = Vector2(0, 0);

    Vector2 AB = centerB - centerA;
    Vector2 direction = Vector2::Normalize(AB);

    contactPoint = centerA + direction * radiusA;
}

void FindCirclePolygonContactPoint(Vector2 circleCenter, float radius, std::vector<Vector2>vertices, Vector2 polygonCenter, 
    Vector2& contactPoint)
{   
    contactPoint = Vector2(0, 0);
    float min_distanceSquared = infinity;

    for(auto vertex = vertices.begin(); vertex != vertices.end(); ++vertex)
    {
        auto next_vertex = std::next(vertex);
        if(next_vertex == vertices.end())
            next_vertex = vertices.begin(); 

        Vector2 vertexA = *vertex;
        Vector2 vertexB = *next_vertex;

        float distanceSquared;
        Vector2 closestPoint;
        PointSegmentDistance(circleCenter, vertexA, vertexB, distanceSquared, closestPoint);

        if(distanceSquared < min_distanceSquared)
        {
            min_distanceSquared = distanceSquared;
            contactPoint = closestPoint;
        }
    }
}

void FindPolygonsContactPoints(std::vector<Vector2>verticesA, std::vector<Vector2>verticesB, 
    Vector2& contactOne, Vector2& contactTwo, int& contactCount)
{
    contactOne = Vector2(0, 0);
    contactTwo = Vector2(0, 0);
    contactCount = 0;

    float min_distanceSquared = infinity;

    for(auto vertex_it = verticesA.begin(); vertex_it != verticesA.end(); ++vertex_it)
    {
        Vector2 point = *vertex_it;

        for(auto vertex_jt = verticesB.begin(); vertex_jt != verticesB.end(); ++vertex_jt)
        {
            auto next_vertex_jt = std::next(vertex_jt);
            if(next_vertex_jt == verticesB.end())
                next_vertex_jt = verticesB.begin(); 

            Vector2 vertexA = *vertex_jt;
            Vector2 vertexB = *next_vertex_jt;

            float distanceSquared;
            Vector2 closestPoint;
            PointSegmentDistance(point, vertexA, vertexB, distanceSquared, closestPoint);
            
            if(NearlyEqual(distanceSquared, min_distanceSquared))
            {
                if(!VectorMath::NearlyEqualVectors(closestPoint, contactOne))
                {
                    contactTwo = closestPoint;
                    contactCount = 2;
                }
            }
            else if(distanceSquared < min_distanceSquared)
            {
                min_distanceSquared = distanceSquared;
                contactOne = closestPoint;
                contactCount = 1;
            }
        }
    }
    
    for(auto vertex_it = verticesB.begin(); vertex_it != verticesB.end(); ++vertex_it)
    {
        Vector2 point = *vertex_it;

        for(auto vertex_jt = verticesA.begin(); vertex_jt != verticesA.end(); ++vertex_jt)
        {
            auto next_vertex_jt = std::next(vertex_jt);
            if(next_vertex_jt == verticesA.end())
                next_vertex_jt = verticesA.begin(); 

            Vector2 vertexA = *vertex_jt;
            Vector2 vertexB = *next_vertex_jt;

            float distanceSquared;
            Vector2 closestPoint;
            PointSegmentDistance(point, vertexA, vertexB, distanceSquared, closestPoint);
            
            if(NearlyEqual(distanceSquared, min_distanceSquared))
            {
                if(!VectorMath::NearlyEqualVectors(closestPoint, contactOne))
                {
                    contactTwo = closestPoint;
                    contactCount = 2;
                }
            }
            else if(distanceSquared < min_distanceSquared)
            {
                min_distanceSquared = distanceSquared;
                contactOne = closestPoint;
                contactCount = 1;
            }
        }
    }
}

bool IntersectAABB(AABB a, AABB b) {
    if (a.right <= b.left || b.right <= a.left) return false;
    if (a.top <= b.bottom || b.top <= a.bottom) return false;

    return true;
}

bool IntersectCircles(Vector2 centerA, float radiusA, Vector2 centerB, float radiusB, 
    Vector2 &normal, float &depth) 
{
    normal = Vector2(0, 0);
    depth = 0.0f;

    float distance = Vector2::Distance(centerA, centerB);
    float radiI = radiusA + radiusB;

    if(distance >= radiI)
    {
        return false;
    }

    normal = centerB - centerA;
    normal = Vector2::Normalize(normal);
    depth = radiI - distance;

    return true;
}

bool IntersectPolygons(std::vector<Vector2>verticesA, Vector2 centerA, std::vector<Vector2>verticesB, Vector2 centerB, 
    Vector2 &normal, float &depth)
{
    normal = Vector2(0, 0);
    depth = infinity;
    
    Vector2 axis = Vector2(0, 0);
    float minA, minB, maxA, maxB;
    
    for(auto vertex = verticesA.begin(); vertex != verticesA.end(); ++vertex)
    {
        auto next_vertex = std::next(vertex);
        if(next_vertex == verticesA.end())
            next_vertex = verticesA.begin(); 

        Vector2 vertexA = *vertex;
        Vector2 vertexB = *next_vertex;

        Vector2 edge = vertexB - vertexA;
        axis = Vector2(edge.y, -edge.x);
        axis = Vector2::Normalize(axis);

        ProjectVertices(verticesA, axis, minA, maxA);
        ProjectVertices(verticesB, axis, minB, maxB);

        if(minA >= maxB || minB >= maxA)
        {
            return false;
        }

        float axisDepth = minF(maxB - minA, maxA - minB);

        if(axisDepth < depth) 
        {
            depth = axisDepth;
            normal = axis;
        }
    }

    for(auto vertex = verticesB.begin(); vertex != verticesB.end(); ++vertex)
    {
        auto next_vertex = std::next(vertex);
        if(next_vertex == verticesA.end())
            next_vertex = verticesA.begin(); 

        Vector2 vertexA = *vertex;
        Vector2 vertexB = *next_vertex;

        Vector2 edge = vertexB - vertexA;
        axis = Vector2(edge.y, -edge.x);
        axis = Vector2::Normalize(axis);

        ProjectVertices(verticesA, axis, minA, maxA);
        ProjectVertices(verticesB, axis, minB, maxB);

        if(minA >= maxB || minB >= maxA)
        {
            return false;
        }

        float axisDepth = minF(maxB - minA, maxA - minB);

        if(axisDepth < depth) 
        {
            depth = axisDepth;
            normal = axis;
        }
    }

    Vector2 direction = centerB - centerA;

    if(Vector2::DotProduct(direction, normal) < 0.0f)
    {
        normal = -normal;
    }

    return true;
}

bool IntersectCirclesPolygons(Vector2 circleCenter, float radius, std::vector<Vector2>vertices, Vector2 polygonCenter,
    Vector2 &normal, float &depth)
{
    normal = Vector2(0, 0);
    depth = infinity;

    Vector2 axis = Vector2(0, 0);
    float minA, minB, maxA, maxB;

    for(auto vertex = vertices.begin(); vertex != vertices.end(); ++vertex)
    {
        
        auto next_vertex = std::next(vertex);
        if(next_vertex == vertices.end())
            next_vertex = vertices.begin(); 

        Vector2 vertexA = *vertex;
        Vector2 vertexB = *next_vertex;

        Vector2 edge = vertexB - vertexA;
        axis = Vector2(edge.y, -edge.x);
        axis = Vector2::Normalize(axis);

        ProjectVertices(vertices, axis, minA, maxA);
        ProjectCircle(circleCenter, radius, axis, minB, maxB);

        if(minA >= maxB || minB >= maxA)
        {
            return false;
        }

        float axisDepth = minF(maxB - minA, maxA - minB);

        if(axisDepth < depth) 
        {
            depth = axisDepth;
            normal = axis;
        }
    }

    int closestPoint_Index = ClosestPointOnPolygon(circleCenter, vertices);

    int index = 0; Vector2 closestPoint;
    for(Vector2 v : vertices)
        if(closestPoint_Index == ++index) { closestPoint = v; break; }

    axis = closestPoint - circleCenter;
    axis = Vector2::Normalize(axis);

    ProjectVertices(vertices, axis, minA, maxA);
    ProjectCircle(circleCenter, radius, axis, minB, maxB);

    if(minA >= maxB || minB >= maxA)
    {
        return false;
    }

    float axisDepth = minF(maxB - minA, maxA - minB);

    if(axisDepth < depth) 
    {
        depth = axisDepth;
        normal = axis;
    }

    Vector2 direction = polygonCenter - circleCenter;

    if(Vector2::DotProduct(direction, normal) < 0.0f)
    {
        normal = -normal;
    }

    return true;
}