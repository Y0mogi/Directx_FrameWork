//=============================================================================
// Contents   : �J�����R���|�[�l���g
//  
// Author     : ���� ����
// LastUpdate : 2024/08/01
// Since      : 2024/06/01
//
// === �X�V���O ===
//	2024/08/01	Target�̐^���ɒǏ]����悤�ɏ�����ύX
//		
//	
//	
//=============================================================================
#include "main.h"
#include "renderer.h"

#include "camera.h"
#include "gameobject.h"
#include "transform.h"
#include "input.h"
#include "scene.h"
#include "manager.h"

using namespace DirectX::SimpleMath;

void Camera::Init()
{
	if (m_Transform == nullptr){
		m_Transform = Parent->GetComponent<Transform>();
		NULLSEARCH(m_Transform)
	}

	if (m_Target == nullptr) {
	m_Target = Parent->scene->GetGameObject("Player")->GetComponent<Transform>();
	NULLSEARCH(m_Target)
	}
	

	//// ���_�ƒ����_�̋������v�Z
	//float vx, vz;
	//vx = m_Transform->position.x - m_Target->position.x;
	//vz = m_Transform->position.z - m_Target->position.z;
	//m_Length = sqrtf(vx * vx + vz * vz);
}

void Camera::Uninit()
{
}

void Camera::Update(const float& dt)
{
	return;
}

void Camera::Draw()
{
	m_Transform->rotation = m_Target->rotation;

	// �^�[�Q�b�g�̈ʒu�iXMFLOAT3�Ŏ擾�j
	XMFLOAT3 targetPosition = m_Target->position;

	// �J�����̃I�t�Z�b�g�i�^�[�Q�b�g�̌���ɔz�u�j
	XMFLOAT3 cameraOffset = m_Target->GetBack() * 15;

	cameraOffset += m_Target->GetUp() * 4;

	// �J�����ʒu���X�V
	XMFLOAT3 newPosition = XMFLOAT3(
		targetPosition.x + cameraOffset.x,
		targetPosition.y + cameraOffset.y,
		targetPosition.z + cameraOffset.z
	);

	m_Transform->position = newPosition;

	// �J�����̌������^�[�Q�b�g�ɍ��킹��
	XMFLOAT3 up{};
	XMFLOAT3 forward{};
	if (Input::GetKeyPress('R')) {
		forward = { 0,0,0 };
	}
	else {
		forward = targetPosition + m_Target->GetForward() * 30;
	}
	up = m_Target->GetUp();
	// �r���[�s���ݒ�
	XMMATRIX viewMatrix = XMMatrixLookAtLH(
		XMLoadFloat3(&m_Transform->position),
		XMLoadFloat3(&forward),
		XMLoadFloat3(&up)
	);

	Renderer::SetViewMatrix(viewMatrix);
	XMStoreFloat4x4(&m_ViewMatrix, viewMatrix);

	// �v���W�F�N�V�����}�g���b�N�X�ݒ�
	XMMATRIX projectionMatrix;
	projectionMatrix = XMMatrixPerspectiveFovLH(1.0f, (float)SCREEN_WIDTH / SCREEN_HEIGHT, 0.1f, 1000000.0f);
	Renderer::SetProjectionMatrix(projectionMatrix);
}

void Camera::CompInfo()
{
	using namespace ImGui;
	SeparatorText("Camera");
	if (TreeNode("CAInfo")) {

		auto tmp = m_Target->position;
		DragFloat3("AtPosition", reinterpret_cast<float*>(&tmp));
		DragFloat("AtDistanth", reinterpret_cast<float*>(&m_Length));
		TreePop();
	}
}

void Camera::UpdatePosition()
{
	// �^�[�Q�b�g�ʒu����̑��Έʒu���v�Z
	//Vector3 relativePosition = Vector3(m_Target->position.x, m_Target->position.y,m_Length + 0.0001f);
	Vector3 relativePosition = Vector3(0, 10, 0 + 0.0001f);
	relativePosition = Vector3::Transform(relativePosition, m_Transform->rotation);
	
	// �V�����J�����ʒu���v�Z
	m_Transform->position = XMFLOAT3(relativePosition);
}