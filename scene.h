#pragma once
#include "main.h"
#include "component.h"
#include "gameobject.h"

#include "modelRenderer.h"
#include "jump.h"
#include "player.h"
#include "polygon2D.h"
#include "field.h"
#include "camera.h"
#include "sprite.h"
#include "transform.h"
#include <list>
#include <memory>
#include <utility>



class Scene
{
public:
	virtual void Init()
	{
		// ゲームオブジェクトを作成し、コンポーネントを追加
		AddObjComp<Field>("Field");
		AddObjComp<ModelRenderer, Jump, Player>("Player");
		AddObjComp<Sprite>("Sprite");
		AddObjComp<Camera>("Camera");

		// 全オブジェクトを初期化
		for (auto& it : _objects)
		{
			// Polygon2D コンポーネントがある場合、その初期化を行う
			if (it->GetComponent<Sprite>())
			{
				it->GetComponent<Sprite>()->LoadTexture(L"asset\\texture\\camera.jpg");
				it->GetComponent<Transform>()->position = { 0,0,0 };
				it->GetComponent<Transform>()->scale = { 100,100,0 };
			}

			// Field コンポーネントがある場合、その初期化を行う
			if (it->GetComponent<Field>())
				it->GetComponent<Field>()->Init(XMFLOAT3{ 0.f,0.f, 0.0f}, XMFLOAT3{ 5.f, 0.0f,5.f }, XMFLOAT4{ 1.0f, 1.0f, 1.0f, 1.0f }, L"asset\\texture\\field.png");

			// Camera コンポーネントがある場合、その初期化を行う
			if (it->GetComponent<Camera>())
				it->GetComponent<Camera>()->Init();

			it->Init();
			
		}
		//obj = nullptr;
	};

	/// <summary>
	/// ゲームオブジェクトの追加
	/// </summary>
	template<typename T, typename... Args>
	T* AddGameObject(Args&&... args)
	{
		// 渡された引数でゲームオブジェクトを作成
		T* gameObject = new T(std::forward<Args>(args)...);
		gameObject->Init();
		_objects.emplace_back(gameObject);

		return gameObject;
	}

	template<typename T>
	T* AddGameObject(T* object)
	{
		// 渡された引数でゲームオブジェクトを作成
		//object->Init();
		_objects.emplace_back(object);

		return object;
	}

	/// <summary>
	/// 複数のコンポーネントを追加したゲームオブジェクトを追加する
	/// </summary>
	template<typename... Components> // 可変長テンプレート
	GameObject* AddObjComp(std::string tag)
	{
		GameObject* obj = new GameObject(tag);
		(obj->AddComponent<Components>(), ...); // パラメータパックの展開
		AddGameObject(obj);
		return obj;
	}

	template<typename T>
	T* GetGameObject() {
		for (auto& it : _objects) {
			if (typeid(*it) == typeid(T)) { // 型比較
				T* tmp = dynamic_cast<T*>(it.get());
				if (tmp != nullptr) {
					return tmp;
				}
			}
		}
		return nullptr;
	}

	template<typename T>
	T* GetGameObject(std::string tag) {
		for (auto& it : _objects) {
			if (typeid(*it) == typeid(T)) { // 型比較
				if ((*it).objectTag == tag) // タグ比較
				{
					T* tmp = dynamic_cast<T*>(it.get());
					if (tmp != nullptr) {
						return tmp;
					}					
				}
				return nullptr;
			}
		}
		return nullptr;
	}

	virtual void Uninit() {

		for (auto& it : _objects)
		{
			it->Uninit();
		}
	};

	virtual void Update() {

		for (auto& it : _objects)
		{
			it->Update();
		}

		this->ImguiUpdate();

	};
	virtual void Draw() {

		for (auto& it : _objects)
		{
			it->Draw();
		}
	};
	
protected:
	std::list<std::unique_ptr<GameObject>> _objects{};

private:
	void ImguiUpdate() {

		// Tree
		// 各ゲームオブジェクトの情報をImGuiで表示
		//for (auto& obj : this->_objects) {
		//	
		//	if (ImGui::TreeNode(obj.get()->objectTag.c_str())) {
		//		for (auto& comp : obj->_componentList) {
		//			std::string tmp = typeid(*comp).name();
		//			tmp.erase(0, 5); // classのみ削除
		//			if (ImGui::TreeNode(tmp.c_str())) {
		//				comp->CompInfo();
		//				ImGui::TreePop();
		//			}
		//		}
		//		ImGui::TreePop();
		//	}
		//}

		{ // 子ウィンドウ
			for (auto& obj : this->_objects) {
				if (ImGui::Button(obj.get()->objectTag.c_str()))
					ImGui::OpenPopup(obj.get()->objectTag.c_str());
				if (ImGui::BeginPopupModal(obj.get()->objectTag.c_str(), NULL))
				{
					ImGui::SeparatorText("Components");
					for (auto& comp : obj->_componentList) {
						
						comp->CompInfo();
						
					}
					ImGui::Separator();
					if (ImGui::Button("Close"))
						ImGui::CloseCurrentPopup();
					ImGui::EndPopup();
				}
			}
		}

	}
};