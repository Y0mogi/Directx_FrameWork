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
#include "input.h"


class Scene
{
public:
	virtual void Init()
	{
		using enum Layer;
		
		// ゲームオブジェクトを作成し、コンポーネントを追加
		AddObjComp<Sprite>("Sprite", Layer_2 , Tag::None);
		AddObjComp<ModelRenderer, OrientedBox,Field>("Field",Layer_1, Tag::Ground);
		auto a = AddObjComp<ModelRenderer,OrientedBox,Player>("Player",Layer_1,Tag::Player);
		a->GetComponent<Transform>()->scale = { 1,2,1 };

		for (int i = 0; i < 10; i++) {
			auto name = std::string("Enemy") + std::to_string(i);
			auto b = AddObjComp<ModelRenderer, OrientedBox, Jump, Enemy>(name, Layer_1, Tag::Enemy);
			b->GetComponent<Transform>()->position = { (5.f * i),1,1 };
		}
		
		AddObjComp<Camera>("Camera",Layer_0, Tag::None);

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
				it->GetComponent<Field>()->Init(XMFLOAT3{ 0.f,0.f, 0.0f}, XMFLOAT3{ 100.f, 0.0f,100.f }, XMFLOAT4{ 1.0f, 1.0f, 1.0f, 1.0f }, L"asset\\texture\\field.png");

			// Camera コンポーネントがある場合、その初期化を行う
			if (it->GetComponent<Camera>())
				it->GetComponent<Camera>()->Init();

			it->Init();
			
		}
		

	};
	virtual void Uninit() {

		for (auto& it : _objects) it->Uninit();

	};
	virtual void Update() {
		static int frame = 0;
		for (auto& it : _objects) it->Update();

		frame++;
		if (frame % 1 == 0) {
			this->CollisionUpdate();
			frame = 0;
		}

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
				return a->GetLayer() < b->GetLayer();
			}
		);

		for (auto& it : _objects) it->Draw();
		
	};


	/// <summary>
	/// 複数のコンポーネントを追加したゲームオブジェクトを追加する
	/// </summary>
	template<typename... Components> // 可変長テンプレート
	GameObject* AddObjComp(std::string name, Layer layer,Tag tag = Tag::None)
	{
		GameObject* obj = new GameObject(name,layer,tag ,this);
		(obj->AddComponent<Components>(), ...); // パラメータパックの展開
		AddGameObject(obj);
		return obj;
	}

	template<typename... Components> // 可変長テンプレート
	GameObject* AddObjComp()
	{
		GameObject* obj = new GameObject(this);
		(obj->AddComponent<Components>(), ...); // パラメータパックの展開
		AddGameObject(obj);
		return obj;
	}

	template<typename T>
	T* GetGameObject() {
		for (auto& it : _objects) {
			if (typeid(*it) == typeid(T)) { // 型比較
				T* tmp = dynamic_cast<T*>(it.get());
				if (tmp != nullptr) return tmp;
			}
		}
		return nullptr;
	}

	template<typename T>
	T* GetGameObject(std::string name) {
		for (auto& it : _objects) {
			if (typeid(*it) == typeid(T)){
				if ((*it).objectName == name) {// タグ比較
					auto* tmp = dynamic_cast<T*>(it.get());
					if (tmp != nullptr) return tmp;
				}
			}
		}
		return nullptr;
	}

	GameObject* GetGameObject(std::string name) {
		for (auto& it : _objects) {
			if (it->GetObjectName() == name) {
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
	/// ゲームオブジェクトの追加
	/// </summary> ========================
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

	// ====================================

	void ImguiUpdate() {
		// 各ゲームオブジェクトの情報をImGuiで表示
		this->ImGuiWindowCreate_Popup();

		//this->ImGuiWindowCreate_Tree();
	}

	// ImGuiUpdateの際の表示方法
	void ImGuiWindowCreate_Popup() {
		for (auto& obj : this->_objects) {
			if (ImGui::Button(obj->GetObjectName().c_str()))
				ImGui::OpenPopup(obj->GetObjectName().c_str());
			if (ImGui::BeginPopupModal(obj->GetObjectName().c_str(), NULL)) {
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

			if (ImGui::TreeNode(obj->GetObjectName().c_str())) {
				for (auto& comp : obj->componentList) {
					std::string tmp = typeid(*comp).name();
					tmp.erase(0, 5); // classのみ削除
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

		if (!collisionA || !collisionB) return false;

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
	/// 衝突処理
	/// </summary>
	void CollisionUpdate() { 
		for (auto& a : _objects) {
			if (!a->GetComponent<Collision_Base>()) continue; // コリジョンがあるかチェック
			if (!a->GetComponent<Collision_Base>()->IsActive()) continue; // 有効かチェック
			for (auto& b : _objects) {
				if (!b->GetComponent<Collision_Base>()) continue; // コリジョンがあるかチェック
				if (!b->GetComponent<Collision_Base>()->IsActive()) continue; // 有効かチェック
				if (b.get() == a.get()) continue; // 自身と判定しない
				a->GetComponent<Collision_Base>()->SetIsHit(IsOverLap(b.get(), a.get())); // 接触結果をセットする
				if(a->GetComponent<Collision_Base>()->IsHit())b->OnCollisionEnter(a.get());  // ヒットしていた場合は各コンポーネントの接触処理を呼び出す
			}
		}
	}
};