#pragma once
#include <list>
#include <memory>
#include <utility>
#include <algorithm>

#include "main.h"
#include "renderer.h"
#include "component.h"
#include "gameobject.h"
#include "collision_base.h"
#include "orientedbox.h"
#include "boxcollision.h"

/// <summary>
/// 接触判定の更新頻度
/// </summary>
enum class COLLITION_UPDATE_FRAME : int {
	Default = 1,	// 毎フレーム
	Half	= 2,	// 2フレームに一度
	COUNT
};

class Scene
{
public:
	virtual void Init()
	{

	};
	virtual void Uninit() {

		for (auto& it : m_Objects) it->Uninit();
		m_Objects.clear();
	};
	virtual void Update(const float& dt) {
		static int frame = 0;
		for (auto& it : m_Objects) { it->Update(dt); }

		frame++;
		if (frame % static_cast<int>(m_CUF) == 0) {
			this->CollisionUpdate();
			frame = 0;
		}
		
		// 削除予定オブジェクトの削除
		m_Objects.remove_if([](const std::unique_ptr<GameObject>& object) {return object->IsDiscard(); });

		this->ImguiUpdate();
	};
	virtual void Draw() {

		// Zソート
		//m_Objects.sort(
		//	[this](const std::unique_ptr<GameObject>& a, const std::unique_ptr<GameObject>& b)
		//	{
		//		auto camera = this->GetGameObject("Camera")->GetComponent<Transform>()->position;
		//		return Distance(a->GetComponent<Transform>()->position, camera) > Distance(b->GetComponent<Transform>()->position, camera);
		//	}
		//);

		// 描画順にソート
		m_Objects.sort(
			[](const std::unique_ptr<GameObject>& a, const std::unique_ptr<GameObject>& b)
			{
				return a->GetLayer() < b->GetLayer();
			}
		);

		for (auto& it : m_Objects) it->Draw();

	};

	//==============================================================================
	// ゲームオブジェクト又はコンポーネントの追加
	//==============================================================================

	/// <summary>
	/// 複数のコンポーネントを追加したゲームオブジェクトを追加する
	/// </summary>
	/// <typeparam name="...Components"></typeparam>
	/// <param name="name"></param>
	/// <param name="layer"></param>
	/// <param name="tag"></param>
	/// <param name="...components"></param>
	/// <returns></returns>
	template<typename... Components> // 可変長テンプレート
	GameObject* AddObjComp(std::string name, Layer layer, Tag tag = Tag::None, Components&&... components)
	{
		GameObject* obj = new GameObject(name, layer, tag, this);
		(obj->AddComponent<Components>(std::forward<Components>(components)),...); // パラメータパックの展開
		AddGameObject(obj);
		obj->Init();  // コンポーネントを追加した後に初期化
		return obj;
	}

	template<typename Component> 
	GameObject* AddObjComp(std::string name, Layer layer, Tag tag, Component* component)
	{
		GameObject* obj = new GameObject(name, layer, tag, this);
		(obj->AddComponent<Component>(component));
		AddGameObject(obj);
		obj->Init();  // コンポーネントを追加した後に初期化
		return obj;
	}

	/// <summary>
	/// 
	/// </summary>
	/// <typeparam name="...Components"></typeparam>
	/// <returns></returns>
	template<typename... Components> // 可変長テンプレート
	GameObject* AddObjComp()
	{
		GameObject* obj = new GameObject(this);
		(obj->AddComponent<Components>(), ...); // パラメータパックの展開
		AddGameObject(obj);
		obj->Init();  // コンポーネントを追加した後に初期化
		return obj;
	}

	/// <summary>
	/// 
	/// </summary>
	/// <typeparam name="T"></typeparam>
	/// <typeparam name="...Args"></typeparam>
	/// <param name="...args"></param>
	/// <returns></returns>
	template<typename T, typename... Args>
	T* AddGameObject(Args&&... args)
	{
		// 渡された引数でゲームオブジェクトを作成
		T* gameObject = new T(std::forward<Args>(args)...);
		gameObject->Init();
		m_Objects.emplace_back(gameObject);

		return gameObject;
	}

	/// <summary>
	/// 
	/// </summary>
	/// <typeparam name="T"></typeparam>
	/// <param name="object"></param>
	/// <returns></returns>
	template<typename T>
	T* AddGameObject(T* object)
	{
		// 渡された引数でゲームオブジェクトを作成
		m_Objects.emplace_back(object);

		return object;
	}

	//==============================================================================
	// ゲームオブジェクト又はコンポーネントの取得
	//==============================================================================

	/// <summary>
	/// 
	/// </summary>
	/// <typeparam name="T"></typeparam>
	/// <returns></returns>
	template<typename T>
	GameObject* GetGameObject() {
		for (auto& it : m_Objects) {
			if (it->GetComponent<T>()) {
				GameObject* tmp = it.get();
				if (tmp != nullptr) return tmp;
			}
		}
		return nullptr;
	}

