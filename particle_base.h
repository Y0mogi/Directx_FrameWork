#pragma once
#include <DirectXMath.h>
#include <string>
#include "transform.h"
using namespace DirectX;

class ParticleEmitter;
/// <summary>
/// パーティクルベースクラス
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
	bool				m_Enable{};		// 使用フラグ
	unsigned short		m_LifeTime{};	// 表示時間(フレーム)
	Transform			m_Transform{};	// 座標
	XMFLOAT3			m_Velocity{};	// 速度ベクトル
	XMFLOAT3			m_Accel{};		// 加速度
	XMFLOAT4			m_Color{};		// 色
	ParticleEmitter*	m_Emitter{};	// 自身を管理するエミッター
};


