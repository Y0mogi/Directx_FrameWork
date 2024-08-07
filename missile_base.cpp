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

		// �^�[�Q�b�g�ւ̕��������߂�
		auto direction = target->position - parent->position;
		direction = VectorNormalize(direction);

		// �O�������̃x�N�g����direction�։�]������
		parent->rotation *= DirectX::SimpleMath::Quaternion::FromToRotation(parent->GetForward(), direction);

		// ���ʐ��`�⊮
		parent->rotation = DirectX::SimpleMath::Quaternion::Slerp(parent->rotation, DirectX::SimpleMath::Quaternion::FromToRotation(parent->GetForward(), direction), m_Mobility * dt);

		// �ȈՓI�Ȕj�󏈗�
		if (EasyDistance(target->position, parent->position) < 3.f) {
			Parent->Destroy();
		}

		// ������ۑ�
		m_TargetDistance = Distance(target->position, parent->position);
	}
}
