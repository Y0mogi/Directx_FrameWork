#include "main.h"
#include "renderer.h"
#include "transform.h"
#include "gameobject.h"

#include "camera.h"
#include "input.h"
#include "scene.h"
#include "manager.h"

using namespace DirectX::SimpleMath;

void Camera::Init()
{
	if (m_Transform == nullptr) m_Transform = Parent->GetComponent<Transform>();
		NULLSEARCH(m_Transform)
	m_Transform->position = {0.0f,5.0f,-10.0f};

	if (m_Target == nullptr) m_Target = Parent->scene->GetGameObject("Player")->GetComponent<Transform>();
		NULLSEARCH(m_Target)

	// ���_�ƒ����_�̋������v�Z
	float vx, vz;
	vx = m_Transform->position.x - m_Target->position.x;
	vz = m_Transform->position.z - m_Target->position.z;
	m_Length = sqrtf(vx * vx + vz * vz);
}

void Camera::Uninit()
{
	m_Transform = nullptr;
}

void Camera::Update()
{
	// ��]�̑��x
	static auto rotationSpeed = XM_PI * 0.01f;
	
	// �㉺���E�̉�]
	if (Input::GetKeyPress('Q')) {
		m_Yaw += rotationSpeed;
	}

	if (Input::GetKeyPress('E')) {
		m_Yaw -= rotationSpeed;
	}

	if (Input::GetKeyPress('W')) {
		m_Pitch -= rotationSpeed;
	}

	if (Input::GetKeyPress('S')) {
		m_Pitch += rotationSpeed;
	}

	// �J�����������ɖ߂�
	if (Input::GetKeyPress('P')) {
		Uninit();
		Init();
	}

	
	m_Transform->rotation = Quaternion::CreateFromYawPitchRoll(m_Yaw, m_Pitch, 0.0f);
	//m_Transform->rotation = Quaternion::LookRotation(m_Target->GetForward(),m_Target->GetUnder());

	UpdatePosition();

}

void Camera::Draw()
{
	// �r���[�}�g���b�N�X�ݒ�
	XMFLOAT3 up = m_Transform->GetTop();
	XMMATRIX viewMatrix = XMMatrixLookAtLH(
		XMLoadFloat3(&m_Transform->position),
		XMLoadFloat3(&m_Target->position),
		XMLoadFloat3(&up)
	);

	Renderer::SetViewMatrix(viewMatrix);
	XMStoreFloat4x4(&m_ViewMatrix, viewMatrix);
	
	
	// �v���W�F�N�V�����}�g���b�N�X�ݒ�
	XMMATRIX projectionMatrix;
	projectionMatrix = XMMatrixPerspectiveFovLH(1.0f, (float)SCREEN_WIDTH / SCREEN_HEIGHT, 1.0f, 1000.0f);

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
	Vector3 relativePosition = Vector3(m_Target->position.x, m_Target->position.y,m_Length + 0.0001f);
	relativePosition = Vector3::Transform(relativePosition, m_Transform->rotation);
	
	// �V�����J�����ʒu���v�Z
	m_Transform->position = XMFLOAT3(relativePosition);
}