#pragma once
#include <list>
#include <memory>
#include <utility>
#include <algorithm>

#include "main.h"
#include "renderer.h"
#include "component.h"
#include "gameobject.h"
#include "transform.h"
#include "modelRenderer.h"
#include "collision_base.h"

#include "orientedbox.h"
#include "boxcollision.h"

#include "jump.h"
#include "player.h"
#include "enemy.h"
#include "field.h"
#include "camera.h"
#include "sprite.h"


class Scene
{
public:
	virtual void Init()
	{
		using enum Layer;

		// �Q�[���I�u�W�F�N�g���쐬���A�R���|�[�l���g��ǉ�
		AddObjComp<Sprite>("Sprite", Layer_2);
		AddObjComp<ModelRenderer, OrientedBox,Field>("Field",Layer_1);
		auto a = AddObjComp<ModelRenderer,OrientedBox,Player>("Player",Layer_1);
		a->GetComponent<Transform>()->scale = { 1,2,1 };
		AddObjComp<ModelRenderer,OrientedBox, Jump, Enemy>("Enemy", Layer_1);
		AddObjComp<Camera>("Camera",Layer_0);

		// �S�I�u�W�F�N�g��������
		for (auto& it : _objects)
		{
			// Polygon2D �R���|�[�l���g������ꍇ�A���̏��������s��
			if (it->GetComponent<Sprite>())
			{
				it->GetComponent<Sprite>()->LoadTexture(L"asset\\texture\\camera.jpg");
				it->GetComponent<Transform>()->position = { 0,0,0 };
				it->GetComponent<Transform>()->scale = { 100,100,0 };
			}

			// Field �R���|�[�l���g������ꍇ�A���̏��������s��
			if (it->GetComponent<Field>())
				it->GetComponent<Field>()->Init(XMFLOAT3{ 0.f,0.f, 0.0f}, XMFLOAT3{ 100.f, 0.0f,100.f }, XMFLOAT4{ 1.0f, 1.0f, 1.0f, 1.0f }, L"asset\\texture\\field.png");

			// Camera �R���|�[�l���g������ꍇ�A���̏��������s��
			if (it->GetComponent<Camera>())
				it->GetComponent<Camera>()->Init();

			it->Init();
			
		}
		

	};
	virtual void Uninit() {

		for (auto& it : _objects) it->Uninit();

	};
	virtual void Update() {

		for (auto& it : _objects) it->Update();
			
		this->CollisionUpdate();

		this->ImguiUpdate();

	};
	virtual void Draw() {

		_objects.sort(
			[this](const std::unique_ptr<GameObject>& a, const std::unique_ptr<GameObject>& b)
			{

				auto camera = this->GetGameObject("Camera")->GetComponent<Transform>()->position;
				return Distance(a->GetComponent<Transform>()->position, camera) > Distance(b->GetComponent<Transform>()->position, camera);
			}
		);

		_objects.sort(
		[](const std::unique_ptr<GameObject>& a , const std::unique_ptr<GameObject>& b)
			{
				return a->layer < b->layer;
			}
		);

		for (auto& it : _objects) it->Draw();
		
	};


	/// <summary>
	/// �����̃R���|�[�l���g��ǉ������Q�[���I�u�W�F�N�g��ǉ�����
	/// </summary>
	template<typename... Components> // �ϒ��e���v���[�g
	GameObject* AddObjComp(std::string tag, Layer layer)
	{
		GameObject* obj = new GameObject(tag, layer, this);
		(obj->AddComponent<Components>(), ...); // �p�����[�^�p�b�N�̓W�J
		AddGameObject(obj);
		return obj;
	}

	template<typename... Components> // �ϒ��e���v���[�g
	GameObject* AddObjComp()
	{
		GameObject* obj = new GameObject(this);
		(obj->AddComponent<Components>(), ...); // �p�����[�^�p�b�N�̓W�J
		AddGameObject(obj);
		return obj;
	}

	template<typename T>
	T* GetGameObject() {
		for (auto& it : _objects) {
			if (typeid(*it) == typeid(T)) { // �^��r
				T* tmp = dynamic_cast<T*>(it.get());
				if (tmp != nullptr) return tmp;
			}
		}
		return nullptr;
	}

	template<typename T>
	T* GetGameObject(std::string tag) {
		for (auto& it : _objects) {
			if (typeid(*it) == typeid(T)){
				if ((*it).objectTag == tag) {// �^�O��r
					auto* tmp = dynamic_cast<T*>(it.get());
					if (tmp != nullptr) return tmp;
				}
			}
		}
		return nullptr;
	}

