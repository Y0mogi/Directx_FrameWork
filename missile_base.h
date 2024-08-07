#pragma once
#include <DirectXMath.h>
#include "component.h"

using namespace DirectX;

class GameObject;

class Missile_Base : public Component{
public:
	void TargetTraking(const float& dt);

	inline GameObject* GetTarget()			{ return this->m_pTarget; }
	inline XMFLOAT3& GetTargetPosition()	{ return this->m_TargetPosition; }
	inline uint32_t& GetTrakingTime()		{ return this->m_TrakingTime;	}
	inline double& GetTrakingDistance()		{ return this->m_TrakingDistance; }

	Missile_Base(GameObject* target)
		:m_pTarget(target) {};
	~Missile_Base() {};
protected:
	GameObject*		m_pTarget{};				// �ǐՂ���^�[�Q�b�g
	XMFLOAT3		m_TargetPosition{};			// �ǐՂ���^�[�Q�b�g(���W)
	double			m_TargetDistance{};			// �^�[�Q�b�g�Ƃ̋���
	uint32_t		m_TrakingTime{};			// �ǐՎ���
	double			m_TrakingDistance{};		// �ǐՋ���
	uint32_t		m_Mobility = 3;	
	

	const uint32_t	MAXTrakingTime = 45;		// �ő�ǐՎ���(�b)
	const double	MAXTrakingDistance = -1;	// �ő�ǐՋ���(-1�͖���)
};