#pragma once

struct ContactPair { int first, second; };

class World
{
    public:
        World()
        {
            this->gravity = Vector2(0, 9.81);
        }
    
        int BodyCount()
        {
            return this->bodyes.size();
        }
    
        void AddBody(Body *body)
        {
            this->bodyes.push_back(body);
        }
    
        void RemoveBody(Body *body)
        {
            this->bodyes.remove(body);
        }
    
        Body* GetBody(int index)
        {
            int i = 0;
            for (Body* body : this->bodyes)
            {
                if(index == i++) 
                    return body;
            }
    
            return NULL;
        }

        void Step(float deltaTime, int totalIterations)
        {
            for(int currentIteration = 0; currentIteration < totalIterations; currentIteration++)
            {
                this->contactPairs.clear();
                this->StepBodies(deltaTime, totalIterations);

                this->BroadPhase();
                this->NarrowPhase();
            }        
        }

    private:
        bool Collide(Body* bodyA, Body* bodyB, Vector2& normal, float& depth)
        {
            normal = Vector2(0, 0);
            depth = 0.0f;

            if(bodyA->shapeType == Body::PolygonShape)
            {
                if(bodyB->shapeType == Body::PolygonShape)
                {
                    return IntersectPolygons(bodyA->GetTransformedVertices(), bodyA->position, bodyB->GetTransformedVertices(), bodyB->position, normal, depth);
                }
                else if(bodyB->shapeType == Body::CircleShape)
                {
                    bool result = IntersectCirclesPolygons(bodyB->position, bodyB->radius, bodyA->GetTransformedVertices(), bodyA->position, normal, depth);
                    
                    normal = -normal;
                    return result;
                }  
            }
            else if(bodyA->shapeType == Body::CircleShape)
            {
                if(bodyB->shapeType == Body::PolygonShape)
                {
                    return IntersectCirclesPolygons(bodyA->position, bodyA->radius, bodyB->GetTransformedVertices(), bodyB->position, normal, depth);
                }
                else if(bodyB->shapeType == Body::CircleShape)
                {
                    return IntersectCircles(bodyA->position, bodyA->radius, bodyB->position, bodyB->radius, normal, depth);
                }  
            }

            return false;
        }

        void SeparateBodies(Body *bodyA, Body *bodyB, Vector2 MinTranslationVector)
        {
            if(bodyA->isStatic)
            {
                bodyB->Move(MinTranslationVector);
            }
            else if(bodyB->isStatic)
            {
                bodyA->Move(-MinTranslationVector);
            }
            else 
            {
                bodyA->Move(-MinTranslationVector / 2.0f);
                bodyB->Move(MinTranslationVector / 2.0f);
            }
        }

        void FindContactPoints(Body *bodyA, Body* bodyB, Vector2& contactOne, Vector2& contactTwo, int& contactCount)
        {
            contactOne = Vector2(0, 0);
            contactTwo = Vector2(0, 0);
            contactCount = 0;

            if(bodyA->shapeType == Body::PolygonShape)
            {
                if(bodyB->shapeType == Body::PolygonShape)
                {
                    FindPolygonsContactPoints(bodyA->GetTransformedVertices(), bodyB->GetTransformedVertices(), contactOne, contactTwo, contactCount);
                }
                else if(bodyB->shapeType == Body::CircleShape)
                {
                    FindCirclePolygonContactPoint(bodyB->position, bodyB->radius, bodyA->GetTransformedVertices(), bodyA->position, contactOne);
                    contactCount = 1;
                }  
            }
            else if(bodyA->shapeType == Body::CircleShape)
            {
                if(bodyB->shapeType == Body::PolygonShape)
                {
                    FindCirclePolygonContactPoint(bodyA->position, bodyA->radius, bodyB->GetTransformedVertices(), bodyB->position, contactOne);
                    contactCount = 1;
                }   
                else if(bodyB->shapeType == Body::CircleShape)
                {
                    FindCircleContactPoint(bodyA->position, bodyA->radius, bodyB->position, contactOne);
                    contactCount = 1;
                }  
            }
        }
        
        void ResolveCollisionBasic(Manifold contact)
        {
            Body *bodyA = contact.bodyA;
            Body *bodyB = contact.bodyB;
            Vector2 normal = contact.normal;
            float depth = contact.depth;

            Vector2 relativeVelocity = bodyB->linearVelocity - bodyA->linearVelocity;

            if(Vector2::DotProduct(relativeVelocity, normal) > 0.0f) return;

            float e = minF(bodyA->restitution, bodyB->restitution);

            float j = -(1 + e) * Vector2::DotProduct(relativeVelocity, normal);
            j = j / (bodyA->invMass + bodyB->invMass); 

            Vector2 impulse = normal * j;

            bodyA->linearVelocity += -impulse * bodyA->invMass;
            bodyB->linearVelocity += impulse * bodyB->invMass;
        }

