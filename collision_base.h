#pragma once

class CollisionEvent {
public:
	virtual void OnCollisionEnter(GameObject* collision) {}; // �ڐG���̏���
	virtual void ResolveCollision(GameObject* collision) {}; // �ڐG���̉�������
	
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
	
	virtual bool Intersects(Collision_Base* other) const = 0; // �������Ă��邩
	virtual bool Contains(Collision_Base* other) const = 0;   // ����Ă��邩

	Collision_Base() {};
	virtual ~Collision_Base() {};

private:
	bool _isActive =  true;		// ���̃R���W�������L����
	bool _isHit	   = false;		// �ڐG���Ă��邩
	CollisionType  _type;		// �R���W�����^�C�v
};

