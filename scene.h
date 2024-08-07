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
/// �ڐG����̍X�V�p�x
/// </summary>
enum class COLLITION_UPDATE_FRAME : int {
	Default = 1,	// ���t���[��
	Half	= 2,	// 2�t���[���Ɉ�x
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
		
		// �폜�\��I�u�W�F�N�g�̍폜
		m_Objects.remove_if([](const std::unique_ptr<GameObject>& object) {return object->IsDiscard(); });

		this->ImguiUpdate();
	};
	virtual void Draw() {

		// Z�\�[�g
		//m_Objects.sort(
		//	[this](const std::unique_ptr<GameObject>& a, const std::unique_ptr<GameObject>& b)
		//	{
		//		auto camera = this->GetGameObject("Camera")->GetComponent<Transform>()->position;
		//		return Distance(a->GetComponent<Transform>()->position, camera) > Distance(b->GetComponent<Transform>()->position, camera);
		//	}
		//);

		// �`�揇�Ƀ\�[�g
		m_Objects.sort(
			[](const std::unique_ptr<GameObject>& a, const std::unique_ptr<GameObject>& b)
			{
				return a->GetLayer() < b->GetLayer();
			}
		);

		for (auto& it : m_Objects) it->Draw();

	};

	//==============================================================================
	// �Q�[���I�u�W�F�N�g���̓R���|�[�l���g�̒ǉ�
	//==============================================================================

	/// <summary>
	/// �����̃R���|�[�l���g��ǉ������Q�[���I�u�W�F�N�g��ǉ�����
	/// </summary>
	/// <typeparam name="...Components"></typeparam>
	/// <param name="name"></param>
	/// <param name="layer"></param>
	/// <param name="tag"></param>
	/// <param name="...components"></param>
	/// <returns></returns>
	template<typename... Components> // �ϒ��e���v���[�g
	GameObject* AddObjComp(std::string name, Layer layer, Tag tag = Tag::None, Components&&... components)
	{
		GameObject* obj = new GameObject(name, layer, tag, this);
		(obj->AddComponent<Components>(std::forward<Components>(components)),...); // �p�����[�^�p�b�N�̓W�J
		AddGameObject(obj);
		obj->Init();  // �R���|�[�l���g��ǉ�������ɏ�����
		return obj;
	}

	template<typename Component> 
	GameObject* AddObjComp(std::string name, Layer layer, Tag tag, Component* component)
	{
		GameObject* obj = new GameObject(name, layer, tag, this);
		(obj->AddComponent<Component>(component));
		AddGameObject(obj);
		obj->Init();  // �R���|�[�l���g��ǉ�������ɏ�����
		return obj;
	}

	/// <summary>
	/// 
	/// </summary>
	/// <typeparam name="...Components"></typeparam>
	/// <returns></returns>
	template<typename... Components> // �ϒ��e���v���[�g
	GameObject* AddObjComp()
	{
		GameObject* obj = new GameObject(this);
		(obj->AddComponent<Components>(), ...); // �p�����[�^�p�b�N�̓W�J
		AddGameObject(obj);
		obj->Init();  // �R���|�[�l���g��ǉ�������ɏ�����
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
		// �n���ꂽ�����ŃQ�[���I�u�W�F�N�g���쐬
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
		// �n���ꂽ�����ŃQ�[���I�u�W�F�N�g���쐬
		m_Objects.emplace_back(object);

		return object;
	}

	//==============================================================================
	// �Q�[���I�u�W�F�N�g���̓R���|�[�l���g�̎擾
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
				if ((*it).objectName == name) {// �^�O��r
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
	/// �I�u�W�F�N�g�����݂��Ă��邩
	/// </summary>
	/// <param name="object">���ׂ����I�u�W�F�N�g�̃|�C���^</param>
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
	/// ImGui�̍X�V
	/// </summary>
	void ImguiUpdate() {
		// �e�Q�[���I�u�W�F�N�g�̏���ImGui�ŕ\��
		this->ImGuiWindowCreate_Popup();

		//this->ImGuiWindowCreate_Tree();
	}

	/// <summary>
	/// ImGui�̕\�����|�b�v�A�b�v�ŕ\������
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
	/// ImGui�̕\�����c���[�ŕ\������
	/// </summary>
	void ImGuiWindowCreate_Tree() {
		for (auto& obj : this->m_Objects) {

			if (ImGui::TreeNode(obj->GetObjectName().c_str())) {
				for (auto& comp : obj->m_ComponentList) {
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

	/// <summary>
	/// �ڐG�m�F
	/// </summary>
	/// <param name="a">�Ώ�a</param>
	/// <param name="b">�Ώ�b</param>
	/// <returns>TRUE:�ڐG FALSE:���ڐG</returns>
	bool IsOverLap(GameObject* a, GameObject* b) {

		// null�`�F�b�N
		if (a == nullptr || b == nullptr) return false;

		auto collisionA = a->GetComponent<Collision_Base>();
		auto collisionB = b->GetComponent<Collision_Base>();

		// �R���|�[�l���g�̊m�F
		if (!collisionA || !collisionB) return false;

		// collisionA�̓����蔻��^�C�v�ɃL���X�g����������
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
		for (auto& a : m_Objects) {
			// �R���W���������� & �L����
			if (!a->GetComponent<Collision_Base>()) continue;
			if (!a->GetComponent<Collision_Base>()->IsActive()) continue;
			for (auto& b : m_Objects) {
				// �R���W���������� & �L����
				if (!b->GetComponent<Collision_Base>()) continue;
				if (!b->GetComponent<Collision_Base>()->IsActive()) continue;

				// ���g�Ɣ��肵�Ȃ�
				if (b.get() == a.get()) continue;

				// �ڐG���ʂ��Z�b�g����
				a->GetComponent<Collision_Base>()->SetIsHit(IsOverLap(b.get(), a.get()));

				// �q�b�g���Ă����ꍇ�͊e�R���|�[�l���g�̐ڐG�������Ăяo��
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
