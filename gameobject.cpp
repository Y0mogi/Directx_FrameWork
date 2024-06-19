#include <list>
#include <string>
#include "gameobject.h"
#include "transform.h"
#include "scene.h"
#include "component.h"


GameObject::GameObject()
{
    AddComponent<Transform>();
    this->objectTag = "GameObject";
    this->scene = nullptr;
}

GameObject::GameObject(Scene* scene)
    :scene(scene)
{
    AddComponent<Transform>();
    objectTag = "GameObject";
    this->scene = scene;
}

GameObject::GameObject(const std::string& tag, Layer layer, Scene* scene)
    :objectTag(tag), layer(layer), scene(scene)
{
    AddComponent<Transform>();
}

GameObject::~GameObject()
{
    for (auto it : componentList) delete it;
}

void GameObject::Init()
{
    for (auto& it : componentList) it->Init();
}

void GameObject::Update()
{
    for (auto& it : componentList) it->Update();
}

void GameObject::CompInfo()
{
    for (auto& it : componentList) it->CompInfo();
}

void GameObject::Uninit()
{
    for (auto& it : componentList) it->Uninit();
}

void GameObject::Draw()
{
    for (auto& it : componentList) it->Draw();
}
