#pragma once
#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

/// <summary>
/// ゲームオブジェクトの表示優先度
/// </summary>
enum class Layer : int {
	Layer_0,	// 3Dオブジェクトなど
	Layer_1,	// 3Dオブジェクトなど
	Layer_2,	// UIなどの2Dスプライトの描画に使用

	Layer_Num,
};

enum class Tag {
	None,
	Ground,
	Player,
	Enemy,

	Tag_Num,
};

class Component;

/// <summary>
/// GameObjectクラス
/// </summary>
class GameObject
{
public:

	void Init();
	void Update();
	void OnCollisionEnter(GameObject* Collision);
	void CompInfo();
	void Uninit();
	void Draw();

	/// <summary>
	/// アタッチされているコンポーネントの取得
	/// </summary>
	/// <typeparam name="T"></typeparam>
	/// <returns>コンポーネントのポインタ</returns>
	template<class T>
	T* GetComponent(){
		for (auto& it : componentList) {
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
		componentList.push_back(tmp);
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
		componentList.push_back(tmp);
		return tmp;
	}

	/// <summary>
	/// 持っているコンポーネントを削除する
	/// </summary>
	/// <typeparam name="T"></typeparam>
	/// <returns>削除が成功したか</returns>
	template<class T>
	bool RemoveComponent() {
		for (auto it = componentList.begin(); it != componentList.end(); ++it) {
			if (typeid(**it) == typeid(T)) {  // typeid で型を比較
				Component* tmp = *it;
				it = componentList.erase(it);  // リストから要素を削除し、イテレータを更新
				delete tmp;  // メモリを解放
				return true;  // 成功
			}
		}
		return false;  // 見つからなかった場合
	}


	std::string& GetObjectName() { return _objectName; }
	Layer& GetLayer() { return _layer; }
	Tag& GetTag() { return _tag; }

	GameObject();
	GameObject(class Scene* scene);
	GameObject(const std::string& name,Layer layer,Tag tag,class Scene* scene);
	~GameObject();


public:
	std::list<Component*> componentList{};
	class Scene* scene = nullptr;

private:
	Tag			_tag{};
	Layer		_layer{};
	std::string _objectName{};
};

#endif // !GAMEOBJECT_H
