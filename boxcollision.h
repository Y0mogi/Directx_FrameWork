#pragma once

#include "component.h"
#include "collision_base.h"


class BoxCollision : public Component, public Collision_Base
{
public:
    void Init() override;
    void Update() override;
    void CompInfo() override;

    bool Intersects(Collision_Base* other) const override;
    bool Contains(Collision_Base* other)const override;
    
    BoundingBox& GetCollision() { return _collision; }

    BoxCollision() { SetCollisionType(CollisionType::AABB); }
    ~BoxCollision() override {}

    

private:
    BoundingBox _collision;
    Transform*  _transform{};
};