        void ResolveCollisionRotation(Manifold contact) {
            Body* bodyA = contact.bodyA;
            Body* bodyB = contact.bodyB;
            Vector2 normal = contact.normal;
            Vector2 contactOne = contact.contactOne;
            Vector2 contactTwo = contact.contactTwo;
            int contactCount = contact.contactCount;

            Vector2 contactList[2] = { contactOne, contactTwo };
            Vector2 impulseList[2] = { Vector2(0, 0), Vector2(0, 0) };
            Vector2 raList[2] = { Vector2(0, 0), Vector2(0, 0) };
            Vector2 rbList[2] = { Vector2(0, 0), Vector2(0, 0) };

            float e = minF(bodyA->restitution, bodyB->restitution);

            for (int i = 0; i < contactCount; i++)
            {
                Vector2 ra = contactList[i] - bodyA->position;
                Vector2 rb = contactList[i] - bodyB->position;

                raList[i] = ra;
                rbList[i] = rb;

                Vector2 raPerp = Vector2(-ra.y, ra.x);
                Vector2 rbPerp = Vector2(-rb.y, rb.x);

                Vector2 angularLinearVelocityA = raPerp * bodyA->angularVelocity;
                Vector2 angularLinearVelocityB = rbPerp * bodyB->angularVelocity;

                Vector2 relativeVelocity =
                    (bodyB->linearVelocity + angularLinearVelocityB) -
                    (bodyA->linearVelocity + angularLinearVelocityA);

                float contactVelocityMag = Vector2::DotProduct(relativeVelocity, normal);

                if (contactVelocityMag > 0.0f)
                    continue;

                float raPerpDotN = Vector2::DotProduct(raPerp, normal);
                float rbPerpDotN = Vector2::DotProduct(rbPerp, normal);

                float denom = bodyA->invMass + bodyB->invMass +
                    (raPerpDotN * raPerpDotN) * bodyA->invInertia +
                    (rbPerpDotN * rbPerpDotN) * bodyB->invInertia;

                float j = -(1.0f + e) * contactVelocityMag;
                j /= denom;
                j /= (float)contactCount;

                Vector2 impulse = normal * j;
                impulseList[i] = impulse;
            }

            for (int i = 0; i < contactCount; i++)
            {
                Vector2 impulse = impulseList[i];
                Vector2 ra = raList[i];
                Vector2 rb = rbList[i];

                bodyA->linearVelocity += -impulse * bodyA->invMass;
                bodyA->angularVelocity += -Vector2::CrossProduct(ra, impulse) * bodyA->invInertia;
                bodyB->linearVelocity += impulse * bodyB->invMass;
                bodyB->angularVelocity += Vector2::CrossProduct(rb, impulse) * bodyB->invInertia;
            }
        }

