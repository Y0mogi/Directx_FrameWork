#pragma once

/// <summary>
/// �ڐG���̃C�x���g�̊��N���X
/// </summary>
class CollisionEvent {
public:
	virtual void OnCollisionEnter(const GameObject* collision) {}; // �ڐG���̏���
	virtual void ResponseCollision(const GameObject* collision) {}; // �ڐG���̉�������
	
	virtual ~CollisionEvent() {};
};

/// <summary>
/// �����蔻��̎�ޔ��ʂɎg�p
/// </summary>
enum class CollisionType {
	AABB,				// �����s���E�{�b�N�X
	OBB,				// �L�����E�{�b�N�X
	Sphere,				// ����
	Ray,				// ����

	CollisionType_Num	// �^�O�̐�
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
	
	virtual bool Intersects(Collision_Base* other) const = 0; // �������Ă��邩
	virtual bool Contains(Collision_Base* other) const = 0;   // ����Ă��邩

	Collision_Base() {};
	virtual ~Collision_Base() {};

private:
	bool m_IsActive = true;		// �ڐG���������邩
	bool m_IsHit	= false;	// �ڐG���Ă��邩
	bool m_OldHit;				// �O�t���[���̐ڐG����
	CollisionType  m_Type{};	// �����蔻��̎��
};

