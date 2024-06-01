#include "main.h"
#include "renderer.h"
#include "transform.h"
#include "gameobject.h"
#include "input.h"
#include "camera.h"
#include "math_helper.h"


void Camera::Init()
{
	if (_transform == nullptr)
		_transform = Parent->GetComponent<Transform>();
	_transform->position = {0.0f,5.0f,-10.0f};

	// ���_�ƒ����_�̋������v�Z
	float vx, vz;
	vx = _transform->position.x - _target.x;
	vz = _transform->position.z - _target.z;
	_length = sqrtf(vx * vx + vz * vz);
}

void Camera::Uninit()
{

}

void Camera::Update()
{

	if (Input::GetKeyPress('Q')){// ���_����u���v
		_transform->rotation.y += (float)(M_PI * 0.01);
		if (_transform->rotation.y > M_PI)
		{
			_transform->rotation.y -= M_PI * 2.0f;
		}

		_transform->position.x = _target.x - sinf(_transform->rotation.y) * _length;
		_transform->position.z = _target.z - cosf(_transform->rotation.y) * _length;
	}

	if (Input::GetKeyPress('E')){// ���_����u�E�v
		_transform->rotation.y -= (float)(M_PI * 0.01);
		if (_transform->rotation.y < -M_PI)
		{
			_transform->rotation.y += M_PI * 2.0f;
		}

		_transform->position.x = _target.x - sinf(_transform->rotation.y) * _length;
		_transform->position.z = _target.z - cosf(_transform->rotation.y) * _length;
	}

	// �J�����������ɖ߂�
	if (Input::GetKeyPress('P'))
	{
		Uninit();
		Init();
	}

	Renderer::DrawDebugData((char*)"a", _target, false, 1, { 500,500 });
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
