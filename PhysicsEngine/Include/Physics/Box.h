#pragma once

class Box : public Body
{
    public:
        Box (float width, float height, Vector2 position, float density, float restitution, Color fillColor, Color outlineColor, bool isStatic)
        {
            this->position = position;
            this->linearVelocity = Vector2::ZERO;
            this->angle = 0.0f;
            this->angularVelocity = 0.0f;
    
            this->force = Vector2::ZERO;
    
            this->width = width;
            this->height = height;
            this->surface = this->width * this->height;
            this->density = density;
            this->mass = isStatic ? 0.0f : this->surface * this->density;
            this->inertia = isStatic ? 0.0f : (1.0f / 12.0f) * this->mass * (this->height * this->height + this->width * this->width);
            this->restitution = restitution;
            this->staticFriction = 0.6f;
            this->dinamicFriction = 0.4f;
            
            this->invMass = isStatic ? 0.0f : 1.0f / mass;
            this->invInertia = isStatic ? 0.0f : 1.0f / inertia;
    
            this->fillColor = fillColor;
            this->outlineColor = outlineColor;
    
            this->isStatic = isStatic;
    
            this->shapeType = Body::PolygonShape;
            
            this->vertices = CreateRectangleVertices(this->width, this->height);
            this->transformVertices = this->vertices;
    
            this->aabb = UpdateRectangleAABB(this->vertices);
    
            this->rectangleShape = Shapes::Rectangle(this->width, this->height);
            this->rectangleShape.SetFillColor(this->fillColor);
            this->rectangleShape.SetColor(this->outlineColor);
            this->shape = &this->rectangleShape;
    
            this->UPDATE_VERTICES = true;
            this->UPDATE_AABB = true;
        }
    
        void Draw() override
        {   
            if(this->UPDATE_VERTICES)
                this->transformVertices = UpdateRectangleVertices(this->vertices, this->position, this->angle);
    
            if(this->UPDATE_AABB)
                this->aabb = UpdateRectangleAABB(this->GetTransformedVertices());
    
            this->rectangleShape.transform.SetPosition(this->position);
            this->rectangleShape.transform.SetAngle(this->angle);
    
            DrawShape(rectangleShape);
    
            this->UPDATE_VERTICES = false;
        }
    
        std::vector<Vector2> GetTransformedVertices() override
        {
            if(this->UPDATE_VERTICES)
                this->transformVertices = UpdateRectangleVertices(this->vertices, this->position, this->angle);
    
            this->UPDATE_VERTICES = false;
            return this->transformVertices;
        }   
    
        AABB GetAABB() override
        {
            if(this->UPDATE_AABB)
                this->aabb = UpdateRectangleAABB(this->GetTransformedVertices());
    
            this->UPDATE_AABB = false;
            return this->aabb;
        }

    private:
        std::vector<Vector2> CreateRectangleVertices(float width, float height)
        {
            std::vector<Vector2> vertices;
    
            float left = -width / 2.0f;
            float right = left + width;
            float bottom = -height / 2.0f;
            float top = bottom + height;
    
            vertices.push_back(Vector2(left, top));
            vertices.push_back(Vector2(right, top));
            vertices.push_back(Vector2(right, bottom));
            vertices.push_back(Vector2(left, bottom));
    
            return vertices;
        }
    
        AABB UpdateRectangleAABB(std::vector<Vector2> vertices) 
        {
            const float infinity = 3.4028234e38f;
    
            float minX = infinity;
            float minY = infinity;
            float maxX = -infinity;
            float maxY = -infinity;
    
            for(Vector2 vertex : vertices)
            {
                if(vertex.x < minX) minX = vertex.x;
                if(vertex.x > maxX) maxX = vertex.x;
                
                if(vertex.y < minY) minY = vertex.y;
                if(vertex.y > maxY) maxY = vertex.y;
            }
    
            return AABB(minX, minY, maxX, maxY);
        }
        
        std::vector<Vector2> UpdateRectangleVertices(std::vector<Vector2> vertices, Vector2 position, float angle)
        {
            std::vector<Vector2> transformVertices;
    
            PTransform transform(position, angle);
    
            for(Vector2 v : vertices)
                transformVertices.push_back(VectorMath::VectorTransformZ(v, transform));
    
            return transformVertices;
        }

        Shapes::Rectangle rectangleShape;
        
        std::vector<Vector2> vertices;
        std::vector<Vector2> transformVertices;
        
        AABB aabb;
};