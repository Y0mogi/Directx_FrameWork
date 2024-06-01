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
	/// アタッチされているコンポーネントの取得
	/// </summary>
	/// <typeparam name="T"></typeparam>
	/// <returns>コンポーネントのポインタ</returns>
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
	/// オブジェクトにコンポーネントをアタッチ
	/// </summary>
	/// <typeparam name="T">追加したいコンポーネントクラス名</typeparam>
	/// <returns>追加したコンポーネントのポインタ</returns>
	template<class T>
	T* AddComponent(){
		T* tmp = new T();
		tmp->Parent = this;
		_componentList.push_back(tmp);
		return tmp;
	}

	/// <summary>
	/// オブジェクトに複数のコンポーネントをアタッチ
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
	/// 持っているコンポーネントを削除する
	/// </summary>
	/// <typeparam name="T"></typeparam>
	/// <returns>削除が成功したか</returns>
	template<class T>
	bool RemoveComponent() {
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

	GameObject();
	~GameObject();

public:
	std::list<Component*> _componentList;
};

#endif // !GAMEOBJECT_H