	/// <summary>
	/// 
	/// </summary>
	/// <typeparam name="T"></typeparam>
	/// <param name="name"></param>
	/// <returns></returns>
	template<typename T>
	T* GetGameObject(std::string name) {
		for (auto& it : m_Objects) {
			if (typeid(*it) == typeid(T)) {
				if ((*it).objectName == name) {// タグ比較
					auto* tmp = dynamic_cast<T*>(it.get());
					if (tmp != nullptr) return tmp;
				}
			}
		}
		return nullptr;
	}

	/// <summary>
	/// 
	/// </summary>
	/// <param name="name"></param>
	/// <returns></returns>
	GameObject* GetGameObject(std::string name) {
		for (auto& it : m_Objects) {
			if (it->GetObjectName() == name) {
				auto* tmp = it.get();
				if (tmp != nullptr) return tmp;
			}
		}
		return nullptr;
	}

	/// <summary>
	/// 
	/// </summary>
	/// <typeparam name="T"></typeparam>
	/// <returns></returns>
	template<typename T>
	std::vector<GameObject*> GetGameObjects() {
		std::vector<T*> returnList{};
		for (auto& it : m_Objects) {
			if (it->GetComponent<T>()) {
				GameObject* tmp = it.get();
				if (tmp != nullptr) returnList.push_back(tmp);
			}
		}
		return returnList;
	}

	/// <summary>
	/// オブジェクトが存在しているか
	/// </summary>
	/// <param name="object">調べたいオブジェクトのポインタ</param>
	/// <returns></returns>
	bool GetGameObjectAlive(GameObject* object){
		auto tmp = std::find_if(m_Objects.begin(), m_Objects.end(), [object](const std::unique_ptr<GameObject>& a)
			{
				return a.get() == object;
			});
		
		if (tmp != m_Objects.end()) { return true; }

		return false;
	}
	//==============================================================================
	//
	//==============================================================================

	/// <summary>
	/// ImGuiの更新
	/// </summary>
	void ImguiUpdate() {
		// 各ゲームオブジェクトの情報をImGuiで表示
		this->ImGuiWindowCreate_Popup();

		//this->ImGuiWindowCreate_Tree();
	}

	/// <summary>
	/// ImGuiの表示をポップアップで表示する
	/// </summary>
	void ImGuiWindowCreate_Popup() {
		for (auto& obj : this->m_Objects) {
			if (ImGui::Button(obj->GetObjectName().c_str()))
				ImGui::OpenPopup(obj->GetObjectName().c_str());
			if (ImGui::BeginPopupModal(obj->GetObjectName().c_str(), NULL)) {
				if (ImGui::Button("Close"))
					ImGui::CloseCurrentPopup();
				// ImGui::SeparatorText("Components");
				for (auto& comp : obj->m_ComponentList) {

					comp->CompInfo();
					ImGui::NewLine();
				}


				ImGui::NewLine();
				ImGui::EndPopup();
			}
		}
	}

	/// <summary>
	/// ImGuiの表示をツリーで表示する
	/// </summary>
	void ImGuiWindowCreate_Tree() {
		for (auto& obj : this->m_Objects) {

			if (ImGui::TreeNode(obj->GetObjectName().c_str())) {
				for (auto& comp : obj->m_ComponentList) {
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

	/// <summary>
	/// 接触確認
	/// </summary>
	/// <param name="a">対象a</param>
	/// <param name="b">対象b</param>
	/// <returns>TRUE:接触 FALSE:未接触</returns>
	bool IsOverLap(GameObject* a, GameObject* b) {

		// nullチェック
		if (a == nullptr || b == nullptr) return false;

		auto collisionA = a->GetComponent<Collision_Base>();
		auto collisionB = b->GetComponent<Collision_Base>();

		// コンポーネントの確認
		if (!collisionA || !collisionB) return false;

		// collisionAの当たり判定タイプにキャスト＆交差判定
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
		for (auto& a : m_Objects) {
			// コリジョンがある & 有効か
			if (!a->GetComponent<Collision_Base>()) continue;
			if (!a->GetComponent<Collision_Base>()->IsActive()) continue;
			for (auto& b : m_Objects) {
				// コリジョンがある & 有効か
				if (!b->GetComponent<Collision_Base>()) continue;
				if (!b->GetComponent<Collision_Base>()->IsActive()) continue;

				// 自身と判定しない
				if (b.get() == a.get()) continue;

				// 接触結果をセットする
				a->GetComponent<Collision_Base>()->SetIsHit(IsOverLap(b.get(), a.get()));

				// ヒットしていた場合は各コンポーネントの接触処理を呼び出す
				if (a->GetComponent<Collision_Base>()->IsHit())b->OnCollisionEnter(a.get());
			}
		}
	}

	std::list<std::unique_ptr<GameObject>>& GetObjectList() { return m_Objects; }

	inline void SetCUF(const COLLITION_UPDATE_FRAME& cuf) { m_CUF = cuf; }

	virtual ~Scene() {};
protected:
	std::list<std::unique_ptr<GameObject>> m_Objects{};
private:
	COLLITION_UPDATE_FRAME m_CUF = COLLITION_UPDATE_FRAME::Half;
};
