#include <list>
#include <string>
#include "transform.h"
#include "component.h"
#include "gameobject.h"
#include "scene.h"



GameObject::GameObject()
{
    AddComponent<Transform>();
    this->_objectName = "GameObject";
    this->scene = nullptr;
}

GameObject::GameObject(Scene* scene)
    :scene(scene)
{
    AddComponent<Transform>();
    this->_objectName = "GameObject" + scene->GetObjectList()->size();
    this->scene = scene;
}

GameObject::GameObject(const std::string& name, Layer layer, Tag tag, Scene* scene)
    :_objectName(name), _layer(layer), _tag(tag), scene(scene)
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
