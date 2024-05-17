#include "main.h"
#include "gameobject.h"

GameObject::GameObject()
{

}

GameObject::GameObject(const Transform& transform)
	:_transform(transform)
{

}

GameObject::~GameObject()
{

}
