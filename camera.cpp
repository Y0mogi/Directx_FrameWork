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
	// �N�I�[�^�j�I���ł̉�]

	if (Input::GetKeyPress('Q')) {// ���_����u���v
		// �N�H�[�^�j�I�����쐬���āAY���𒆐S�ɋt�����ɉ�]����
		Quaternion rotationDelta = Quaternion::AngleAxis((float)(M_PI * 0.01), _transform->GetTop());
		// ���݂̉�]�ɏ悶��
		_transform->rotation = rotationDelta * _transform->rotation;
	}


	
	if (Input::GetKeyPress('E')) {// ���_����u�E�v
		// �N�H�[�^�j�I�����쐬���āAY���𒆐S�ɋt�����ɉ�]����
		Quaternion rotationDelta = Quaternion::AngleAxis((float)(-M_PI * 0.01), _transform->GetTop());
		// ���݂̉�]�ɏ悶��
		_transform->rotation = rotationDelta * _transform->rotation;

	}

	// �J�����������ɖ߂�
	if (Input::GetKeyPress('P'))
	{
		Uninit();
		Init();
	}

	UpdatePosition();

	ImGui::Text("camera Rotation E:x.%f y.%f z.%f", _transform->rotation.ToXMFloat4().x, _transform->rotation.ToXMFloat4().y, _transform->rotation.ToXMFloat4().z);
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

void Camera::UpdatePosition()
{
	XMFLOAT3 forward = _transform->GetForward();
	_transform->position.x = _target.x - forward.x * _length;
	_transform->position.z = _target.z - forward.z * _length;
}
