#include "normalmissile.h"
#include "manager.h"
#include "scene.h"

void NormalMissile::Init() {
	return;
}

void NormalMissile::Uninit()
{
}

void NormalMissile::Update(const float& dt)
{
	// �^�[�Q�b�g�����񂴂����Ă��邩�̊m�F
	if (!Manager::GetScene()->GetGameObjectAlive(m_pTarget) && m_pTarget) {
		m_pTarget = nullptr;
	}

	if (m_pTarget) {
		
	}
}

void NormalMissile::Draw()
{

}

void NormalMissile::CompInfo()
{

}
