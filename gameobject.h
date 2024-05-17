#pragma once
#include <list>

class Component;

class GameObject
{
public:

	void Init();
	void Update();
	void Uninit();
	void Draw();

	// �I�u�W�F�N�g�������Ă���R���|�[�l���g���擾
	template<class T>
	T* GetComponent();

	// �I�u�W�F�N�g�������Ă���R���|�[�l���g��ǉ�
	template<class T>
	T* AddComponent();

	// �����Ă���R���|�[�l���g���폜
	template<class T>
	bool RemoveComponent();

	GameObject();
	~GameObject();

public:
	std::list<Component*> _componentList;
};


template<class T>
T* GameObject::GetComponent()
{
	for (auto& it : _componentList) {
		T* tmp = dynamic_cast<T*>(it);
		if (tmp != nullptr)
			return tmp;
	}
	return nullptr;
}

template<class T>
T* GameObject::AddComponent()
{
	T* tmp = new T();
	tmp->Parent = this;
	_componentList.push_back(tmp);
	//tmp->Init();
	return tmp;
}

template<class T>
bool GameObject::RemoveComponent()
{
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

