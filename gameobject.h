#pragma once

enum class Layer;
enum class Tag;
class Component;

/// <summary>
/// GameObjectクラス
/// </summary>
class GameObject
{
public:

	void Init();	// 初期化
	void Update();	// 更新
	void Uninit();	// 開放
	void Draw();	// 描画
	inline bool IsDiscard(){
		if (this->m_Discard) {
			this->Uninit();
			return true;
		}
		else {
			return false;
		}
	}   

	inline void Discard() { this->m_Discard = true; };	// 削除予約
	void OnCollisionEnter(GameObject* Collision);	// 衝突処理
	void CompInfo();								// コンポーネントの情報表示

	/// <summary>
	/// アタッチされているコンポーネントの取得
	/// </summary>
	/// <typeparam name="T">取得したいコンポーネントの型</typeparam>
	/// <returns>コンポーネントのポインタ</returns>
	template<class T>
	T* GetComponent(){
		for (auto& it : this->componentList) {
			T* tmp = dynamic_cast<T*>(it);
			if (tmp != nullptr)
				return tmp;
		}
		return nullptr;
	}

	/// <summary>
	/// オブジェクトにコンポーネントをアタッチ
	/// </summary>
	/// <typeparam name="T">追加したいコンポーネントの型</typeparam>
	/// <returns>追加したコンポーネントのポインタ</returns>
	template<class T>
	T* AddComponent(){
		T* tmp = new T();
		tmp->Parent = this;
		this->componentList.push_back(tmp);
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
		this->componentList.push_back(tmp);
		return tmp;
	}

	/// <summary>
	/// 持っているコンポーネントを削除する
	/// </summary>
	/// <typeparam name="T"></typeparam>
	/// <returns>削除が成功したか</returns>
	template<class T>
	bool RemoveComponent() {
		for (auto it = this->componentList.begin(); it != this->componentList.end(); ++it) {
			if (typeid(**it) == typeid(T)) {  // typeid で型を比較
				Component* tmp = *it;
				it = this->componentList.erase(it);  // リストから要素を削除し、イテレータを更新
				delete tmp;  // メモリを解放
				return true;  // 成功
			}
		}
		return false;  // 見つからなかった場合
	}

	// getter

	std::string&	GetObjectName()	{ return this->m_ObjectName;	}
	Layer&			GetLayer()		{ return this->m_Layer;		}
	Tag&			GetTag()		{ return this->m_Tag;			}


	// コンストラクタ・デストラクタ

	GameObject();
	GameObject(class Scene* scene);
	GameObject(const std::string& name,Layer layer,Tag tag,class Scene* scene);
	~GameObject();


public:
	std::list<Component*> componentList{};	// 自身についているコンポーネント
	class Scene* scene = nullptr;			// 自身を管理しているシーンのポインタ

private:
	bool		m_Discard = false;			// 削除予約
	Tag			m_Tag{};					// 自身のタグ
	Layer		m_Layer{};					// 表示レイヤー
	std::string m_ObjectName{};				// オブジェクトの固有名詞
};

/// <summary>
/// ゲームオブジェクトの表示優先度
/// </summary>
enum class Layer : int {
	Layer_0,		// 3Dオブジェクトなど
	Layer_1,		// 3Dオブジェクトなど
	Layer_2,		// UIなどの2Dスプライトの描画に使用

	Layer_Num,
};

/// <summary>
/// GameObjectクラスのタグ
/// </summary>
enum class Tag {
	None,
	Ground,
	Player,
	Enemy,

	Tag_Num,
};
