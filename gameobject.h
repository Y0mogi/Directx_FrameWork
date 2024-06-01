#pragma once
#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <list>

class Component;

class GameObject
{
public:

	void Init();
	void Update();
	void Uninit();
	void Draw();

	/// <summary>
	/// �A�^�b�`����Ă���R���|�[�l���g�̎擾
	/// </summary>
	/// <typeparam name="T"></typeparam>
	/// <returns>�R���|�[�l���g�̃|�C���^</returns>
	template<class T>
	T* GetComponent(){
		for (auto& it : _componentList) {
			T* tmp = dynamic_cast<T*>(it);
			if (tmp != nullptr)
				return tmp;
		}
		return nullptr;
	}

	/// <summary>
	/// �I�u�W�F�N�g�ɃR���|�[�l���g���A�^�b�`
	/// </summary>
	/// <typeparam name="T">�ǉ��������R���|�[�l���g�N���X��</typeparam>
	/// <returns>�ǉ������R���|�[�l���g�̃|�C���^</returns>
	template<class T>
	T* AddComponent(){
		T* tmp = new T();
		tmp->Parent = this;
		_componentList.push_back(tmp);
		return tmp;
	}

	/// <summary>
	/// �I�u�W�F�N�g�ɕ����̃R���|�[�l���g���A�^�b�`
	/// </summary>
	/// <typeparam name="T"></typeparam>
	/// <typeparam name="...Args"></typeparam>
	/// <param name="...args"></param>
	/// <returns></returns>
	template<class T, typename... Args>
	T* AddComponent(Args&&... args) {
		T* tmp = new T(std::forward<Args>(args)...);
		tmp->Parent = this;
		_componentList.push_back(tmp);
		return tmp;
	}

	/// <summary>
	/// �����Ă���R���|�[�l���g���폜����
	/// </summary>
	/// <typeparam name="T"></typeparam>
	/// <returns>�폜������������</returns>
	template<class T>
	bool RemoveComponent() {
		for (auto it = _componentList.begin(); it != _componentList.end(); ++it) {
			if (typeid(**it) == typeid(T)) {  // typeid �Ō^���r
				Component* tmp = *it;
				it = _componentList.erase(it);  // ���X�g����v�f���폜���A�C�e���[�^���X�V
				delete tmp;  // �����������
				return true;  // ����
			}
		}
		return false;  // ������Ȃ������ꍇ
	}

	GameObject();
	~GameObject();

public:
	std::list<Component*> _componentList;
};

#endif // !GAMEOBJECT_H
