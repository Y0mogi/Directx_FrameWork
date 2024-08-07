#include <list>
#include <string>
#include "transform.h"
#include "component.h"
#include "scene.h"
#include "gameobject.h"
#include "modelRenderer.h"


GameObject::GameObject()
{
    AddComponent<Transform>();
    AddComponent<ModelRenderer>();
    this->m_ObjectName = "GameObject";
    this->m_Scene = nullptr;
}

GameObject::GameObject(Scene* scene)
    :m_Scene(scene)
{
    AddComponent<Transform>();
    AddComponent<ModelRenderer>();
    this->m_ObjectName = "GameObject" + scene->GetObjectList().size();
}

GameObject::GameObject(const std::string& name, Layer layer, Tag tag, Scene* scene)
    :m_ObjectName(name), m_Layer(layer), m_Tag(tag), m_Scene(scene)
{
    AddComponent<Transform>();
    AddComponent<ModelRenderer>();
}

GameObject::~GameObject()
{
    for (auto it : m_ComponentList) delete it;
}

void GameObject::Init()
{
    for (auto& it : m_ComponentList) it->Init();
}

void GameObject::Update(const float& dt)
{
    for (auto& it : m_ComponentList) it->Update(dt);
}

void GameObject::Uninit()
{
    for (auto& it : m_ComponentList) it->Uninit();
}

void GameObject::Draw()
{
    for (auto& it : m_ComponentList) it->Draw();
}

void GameObject::OnCollisionEnter(GameObject* collision) {
    for (auto& it : m_ComponentList) {
        auto tmp = dynamic_cast<CollisionEvent*>(it);
        if (tmp) tmp->OnCollisionEnter(collision);
    }
}

void GameObject::CompInfo()
{
    for (auto& it : m_ComponentList) it->CompInfo();
}