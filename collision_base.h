#pragma once

/// <summary>
/// 接触時のイベントの基底クラス
/// </summary>
class CollisionEvent {
public:
	virtual void OnCollisionEnter(const GameObject* collision) {}; // 接触時の処理
	virtual void ResponseCollision(const GameObject* collision) {}; // 接触時の応答反応
	
	virtual ~CollisionEvent() {};
};

/// <summary>
/// 当たり判定の種類判別に使用
/// </summary>
enum class CollisionType {
	AABB,				// 軸平行境界ボックス
	OBB,				// 有向境界ボックス
	Sphere,				// 球体
	Ray,				// 光線

	CollisionType_Num	// タグの数
};

/// <summary>
/// 
/// </summary>
class Collision_Base {
public:


	// Geter

	inline bool			 IsActive()			const { return m_IsActive; }
	inline bool			 IsHit()			const { return m_IsHit;	  }
	inline CollisionType GetCollisionType() const { return m_Type;	  }

	// Seter

	inline void SetIsActive(const bool& active) { m_IsActive = active; }
	inline void SetIsHit   (const bool& hit)	{ m_IsHit	= hit;	  }
	inline void SetCollisionType(const CollisionType& type) { m_Type = type; }
	
	virtual bool Intersects(Collision_Base* other) const = 0; // 交差しているか
	virtual bool Contains(Collision_Base* other) const = 0;   // 内包しているか

	Collision_Base() {};
	virtual ~Collision_Base() {};

private:
	bool m_IsActive = true;		// 接触処理をするか
	bool m_IsHit	= false;	// 接触しているか
	bool m_OldHit;				// 前フレームの接触判定
	CollisionType  m_Type{};	// 当たり判定の種類
};

