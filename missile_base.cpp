#include "main.h"

#include "missile_base.h"
#include "manager.h"
#include "transform.h"
#include "scene.h"

void Missile_Base::TargetTraking(const float& dt)
{
	if (m_pTarget) {
		if (!Manager::GetScene()->GetGameObjectAlive(m_pTarget)) {
			m_pTarget = nullptr;
		}
	}

	if (m_pTarget) {
		Transform* target = m_pTarget->GetComponent<Transform>();
		Transform* parent = Parent->GetComponent<Transform>();

		// ターゲットへの方向を求める
		auto direction = target->position - parent->position;
		direction = VectorNormalize(direction);

		// 前方方向のベクトルをdirectionへ回転させる
		parent->rotation *= DirectX::SimpleMath::Quaternion::FromToRotation(parent->GetForward(), direction);

		// 球面線形補完
		parent->rotation = DirectX::SimpleMath::Quaternion::Slerp(parent->rotation, DirectX::SimpleMath::Quaternion::FromToRotation(parent->GetForward(), direction), m_Mobility * dt);

		// 簡易的な破壊処理
		if (EasyDistance(target->position, parent->position) < 3.f) {
			Parent->Destroy();
		}

		// 距離を保存
		m_TargetDistance = Distance(target->position, parent->position);
	}
}
