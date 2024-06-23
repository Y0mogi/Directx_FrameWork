#include "DirectXCollision.h"
#include "transform.h"
#include "boxcollision.h"
#include "orientedbox.h"
#include "gameobject.h"

void BoxCollision::Init()
{
	_transform = Parent->GetComponent<Transform>();
	_collision = BoundingBox(_transform->position,_transform->scale);
}

void BoxCollision::Update()
{
	_collision.Center = _transform->position ;
	_collision.Extents = _transform->scale;
}

void BoxCollision::CompInfo()
{

}

bool BoxCollision::Intersects(Collision_Base* other) const
{
    switch (other->GetCollisionType()) {
    case CollisionType::AABB: {
        auto otherBox = static_cast<BoxCollision*>(other);
        if (!otherBox) return false;
        return this->_collision.Intersects(otherBox->GetCollision());
    }
    case CollisionType::OBB:{
        auto otherBox = static_cast<OrientedBox*>(other);
        if (!otherBox) return false;
        return this->_collision.Intersects(otherBox->GetCollision());
    }
    
    case CollisionType::Sphere: {

        return false;
    }

    case CollisionType::Ray: {

        return false;
    }
    default:
        return false;
    }
    return false;
}

bool BoxCollision::Contains(Collision_Base* other) const
{
    switch (other->GetCollisionType()) {
    case CollisionType::AABB: {
        auto otherBox = static_cast<BoxCollision*>(other);
        if (!otherBox) return false;
        return _collision.Contains(otherBox->GetCollision());
    }
    case CollisionType::OBB: {
        auto otherBox = static_cast<OrientedBox*>(other);
        if (!otherBox) return false;
        return this->_collision.Contains(otherBox->GetCollision());
    }
    case CollisionType::Sphere: {

        return false;
    }

    case CollisionType::Ray: {

        return false;
    }
    default:
        return false;
    }
    return false;

}


