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

#include "jump.h"
#include "player.h"
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
		AddObjComp<Field>("Field",Layer_1);
		AddObjComp<ModelRenderer, Jump, Player>("Player",Layer_1);
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
				it->GetComponent<Field>()->Init(XMFLOAT3{ 0.f,0.f, 0.0f}, XMFLOAT3{ 5.f, 0.0f,5.f }, XMFLOAT4{ 1.0f, 1.0f, 1.0f, 1.0f }, L"asset\\texture\\field.png");

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

		this->ImguiUpdate();

	};
	virtual void Draw() {

		_objects.sort(
			[this](const std::unique_ptr<GameObject>& a, const std::unique_ptr<GameObject>& b)
			{

				auto camera = this->GetGameObject<Camera>()->Parent->GetComponent<Transform>()->position;
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
	
protected:
	std::list<std::unique_ptr<GameObject>> _objects{};
private:

	/// <summary>
	/// �Q�[���I�u�W�F�N�g�̒ǉ�
	/// </summary>
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


	void ImguiUpdate() {
		// �e�Q�[���I�u�W�F�N�g�̏���ImGui�ŕ\��
		this->ImGuiWindowCreate_Popup();
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
};