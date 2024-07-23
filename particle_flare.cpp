#include "main.h"
#include "renderer.h"
#include "particle_flare.h"
#include "particleemitter.h"
#include "gameobject.h"


void Particle_flare::Update()
{
	
	// 使用しているかチェック
	if (m_Enable == false) {
	
		m_Enable = true;
		m_Transform.position = m_Emitter->Parent->GetComponent<Transform>()->position;
	
		// 動きをランダムにする
		m_Velocity.x = (rand() % 100 - 50) / 500.0f;
		m_Velocity.y = (rand() % 100 + 50) / 500.0f;
		m_Velocity.z = (rand() % 100 - 50) / 500.0f;
	
		m_LifeTime = 60;
	
	}

	if (m_Enable == true) {

		// 重力加速度を加算する
		m_Velocity.y += -0.01f;

		// 座標を更新
		m_Transform.position += m_Velocity;

		// ライフをデクリメント
		m_LifeTime--;

		// 使用フラグをfalseに
		if (m_LifeTime <= 0) m_Enable = false;
	}

}


