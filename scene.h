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
		// �Q�[���I�u�W�F�N�g���쐬���A�R���|�[�l���g��ǉ�
		AddObjComp<Field>();
		AddObjComp<ModelRenderer, Jump, Player>();
		AddObjComp<Sprite>();
		AddObjComp<Camera>();

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
				it->GetComponent<Field>()->Init(XMFLOAT3{ 0.f,0.f, 0.0f }, XMFLOAT3{ 5.f, 0.0f,5.f }, XMFLOAT4{ 1.0f, 1.0f, 1.0f, 1.0f }, L"asset\\texture\\field.png");

			// Camera �R���|�[�l���g������ꍇ�A���̏��������s��
			if (it->GetComponent<Camera>())
				it->GetComponent<Camera>()->Init();

			it->Init();
			
		}
		//obj = nullptr;
	};

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

	/// <summary>
	/// �����̃R���|�[�l���g��ǉ������Q�[���I�u�W�F�N�g��ǉ�����
	/// </summary>
	template<typename... Components> // �ϒ��e���v���[�g
	GameObject* AddObjComp()
	{
		GameObject* obj = new GameObject();
		(obj->AddComponent<Components>(), ...); // �p�����[�^�p�b�N�̓W�J
		AddGameObject(obj);
		return obj;
	}

	template<typename T>
	T* GetGameObject() {
		for (auto& it : _objects) {
			if (typeid(*it) == typeid(T)) { // �^��r
				T* tmp = dynamic_cast<T*>(it.get());
				if (tmp != nullptr) {
					return tmp;
				}
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

	};
	virtual void Draw() {

		for (auto& it : _objects)
		{
			it->Draw();
		}
	};
	
protected:
	std::list<std::unique_ptr<GameObject>> _objects{};
};