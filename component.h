#pragma once
#ifndef COMPONENT_H
#define COMPONENT_H

class GameObject;

/// <summary>
/// コンポーネント基底クラス
/// </summary>
class Component
{
public:

	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Init() {};

	/// <summary>
	/// 終了
	/// </summary>
	virtual void Uninit() {};

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="dt">1フレーム当たりの時間</param>
	virtual void Update(const float& dt) {};

	/// <summary>
	/// 描画
	/// </summary>
	virtual void Draw() {};

	/// <summary>
	/// ImGuiでコンポーネント内の情報を表示
	/// </summary>
	virtual void CompInfo() {};

	Component() {};
	virtual ~Component(){}

public:
	GameObject* Parent = nullptr;	// アタッチされているオブジェクト
};

#endif // COMPONENT_H