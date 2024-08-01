#include "DirectXCollision.h"
#include "transform.h"
#include "boxcollision.h"
#include "orientedbox.h"
#include "gameobject.h"

void BoxCollision::Init()
{
	m_Transform = Parent->GetComponent<Transform>();
	m_Collision = BoundingBox(m_Transform->position,m_Transform->scale);
}

void BoxCollision::Update(const float& dt)
{
	m_Collision.Center = m_Transform->position ;
	m_Collision.Extents = m_Transform->scale;
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
        return this->m_Collision.Intersects(otherBox->GetCollision());
    }
    case CollisionType::OBB:{
        auto otherBox = static_cast<OrientedBox*>(other);
        if (!otherBox) return false;
        return this->m_Collision.Intersects(otherBox->GetCollision());
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
        return m_Collision.Contains(otherBox->GetCollision());
    }
    case CollisionType::OBB: {
        auto otherBox = static_cast<OrientedBox*>(other);
        if (!otherBox) return false;
        return this->m_Collision.Contains(otherBox->GetCollision());
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


