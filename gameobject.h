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

	// オブジェクトが持っているコンポーネントを取得
	template<class T>
	T* GetComponent();

	// オブジェクトが持っているコンポーネントを追加
	template<class T>
	T* AddComponent();

	// 持っているコンポーネントを削除
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
		if (typeid(**it) == typeid(T)) {  // typeid で型を比較
			Component* tmp = *it;
			it = _componentList.erase(it);  // リストから要素を削除し、イテレータを更新
			delete tmp;  // メモリを解放
			return true;  // 成功
		}
	}
	return false;  // 見つからなかった場合
}

