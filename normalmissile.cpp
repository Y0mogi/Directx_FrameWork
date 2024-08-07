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
	// ターゲットがぞんざいしているかの確認
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
