#pragma once
#include <DirectXMath.h>
#include <string>
#include "transform.h"
using namespace DirectX;

class ParticleEmitter;
/// <summary>
/// �p�[�e�B�N���x�[�X�N���X
/// </summary>
class Particle_Base {
public:
	
	virtual void Update(const float& dt) = 0;

	inline bool& GetEnable()		 { return this->m_Enable;	}
	inline unsigned short& GetLife() { return this->m_LifeTime;	}
	inline Transform& GetTransform() { return this->m_Transform;}
	inline XMFLOAT3& GetVelocity()	 { return this->m_Velocity;	}
	inline XMFLOAT3& GetAccel()		 { return this->m_Accel;	}
	inline XMFLOAT4& GetColor()		 { return this->m_Color;	}

	Particle_Base(ParticleEmitter* emitter, const XMFLOAT4& color)
		:m_Emitter(emitter), m_Color(color) {};

	virtual ~Particle_Base() {};

protected:
	bool				m_Enable{};		// �g�p�t���O
	unsigned short		m_LifeTime{};	// �\������(�t���[��)
	Transform			m_Transform{};	// ���W
	XMFLOAT3			m_Velocity{};	// ���x�x�N�g��
	XMFLOAT3			m_Accel{};		// �����x
	XMFLOAT4			m_Color{};		// �F
	ParticleEmitter*	m_Emitter{};	// ���g���Ǘ�����G�~�b�^�[
};


