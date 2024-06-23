#include <list>
#include <string>
#include "transform.h"
#include "component.h"
#include "gameobject.h"
#include "scene.h"



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
    objectTag = "GameObject" + scene->GetObjectList()->size();
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

void GameObject::OnCollisionEnter(GameObject* collision) {
    for (auto& it : componentList) {
        auto tmp = dynamic_cast<CollisionEvent*>(it);
        if (tmp) tmp->OnCollisionEnter(collision);
    }
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