        void ResolveCollisionRotationFriction(Manifold contact) {
            Body* bodyA = contact.bodyA;
            Body* bodyB = contact.bodyB;
            Vector2 normal = contact.normal;
            Vector2 contactOne = contact.contactOne;
            Vector2 contactTwo = contact.contactTwo;
            float depth = contact.depth;
            int contactCount = contact.contactCount;

            float e = minF(bodyA->restitution, bodyB->restitution);

            float sf = (bodyA->staticFriction + bodyB->staticFriction) * 0.5f;
            float df = (bodyA->dinamicFriction + bodyB->dinamicFriction) * 0.5f;

            float jList[2] = {0, 0};
            Vector2 contactList[2] = {contactOne, contactTwo};
            Vector2 impulseList[2] = {Vector2(0, 0), Vector2(0, 0)};
            Vector2 frictionImpulseList[2] = { Vector2(0, 0), Vector2(0, 0) };
            Vector2 raList[2] = { Vector2(0, 0), Vector2(0, 0) };
            Vector2 rbList[2] = { Vector2(0, 0), Vector2(0, 0) };

            for (int i = 0; i < contactCount; i++) {
                Vector2 ra = contactList[i] - bodyA->position;
                Vector2 rb = contactList[i] - bodyB->position;

                raList[i] = ra; rbList[i] = rb;

                Vector2 raPerp = Vector2(-ra.y, ra.x);
                Vector2 rbPerp = Vector2(-rb.y, rb.x);

                Vector2 angularLinearVelocityA = raPerp * bodyA->angularVelocity;
                Vector2 angularLinearVelocityB = rbPerp * bodyB->angularVelocity;

                Vector2 relativeVelocity = 
                    (bodyB->linearVelocity + angularLinearVelocityB) 
                    - (bodyA->linearVelocity + angularLinearVelocityA);

                float contactVelocityMag = Vector2::DotProduct(relativeVelocity, normal);

                if (contactVelocityMag > 0) continue;

                float raPerpDot = Vector2::DotProduct(raPerp, normal);
                float rbPerpDot = Vector2::DotProduct(rbPerp, normal);

                float denominator = (bodyA->invMass + bodyB->invMass) +
                    (raPerpDot * raPerpDot) * bodyA->invInertia + 
                    (rbPerpDot * rbPerpDot) * bodyB->invInertia;

                float j = -(1.0f + e) * contactVelocityMag;
                j /= denominator;
                j /= float(contactCount);

                Vector2 impulse = normal * j;

                impulseList[i] = impulse;
                jList[i] = j;

            }

            for (int i = 0; i < contactCount; i++) {
                Vector2 impulse = impulseList[i];
                Vector2 ra = raList[i];
                Vector2 rb = rbList[i];

                bodyA->linearVelocity += -impulse * bodyA->invMass;
                bodyA->angularVelocity += -Vector2::CrossProduct(ra, impulse) * bodyA->invInertia;
                bodyB->linearVelocity += impulse * bodyB->invMass;
                bodyB->angularVelocity += Vector2::CrossProduct(rb, impulse) * bodyB->invInertia;
            }

            for (int i = 0; i < contactCount; i++) {
                Vector2 ra = contactList[i] - bodyA->position;
                Vector2 rb = contactList[i] - bodyB->position;

                raList[i] = ra; rbList[i] = rb;

                Vector2 raPerp = Vector2(-ra.y, ra.x);
                Vector2 rbPerp = Vector2(-rb.y, rb.x);

                Vector2 angularLinearVelocityA = raPerp * bodyA->angularVelocity;
                Vector2 angularLinearVelocityB = rbPerp * bodyB->angularVelocity;

                Vector2 relativeVelocity =
                    (bodyB->linearVelocity + angularLinearVelocityB)
                    - (bodyA->linearVelocity + angularLinearVelocityA);
                
                float relativeVelocityMag = Vector2::DotProduct(relativeVelocity, normal);
                Vector2 tangent = Vector2((relativeVelocity.x - relativeVelocityMag) * normal.x, (relativeVelocity.y - relativeVelocityMag) * normal.y);

                if(VectorMath::NearlyEqualVectors(tangent, Vector2(0, 0))) continue;
                else {
                    tangent = Vector2::Normalize(tangent);
                }

                float raPerpDot = Vector2::DotProduct(raPerp, tangent);
                float rbPerpDot = Vector2::DotProduct(rbPerp, tangent);

                float denominator = (bodyA->invMass + bodyB->invMass) +
                    (raPerpDot * raPerpDot) * bodyA->invInertia +
                    (rbPerpDot * rbPerpDot) * bodyB->invInertia;

                float jt = -Vector2::DotProduct(relativeVelocity, tangent);
                jt /= denominator;
                jt /= float(contactCount);

                Vector2 frictionImpulse;
                float j = jList[i];

                if (std::abs(jt) <= j * sf) {
                    frictionImpulse = tangent * jt;
                }
                else {
                    frictionImpulse = tangent * -j * df;
                }

                frictionImpulseList[i] = frictionImpulse;
            }

            for (int i = 0; i < contactCount; i++) {
                Vector2 frictionImpulse = frictionImpulseList[i];
                Vector2 ra = raList[i];
                Vector2 rb = rbList[i];

                bodyA->linearVelocity += -frictionImpulse * bodyA->invMass;
                bodyA->angularVelocity += -Vector2::CrossProduct(ra, frictionImpulse) * bodyA->invInertia;
                bodyB->linearVelocity += frictionImpulse * bodyB->invMass;
                bodyB->angularVelocity += Vector2::CrossProduct(rb, frictionImpulse) * bodyB->invInertia;
            }
        }
        
        void StepBodies(float deltaTime, int totalIterations)
        {
            for(Body* body : bodyes)
            {
                if(!body->isStatic)
                    body->AddForce(gravity * body->mass);

                body->Step(deltaTime, totalIterations);
            }
        }

        void BroadPhase()
        {
            int i = 0;
            for(auto body_it = bodyes.begin(); body_it != bodyes.end(); ++body_it, i++)
            {
                Body* bodyA = *(body_it);    
                AABB bodyA_AABB = bodyA->GetAABB();
                
                int j = i + 1;
                for(auto body_jt = std::next(body_it); body_jt != bodyes.end(); ++body_jt, j++)
                {
                    Body* bodyB = *(body_jt);
                    AABB bodyB_AABB = bodyB->GetAABB();

                    if(bodyA->isStatic && bodyB->isStatic) continue;
                    if(!IntersectAABB(bodyA_AABB, bodyB_AABB)) continue;
                    
                    contactPairs.emplace_back(ContactPair{i, j});
                }
            }
        }

        void NarrowPhase()
        {
            for(ContactPair pair : contactPairs)
            {
                Body *bodyA = this->GetBody(pair.first);
                Body *bodyB = this->GetBody(pair.second);

                Vector2 normal; float depth;
                if(!Collide(bodyA, bodyB, normal, depth)) continue;
                if(VectorMath::NAN_Values(normal)) continue;

                SeparateBodies(bodyA, bodyB, normal * depth);

                Vector2 contactOne, contactTwo; int contactCount;
                FindContactPoints(bodyA, bodyB, contactOne, contactTwo, contactCount);
                Manifold contact(bodyA, bodyB, normal, depth, contactOne, contactTwo, contactCount);

                ResolveCollisionRotationFriction(contact);
            }
        }

        Vector2 gravity;
        std::list<Body *> bodyes;
        std::vector<ContactPair> contactPairs;
};