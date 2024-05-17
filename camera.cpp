#include "main.h"
#include "renderer.h"
#include "transform.h"
#include "gameobject.h"
#include "camera.h"


void Camera::Init()
{
	if (_transform == nullptr)
		_transform = Parent->GetComponent<Transform>();
	_transform->position = {0.0f,5.0f,-10.0f};
	_target = {0.0f,0.0f,0.0f};
}

void Camera::Uninit()
{
}

void Camera::Update()
{
	
}

void Camera::Draw()
{
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
