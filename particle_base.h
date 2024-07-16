#pragma once
#include <DirectXMath.h>
#include <string>
#include "transform.h"
using namespace DirectX;


/// <summary>
/// パーティクルベースクラス
/// </summary>
class Particle_Base {
public:
	

	virtual void Update() = 0;

	inline bool GetEnable() const { return m_Enable; }

	Particle_Base(const XMFLOAT3& pos, const XMFLOAT4& color, const std::string& path){};
	virtual ~Particle_Base() {};

	bool					m_Enable{};		// 使用フラグ
	unsigned short			m_LifeTime{};	// 表示時間(フレーム)
	Transform				m_Transform{};	// 座標
	XMFLOAT3				m_Velocity{};	// 速度ベクトル
	XMFLOAT3				m_Accel{};		// 加速度
	XMFLOAT4				m_Color{};		// 色
	std::string				m_Path{};		// ファイルパス
	class ParticleEmitter*	m_Emitter{};	// 自身を管理するエミッター
};


