#include "main.h"
#include "renderer.h"
#include "transform.h"
#include "gameobject.h"
#include "input.h"
#include "camera_fps.h"
#include "math_helper.h"



void Camera_FPS::Init()
{
	if (_transform == nullptr)
		_transform = Parent->GetComponent<Transform>();
}

void Camera_FPS::Uninit()
{

}

void Camera_FPS::Update()
{
	// �J�����̒����_���v���C���[�̑O�������ɃZ�b�g
	float pitch = _transform->rotation.x; // �J�����̋p���擾
	float yaw = _transform->rotation.y; // �J�����̉�]�p���擾

	// �O���������v�Z
	XMFLOAT3 forwardVector;
	forwardVector.x = sinf(yaw) * cosf(pitch);
	forwardVector.y = -sinf(pitch);
	forwardVector.z = cosf(yaw) * cosf(pitch);

	// �J�����̒����_���v���C���[�̑O�������ɃZ�b�g
	_target = _transform->position + forwardVector;

	//Renderer::DrawDebugData((char*)"a", _target, false, 1, { 500,500 });
}

void Camera_FPS::Draw()
{
	Renderer::SetDepthEnable(true);
	// �r���[�}�g���b�N�X�ݒ�
	XMFLOAT3 up = { 0.0f,1.0f,0.0f };
	XMMATRIX viewMatrix = XMMatrixLookAtLH(
		XMLoadFloat3(&_transform->position),
		XMLoadFloat3(&_target),
		XMLoadFloat3(&up)
	);

	Renderer::SetViewMatrix(viewMatrix);
	XMStoreFloat4x4(&_viewMatrix, viewMatrix);

	// �v���W�F�N�V�����}�g���b�N�X�ݒ�
	XMMATRIX projectionMatrix;
	projectionMatrix = XMMatrixPerspectiveFovLH(1.0f, (float)SCREEN_WIDTH / SCREEN_HEIGHT, 1.0f, 1000.0f);

	Renderer::SetProjectionMatrix(projectionMatrix);
}
