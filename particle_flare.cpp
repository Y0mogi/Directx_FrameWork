#include "main.h"
#include "renderer.h"
#include "particle_flare.h"
#include "particleemitter.h"
#include "gameobject.h"


void Particle_flare::Update()
{
	
	// �g�p���Ă��邩�`�F�b�N
	if (m_Enable == false) {
	
		m_Enable = true;
		m_Transform.position = m_Emitter->Parent->GetComponent<Transform>()->position;
	
		// �����������_���ɂ���
		m_Velocity.x = (rand() % 100 - 50) / 500.0f;
		m_Velocity.y = (rand() % 100 + 50) / 500.0f;
		m_Velocity.z = (rand() % 100 - 50) / 500.0f;
	
		m_LifeTime = 60;
	
	}

	if (m_Enable == true) {

		// �d�͉����x�����Z����
		m_Velocity.y += -0.01f;

		// ���W���X�V
		m_Transform.position += m_Velocity;

		// ���C�t���f�N�������g
		m_LifeTime--;

		// �g�p�t���O��false��
		if (m_LifeTime <= 0) m_Enable = false;
	}

}


