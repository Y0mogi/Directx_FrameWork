#pragma once

class CollisionEvent {
public:
	virtual void OnCollisionEnter(GameObject* collision) {}; // 接触時の処理
	virtual void ResolveCollision(GameObject* collision) {}; // 接触時の応答反応
	
	virtual ~CollisionEvent() {};
};

enum class CollisionType {
	AABB,	
	OBB,
	Sphere,
	Ray, 

	CollisionType_Num
};


class Collision_Base {
public:


	// Geter

	inline bool			 IsActive()			const { return _isActive; }
	inline bool			 IsHit()			const { return _isHit;	  }
	inline CollisionType GetCollisionType() const { return _type;	  }

	// Seter

	inline void SetIsActive(const bool& active) { _isActive = active; }
	inline void SetIsHit   (const bool& hit)	{ _isHit	= hit;	  }
	inline void SetCollisionType(const CollisionType& type) { _type = type; }
	
	virtual bool Intersects(Collision_Base* other) const = 0; // 交差しているか
	virtual bool Contains(Collision_Base* other) const = 0;   // 内包しているか

	Collision_Base() {};
	virtual ~Collision_Base() {};

private:
	bool _isActive =  true;		// このコリジョンが有効か
	bool _isHit	   = false;		// 接触しているか
	CollisionType  _type;		// コリジョンタイプ
};

