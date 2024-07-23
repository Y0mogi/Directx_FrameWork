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
	
	virtual void Update() = 0;

	inline bool GetEnable() const { return this->m_Enable; }

	Particle_Base(ParticleEmitter* emitter,const XMFLOAT4& color)
		:m_Emitter(emitter),m_Color(color) {};

	virtual ~Particle_Base() {};

public:
	bool				m_Enable{};		// �g�p�t���O
	unsigned short		m_LifeTime{};	// �\������(�t���[��)
	Transform			m_Transform{};	// ���W
	XMFLOAT3			m_Velocity{};	// ���x�x�N�g��
	XMFLOAT3			m_Accel{};		// �����x
	XMFLOAT4			m_Color{};		// �F
	ParticleEmitter*	m_Emitter{};	// ���g���Ǘ�����G�~�b�^�[
};


