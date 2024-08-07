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
	GameObject*		m_pTarget{};				// 追跡するターゲット
	XMFLOAT3		m_TargetPosition{};			// 追跡するターゲット(座標)
	double			m_TargetDistance{};			// ターゲットとの距離
	uint32_t		m_TrakingTime{};			// 追跡時間
	double			m_TrakingDistance{};		// 追跡距離
	uint32_t		m_Mobility = 3;	
	

	const uint32_t	MAXTrakingTime = 45;		// 最大追跡時間(秒)
	const double	MAXTrakingDistance = -1;	// 最大追跡距離(-1は無限)
};