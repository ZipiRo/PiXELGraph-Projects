#pragma once

class Body
{
    public:
        enum { PolygonShape, CircleShape};
        
        Vector2 position;
        Vector2 linearVelocity;
        float angle;
        float angularVelocity;

        Vector2 force;

        float width;
        float height;
        float radius;

        float inertia;
        float invInertia;

        float surface;
        float density;
        float mass;
        float invMass;
        float restitution;
        float area;

        float staticFriction;
        float dinamicFriction;

        bool isStatic;

        bool UPDATE_VERTICES;
        bool UPDATE_AABB;

        Color fillColor;
        Color outlineColor;
        Shape* shape;

        int shapeType;

        Body() {};
        
        virtual void Draw() = 0;
        virtual std::vector<Vector2> GetTransformedVertices() = 0;
        virtual AABB GetAABB() = 0;

        void Move(Vector2 amount) {
            this->position += amount;
            this->UPDATE_VERTICES = true;
            this->UPDATE_AABB = true;
        }

        void MoveTo(Vector2 position) {
            this->position = position;
            this->UPDATE_VERTICES = true;
            this->UPDATE_AABB = true;
        }

        void Rotate(float amount) {
            this->angle += amount;
            this->UPDATE_VERTICES = true;
            this->UPDATE_AABB = true;
        }

        void RotateTo(float angle) {
            this->angle = angle;
            this->UPDATE_VERTICES = true;
            this->UPDATE_AABB = true;
        }

        void AddForce(Vector2 amount) {
            this->force += amount;
        }

        void SetFillColor(Color color) {
            this->fillColor = color;
            this->shape->SetFillColor(this->fillColor);
        }

        void SetOutlineColor(Color color) {
            this->outlineColor = color;
            this->shape->SetColor(this->outlineColor);
        }

        void Step(float deltaTime, int totalIterations) {
            if(this->isStatic) return;
            
            deltaTime /= float(totalIterations);

            Vector2 acceleration = this->force / this->mass;

            Debug::Log(std::to_string(position.y));

            this->linearVelocity += acceleration * deltaTime;
            this->position += this->linearVelocity * deltaTime;
            this->angle += this->angularVelocity * RAD_TO_DEG * deltaTime;
            
            this->force = Vector2(0, 0);

            this->UPDATE_VERTICES = true;
            this->UPDATE_AABB = true;
        }
};