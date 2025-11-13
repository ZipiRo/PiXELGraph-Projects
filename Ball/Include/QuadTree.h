#pragma once

struct Point
{
    Vector2 position;
    Ball *ball;
};

class QuadTree;

class RectangleBoundry
{
private:
    Vector2 position;
    float width;
    float height;

    friend class QuadTree;

public:
    RectangleBoundry()
    {
        position = Vector2::ZERO;
        width = 0;
        height = 0;
    }

    RectangleBoundry(float x, float y, float width, float height)
    {
        position = Vector2(x, y);
        this->width = width;
        this->height = height;
    }

    bool Contains(const Vector2 &pointPosition)
    {
        return pointPosition.x >= position.x - width / 2 && // left 
                pointPosition.y >= position.y - height / 2 && // top
                pointPosition.x <= position.x + width / 2 && // right
                pointPosition.y <= position.y + height / 2; // bottom
    }

    bool Intersects(const AABB &boundingBox)
    {
        float left = position.x - width / 2;
        float right = position.x + width / 2;
        float top = position.y + height / 2;
        float bottom = position.y - height / 2;
        
        return !(right <= boundingBox.left || left >= boundingBox.right || top <= boundingBox.bottom || bottom >= boundingBox.top);     
    }
};

class QuadTree
{
private:
    RectangleBoundry boundry;
    int capacity;

    bool divided;
    std::vector<Point> points;

    QuadTree *topleft;
    QuadTree *topright;
    QuadTree *bottomleft;
    QuadTree *bottomright;

    Color color;

    void Subdivide()
    {
        float x = boundry.position.x;
        float y = boundry.position.y;
        float half_width = boundry.width / 2.0f;
        float half_height = boundry.height / 2.0f;

        RectangleBoundry boundry_topleft(x - half_width / 2, y - half_height / 2, half_width, half_height);
        topleft = new QuadTree(boundry_topleft, capacity); 

        RectangleBoundry boundry_topright(x + half_width / 2, y - half_height / 2, half_width, half_height);
        topright = new QuadTree(boundry_topright, capacity);
        
        RectangleBoundry boundry_bottomleft(x - half_width / 2, y + half_height / 2, half_width, half_height);
        bottomleft = new QuadTree(boundry_bottomleft, capacity);
        
        RectangleBoundry boundry_bottomright(x + half_width / 2, y + half_height / 2, half_width, half_height);
        bottomright = new QuadTree(boundry_bottomright, capacity);

        divided = true;
    }

public:
    QuadTree()
    {
        points.reserve(0);
        
        topleft = nullptr;
        topright = nullptr;
        bottomleft = nullptr;
        bottomright = nullptr;

        divided = false;
        capacity = 0;
    }

    QuadTree(const RectangleBoundry &boundry, int capacity)
    {
        points.reserve(capacity);
        this->boundry = boundry;

        topleft = nullptr;
        topright = nullptr;
        bottomleft = nullptr;
        bottomright = nullptr;

        divided = false;
        this->capacity = capacity;
    }

    bool Insert(const Point &point)
    {
        if(!boundry.Contains(point.position)) return false; // check if the point is in the bounds

        if(points.size() < capacity) points.push_back(point); // add the point if the capacity is not exceded
        else 
        {
            if(!divided)    
                Subdivide(); // divide if the tree is not divided

            // check if the point is in any of the sections
            if(topleft->Insert(point)) return true;
            if(topright->Insert(point)) return true;
            if(bottomleft->Insert(point)) return true;
            if(bottomright->Insert(point)) return true;
        }

        return true;
    }

    void Clear()
    {
        points.clear();
        if(!divided) return;

        if (topleft) { topleft->Clear(); delete topleft; topleft = nullptr; }
        if (topright) { topright->Clear(); delete topright; topright = nullptr; }
        if (bottomleft) { bottomleft->Clear(); delete bottomleft; bottomleft = nullptr; }
        if (bottomright) { bottomright->Clear(); delete bottomright; bottomright = nullptr; }
        
        divided = false;
    }

    void Querry(std::vector<Point> &querryPoints, const AABB &boundingBox)
    {
        if(!boundry.Intersects(boundingBox)) return;

        for(const auto &point : points)
            if(boundry.Contains(point.position))
                querryPoints.push_back(point);
            
        if(!divided) return;

        topleft->Querry(querryPoints, boundingBox);
        topright->Querry(querryPoints, boundingBox);
        bottomleft->Querry(querryPoints, boundingBox);
        bottomright->Querry(querryPoints, boundingBox);
    }

    void ShowSection()
    {
        Shapes::Rectangle section(boundry.width, boundry.height);
        section.transform.SetPosition(boundry.position);
        section.SetColor(Color(0, 255, 0, 0.3));

        DrawShape(section);

        if(!divided) return;

        topleft->ShowSection();
        topright->ShowSection();
        bottomleft->ShowSection();
        bottomright->ShowSection();
    }

    void ShowPoints()
    {
        Shapes::Elipse p(2, 2, 3);

        for(const auto& point : points)
        {
            p.transform.SetPosition(point.position);
            p.SetColor(Color::Red);

            DrawShape(p);
        }

        if(!divided) return;

        topleft->ShowPoints();
        topright->ShowPoints();
        bottomleft->ShowPoints();
        bottomright->ShowPoints();
    }
};