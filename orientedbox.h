#pragma once
#include "collision_base.h"
#include "component.h"

class OrientedBox :public Component, public Collision_Base
{
public:
	// 初期化
	void Init()   override;

	// 更新
	void Uninit() override;

	// 開放
	void Update(const float& dt) override;

	// 描画
	void Draw()   override;

	// コンポーネントの情報表示
	void CompInfo() override;   

	/// <summary>
	/// 自身との当たり判定の交差判定
	/// </summary>
	/// <param name="other">判定したい当たり判定のポインタ</param>
	/// <returns>True:交差している False:交差していない</returns>
	bool Intersects(Collision_Base* other) const override;

	/// <summary>
	/// 自身との当たり判定のの内包判定
	/// </summary>
	/// <param name="other">判定したい当たり判定のポインタ</param>
	/// <returns>True:内包している False:内包していない</returns>
	bool Contains(Collision_Base* other) const override;

	// gettter

	inline BoundingOrientedBox& GetCollision() { return m_Collision; }


	// コンストラクタ・デストラクタ
	inline OrientedBox() { SetCollisionType(CollisionType::OBB); }
	inline ~OrientedBox() override {}

	
	
private: 
	BoundingOrientedBox m_Collision;
	class Transform*    m_Transform{};
	
	// デバッグ表示用

	ID3D11VertexShader* m_VertexShader{};
	ID3D11InputLayout* m_VertexLayout{};
	ID3D11PixelShader* m_PixelShader{};
	
};

