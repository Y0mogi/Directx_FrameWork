#include "gameobject.h"
#include "transform.h"
#include "component.h"


GameObject::GameObject()
{
    AddComponent<Transform>();
}

GameObject::~GameObject()
{
    for (auto it : _componentList)
    {
        delete it;
    }
}

void GameObject::Init()
{
    for (auto& it : _componentList) it->Init();
}

void GameObject::Update()
{
    for (auto& it : _componentList) it->Update();
}

void GameObject::Uninit()
{
    for (auto& it : _componentList) it->Uninit();
}

void GameObject::Draw()
{
    for (auto& it : _componentList) it->Draw();
}
