#include <list>
#include <string>
#include "gameobject.h"
#include "transform.h"
#include "component.h"


GameObject::GameObject()
{
    AddComponent<Transform>();
}

GameObject::GameObject(const std::string& tag)
    :objectTag(tag)
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

void GameObject::CompInfo()
{
    for (auto& it : _componentList) it->CompInfo();
}

void GameObject::Uninit()
{
    for (auto& it : _componentList) it->Uninit();
}

void GameObject::Draw()
{
    for (auto& it : _componentList) it->Draw();
}