	GameObject* GetGameObject(std::string tag) {
		for (auto& it : _objects) {
			if ((*it).objectTag == tag) {
				auto* tmp = it.get();
				if (tmp != nullptr) return tmp;
			}
		}
		return nullptr;
	}
	
	std::list<std::unique_ptr<GameObject>>* GetObjectList() { return &_objects; }

protected:
	std::list<std::unique_ptr<GameObject>> _objects{};
private:

	/// <summary>
	/// �Q�[���I�u�W�F�N�g�̒ǉ�
	/// </summary> ========================
	template<typename T, typename... Args>
	T* AddGameObject(Args&&... args)
	{
		// �n���ꂽ�����ŃQ�[���I�u�W�F�N�g���쐬
		T* gameObject = new T(std::forward<Args>(args)...);
		gameObject->Init();
		_objects.emplace_back(gameObject);

		return gameObject;
	}
	template<typename T>
	T* AddGameObject(T* object)
	{
		// �n���ꂽ�����ŃQ�[���I�u�W�F�N�g���쐬
		//object->Init();
		_objects.emplace_back(object);

		return object;
	}

	// ====================================

	void ImguiUpdate() {
		// �e�Q�[���I�u�W�F�N�g�̏���ImGui�ŕ\��
		this->ImGuiWindowCreate_Popup();

		//this->ImGuiWindowCreate_Tree();
	}

	// ImGuiUpdate�̍ۂ̕\�����@
	void ImGuiWindowCreate_Popup() {
		for (auto& obj : this->_objects) {
			if (ImGui::Button(obj.get()->objectTag.c_str()))
				ImGui::OpenPopup(obj.get()->objectTag.c_str());
			if (ImGui::BeginPopupModal(obj.get()->objectTag.c_str(), NULL)) {
				if (ImGui::Button("Close"))
					ImGui::CloseCurrentPopup();
				// ImGui::SeparatorText("Components");
				for (auto& comp : obj->componentList) {

					comp->CompInfo();
					ImGui::NewLine();
				}

				
				ImGui::NewLine();
				ImGui::EndPopup();
			}
		}
	}
	void ImGuiWindowCreate_Tree() {
		for (auto& obj : this->_objects) {

			if (ImGui::TreeNode(obj.get()->objectTag.c_str())) {
				for (auto& comp : obj->componentList) {
					std::string tmp = typeid(*comp).name();
					tmp.erase(0, 5); // class�̂ݍ폜
					if (ImGui::TreeNode(tmp.c_str())) {
						comp->CompInfo();
						ImGui::TreePop();
					}
				}
				ImGui::TreePop();
			}
		}
	}

	// ====================================

	bool IsOverLap(GameObject* a, GameObject* b) {
		
		if (a == nullptr || b == nullptr) return false;

		auto collisionA = a->GetComponent<Collision_Base>();
		auto collisionB = b->GetComponent<Collision_Base>();

		switch (collisionA->GetCollisionType()) {
			case CollisionType::AABB: {
				return static_cast<BoxCollision*>(collisionA)->Intersects(collisionB);
			}
			case CollisionType::OBB: {
				return static_cast<OrientedBox*>(collisionA)->Intersects(collisionB);
			}
			case CollisionType::Sphere: {
				return false;
			}

			case CollisionType::Ray: {
				return false;
			}
		}

		return false;
	}

	/// <summary>
	/// �Փˏ���
	/// </summary>
	void CollisionUpdate() { 
		for (auto& a : _objects) {
			if (!a->GetComponent<Collision_Base>()) continue; // �R���W���������邩�`�F�b�N
			if (!a->GetComponent<Collision_Base>()->IsActive()) continue; // �L�����`�F�b�N
			for (auto& b : _objects) {
				if (!b->GetComponent<Collision_Base>()) continue; // �R���W���������邩�`�F�b�N
				if (!b->GetComponent<Collision_Base>()->IsActive()) continue; // �L�����`�F�b�N
				if (a.get() == b.get()) continue; // ���g�Ɣ��肵�Ȃ�
				a->GetComponent<Collision_Base>()->SetIsHit(IsOverLap(a.get(), b.get())); // �ڐG���ʂ��Z�b�g����
				if(a->GetComponent<Collision_Base>()->IsHit())a->OnCollisionEnter(b.get());  // �q�b�g���Ă����ꍇ�͊e�R���|�[�l���g�̐ڐG�������Ăяo��
			}
		}
	}
};