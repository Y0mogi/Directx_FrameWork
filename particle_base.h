#pragma once
#include <DirectXMath.h>
#include <string>
#include "transform.h"
using namespace DirectX;


/// <summary>
/// �p�[�e�B�N���x�[�X�N���X
/// </summary>
class Particle_Base {
public:
	

	virtual void Update() = 0;

	inline bool GetEnable() const { return m_Enable; }

	Particle_Base(const XMFLOAT3& pos, const XMFLOAT4& color, const std::string& path){};
	virtual ~Particle_Base() {};

	bool					m_Enable{};		// �g�p�t���O
	unsigned short			m_LifeTime{};	// �\������(�t���[��)
	Transform				m_Transform{};	// ���W
	XMFLOAT3				m_Velocity{};	// ���x�x�N�g��
	XMFLOAT3				m_Accel{};		// �����x
	XMFLOAT4				m_Color{};		// �F
	std::string				m_Path{};		// �t�@�C���p�X
	class ParticleEmitter*	m_Emitter{};	// ���g���Ǘ�����G�~�b�^�[
};


